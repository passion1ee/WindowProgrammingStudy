#include "Intersection.h"
#include <cmath>

namespace ys
{
	void Intersection::setScreen(RECT screenSize)
	{
		::AdjustWindowRect(&screenSize, WS_OVERLAPPEDWINDOW, FALSE);
		screen = screenSize;

		SetWindowPos(hWnd, nullptr, 0, 0, screen.right - screen.left, screen.bottom - screen.top, SWP_NOMOVE | SWP_NOZORDER);

		hBitmap = CreateCompatibleBitmap(hDC, screen.right - screen.left, screen.bottom - screen.top);
		auto oldBitmap = SelectObject(hBackDC, hBitmap);
		DeleteObject(oldBitmap);
	}

	void Intersection::Init(HWND hWnd_, RECT screenSize)
	{
		hWnd = hWnd_;
		hDC = GetDC(hWnd_);
		hBackDC = CreateCompatibleDC(hDC);

		Intersection::setScreen(screenSize);
		auto width = screen.right - screen.left;
		auto height = screen.bottom - screen.top;

		cars.clear();
		cars.reserve(8);

		horizontalTL.Clear();
		horizontalTL.SetState(TrffLightSignal::GREEN, screen);
		horizontalTL.Init(RECT(width / 12, 20, width * 7 / 24, height / 6));

		verticalTL.Clear();
		verticalTL.SetState(TrffLightSignal::RED, screen);
		verticalTL.Init(RECT(width * 2 / 3 + width / 12,  height * 2 / 3 + 20,  width * 2 / 3 + width * 7 / 24, height * 2 / 3 + height / 6));

		trafficTime = 0;
		isAuto = true;
		isBeep = true;

		//TrafficLight�� Observer���
		for (int i = 0; i < 2; ++i)
		{
			std::shared_ptr<CarState> state = std::make_shared<HorizontalMove>();
			std::shared_ptr<Car> car = std::make_shared<Car>();
			car->Init(state, ys::fVector{ 0, (height * 7 / 12) - (car->getSize() / 2) }, ys::fVector{ ( i + 1) * 100.0f, 0 });
			horizontalTL.Attach(car);
			cars.push_back(car);
		}
		for (int i = 0; i < 2; ++i)
		{
			std::shared_ptr<CarState> state = std::make_shared<HorizontalReverseMove>();
			std::shared_ptr<Car> car = std::make_shared<Car>();
			car->Init(state, ys::fVector{ 0, (height * 5 / 12) - (car->getSize() / 2) }, ys::fVector{ ( i + 1 ) * -100.0f, 0 });
			horizontalTL.Attach(car);
			cars.push_back(car);
		}
		for (int i = 0; i < 2; ++i)
		{
			std::shared_ptr<CarState> state = std::make_shared<VerticalMove>();
			std::shared_ptr<Car> car = std::make_shared<Car>();
			car->Init(state, ys::fVector{ (width * 5 / 12) - (car->getSize() / 2), 0 }, ys::fVector{ 0, ( i + 1 ) * 100.0f });
			verticalTL.Attach(car);
			cars.push_back(car);
		}
		for (int i = 0; i < 2; ++i)
		{
			std::shared_ptr<CarState> state = std::make_shared<VerticalReverseMove>();
			std::shared_ptr<Car> car = std::make_shared<Car>();
			car->Init(state, ys::fVector{ (width * 7 / 12) - (car->getSize() / 2), 0 }, ys::fVector{ 0, ( i + 1 ) * -100.0f });
			verticalTL.Attach(car);
			cars.push_back(car);
		}

		Timer::Init();
	}

	void Intersection::Run()
	{
		Timer::Update();
		if (1 / Timer::getRealFPS() <= (frameCheck += Timer::getDeltaTime()))
		{
			InputManager::BeforeUpdate();
			frameCheck -= 1 / Timer::getRealFPS();
			Update();
			Render();
			Timer::Render(hDC, POINT(screen.right - screen.left, screen.bottom - screen.top));
			InputManager::AfterUpdate();
		}
	}

	void Intersection::Render()
	{
		PatBlt(hBackDC, 0, 0, screen.right - screen.left, screen.bottom - screen.top, WHITENESS);
		renderFrame(hBackDC);
		for (auto& car : cars)
			car->Render(hBackDC, screen);
		horizontalTL.Render(hBackDC);
		verticalTL.Render(hBackDC);
		BitBlt(hDC, 0, 0, screen.right - screen.left, screen.bottom - screen.top, hBackDC, 0, 0, SRCCOPY);
	}

	void Intersection::Update()
	{
		KeyUpdate();

		if (isAuto)//�ڵ��̸� Ÿ�̸Ӹ� ���� �ƴϸ� 0.0���� �����Ѵ�
			trafficTime += 1 / ys::Timer::getRealFPS();
		else
			trafficTime = 0.0f;

		if (trafficTime >= 4.0f)
		{
			if (isBeep)
			{
				Beep(1000, 100);
				isBeep = false;
			}
				
			if (horizontalTL.GetState() == TrffLightSignal::GREEN)
				horizontalTL.SetState(TrffLightSignal::GREENtoYELLOW, screen);

			if (verticalTL.GetState() == TrffLightSignal::GREEN)
				verticalTL.SetState(TrffLightSignal::GREENtoYELLOW, screen);
		}

		if (trafficTime >= 5.0f)
		{
			isBeep = true;
			Beep(2000, 100);
			if (horizontalTL.GetState() == TrffLightSignal::GREENtoYELLOW)
			{
				horizontalTL.SetState(TrffLightSignal::RED, screen);
				verticalTL.SetState(TrffLightSignal::GREEN, screen);
			}

			if (verticalTL.GetState() == TrffLightSignal::GREENtoYELLOW)
			{
				verticalTL.SetState(TrffLightSignal::RED, screen);
				horizontalTL.SetState(TrffLightSignal::GREEN, screen);
			}
			trafficTime = 0.0f;
		}


		if (horizontalTL.GetState() == TrffLightSignal::GREENtoYELLOW) 
			horizontalTL.SetState(TrffLightSignal::GREENtoYELLOW, screen);

		if (verticalTL.GetState() == TrffLightSignal::GREENtoYELLOW)
			verticalTL.SetState(TrffLightSignal::GREENtoYELLOW, screen);

		if (horizontalTL.GetState() == TrffLightSignal::GREEN)
			horizontalTL.SetState(TrffLightSignal::GREEN, screen);

		if (verticalTL.GetState() == TrffLightSignal::GREEN)
			verticalTL.SetState(TrffLightSignal::GREEN, screen);
	}

	void Intersection::click(int x, int y)
	{
		mousePosition = { static_cast<float>(x), static_cast<float>(y) };
		//�ش��ϴ� ��ȣ���� ���� Ŭ���ߴ°� -> ��ȣ���� �� ���� �ش��ϴ� �޼��� ����
		//�ݴ��� ��ȣ���� �� �ݴ� �޼��� ����
		//��ȣ ChangeTime�� reset
	}

	inline void Intersection::KeyUpdate()
	{
		if (ys::InputManager::getKeyUp((UINT)ys::Key::Q))
		{
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}

		if (ys::InputManager::getKeyUp((UINT)ys::Key::A))
		{
			isAuto = isAuto ? false : true;
			if(!isAuto) trafficTime = 0.0f;
		}

		if (ys::InputManager::getKeyUp(VK_LBUTTON))
		{
			const auto& hoRED = horizontalTL.getREDcircle();
			const auto& hoYELLOW = horizontalTL.getYELLOWcircle();
			const auto& hoGREEN = horizontalTL.getGREENcircle();

			const auto& veRED = verticalTL.getREDcircle();
			const auto& veYELLOW = verticalTL.getYELLOWcircle();
			const auto& veGREEN = verticalTL.getGREENcircle();
			if (circleCollide(hoRED, mousePosition.x, mousePosition.y))
			{
				Beep(3000, 100);
				horizontalTL.SetState(TrffLightSignal::RED, screen);
				verticalTL.SetState(TrffLightSignal::GREEN, screen);
				trafficTime = 0.0f;
			}
			if (circleCollide(hoYELLOW, mousePosition.x, mousePosition.y))
			{
				Beep(3000, 100);
				horizontalTL.SetState(TrffLightSignal::GREENtoYELLOW, screen);
				verticalTL.SetState(TrffLightSignal::RED, screen);
				trafficTime = 4.0f;
			}
			if (circleCollide(hoGREEN, mousePosition.x, mousePosition.y))
			{
				Beep(3000, 100);
				horizontalTL.SetState(TrffLightSignal::GREEN, screen);
				verticalTL.SetState(TrffLightSignal::RED, screen);
				trafficTime = 0.0f;
			}
			if (circleCollide(veRED, mousePosition.x, mousePosition.y))
			{
				Beep(3000, 100);
				verticalTL.SetState(TrffLightSignal::RED, screen);
				horizontalTL.SetState(TrffLightSignal::GREEN, screen);
				trafficTime = 0.0f;
			}
			if (circleCollide(veYELLOW, mousePosition.x, mousePosition.y))
			{
				Beep(3000, 100);
				verticalTL.SetState(TrffLightSignal::GREENtoYELLOW, screen);
				horizontalTL.SetState(TrffLightSignal::RED, screen);
				trafficTime = 4.0f;
			}
			if (circleCollide(veGREEN, mousePosition.x, mousePosition.y))
			{
				Beep(3000, 100);
				verticalTL.SetState(TrffLightSignal::GREEN, screen);
				horizontalTL.SetState(TrffLightSignal::RED, screen);
				trafficTime = 0.0f;
			}
		}
	}

	void Intersection::renderFrame(HDC hdc)
	{
		auto width = screen.right - screen.left;
		auto height = screen.bottom - screen.top;
		//�߾Ӽ�
		{
			auto pen = CreatePen(PS_SOLID, 8, RGB(255, 255, 0));
			auto oldPen = SelectObject(hdc, pen);

			MoveToEx(hdc, 0, height / 2, NULL);
			LineTo(hdc, width, height / 2);
			MoveToEx(hdc, width / 2, 0, NULL);
			LineTo(hdc, width / 2, height);

			SelectObject(hdc, oldPen);
			DeleteObject(pen);

			pen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
			oldPen = SelectObject(hdc, pen);

			MoveToEx(hdc, 0, height / 2, NULL);
			LineTo(hdc, width, height / 2);
			MoveToEx(hdc, width / 2, 0, NULL);
			LineTo(hdc, width / 2, height);

			SelectObject(hdc, oldPen);
			DeleteObject(pen);
		}
		//���
		{
			auto pen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
			auto oldPen = SelectObject(hdc, pen);

			Rectangle(hdc, width / 3, height / 3, width * 2 / 3, height * 2 / 3);

			SelectObject(hdc, oldPen);
			DeleteObject(pen);
		}
		//�ܰ���
		{
			Rectangle(hdc, 0, 0, width / 3, height / 3);
			Rectangle(hdc, width * 2 / 3, 0, width, height / 3);
			Rectangle(hdc, 0, height * 2 / 3, width / 3, height);
			Rectangle(hdc, width * 2 / 3, height * 2 / 3, width, height);
		}
	}

}