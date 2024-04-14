#include "Intersection.h"
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

		horizontalTL.Clear();
		verticalTL.Clear();
		horizontalTL.SetState(TrffLightSignal::RED, screen);
		verticalTL.SetState(TrffLightSignal::GREEN, screen);
		trafficTime = 0;
		isAuto = true;

		auto width = screen.right - screen.left;
		auto height = screen.bottom - screen.top;
		//((height * 2 / 3) - (height / 2)) / 2;
		//((width * 2 / 3) - (width / 2)) / 2;

		//TrafficLight�� Observer���
		for (int i = 0; i < 2; ++i)
		{
			std::shared_ptr<CarState> state = std::make_shared<HorizontalMove>();
			std::shared_ptr<Car> car = std::make_shared<Car>();
			car->Init(state, ys::fVector{ 0, (height * 7 / 12) - (car->getSize() / 2) }, ys::fVector{ i * 100.0f, 0 });
			horizontalTL.Attach(car);
		}
		for (int i = 0; i < 2; ++i)
		{
			std::shared_ptr<CarState> state = std::make_shared<HorizontalReverseMove>();
			std::shared_ptr<Car> car = std::make_shared<Car>();
			car->Init(state, ys::fVector{ 0, (height * 5 / 12) - (car->getSize() / 2) }, ys::fVector{ i * -100.0f, 0 });
			horizontalTL.Attach(car);
		}
		for (int i = 0; i < 2; ++i)
		{
			std::shared_ptr<CarState> state = std::make_shared<VerticalMove>();
			std::shared_ptr<Car> car = std::make_shared<Car>();
			car->Init(state, ys::fVector{ (width * 5 / 12) - (car->getSize() / 2), 0 }, ys::fVector{ 0, i * 100.0f });
			verticalTL.Attach(car);
		}
		for (int i = 0; i < 2; ++i)
		{
			std::shared_ptr<CarState> state = std::make_shared<VerticalReverseMove>();
			std::shared_ptr<Car> car = std::make_shared<Car>();
			car->Init(state, ys::fVector{ (width * 7 / 12) - (car->getSize() / 2), 0 }, ys::fVector{ 0, i * -100.0f });
			verticalTL.Attach(car);
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

	void Intersection::Update()
	{
		KeyUpdate();

		if (isAuto)//�ڵ��̸� Ÿ�̸Ӹ� ���� �ƴϸ� 0.0���� �����Ѵ�
			trafficTime += 1 / ys::Timer::getRealFPS();			

		if (trafficTime >= 4.0f)
		{
			if (horizontalTL.GetState() == TrffLightSignal::GREEN)
				horizontalTL.SetState(TrffLightSignal::GREENtoYELLOW, screen);

			if (verticalTL.GetState() == TrffLightSignal::GREEN)
				horizontalTL.SetState(TrffLightSignal::GREENtoYELLOW, screen);
		}

		if (trafficTime >= 5.0f)
		{
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
	}

	void Intersection::renderFrame(HDC hdc)
	{
		auto width = screen.right - screen.left;
		auto height = screen.bottom - screen.top;
		//�߾Ӽ�
		{
			auto pen = CreatePen(PS_SOLID, 5, RGB(255, 255, 0));
			auto oldPen = SelectObject(hdc, pen);

			MoveToEx(hdc, 0, height / 2, NULL);
			LineTo(hdc, width, height / 2);
			MoveToEx(hdc, width / 2, 0, NULL);
			LineTo(hdc, width / 2, height);

			SelectObject(hdc, oldPen);
			DeleteObject(pen);
		}
		//���
		{
			auto pen = CreatePen(PS_SOLID, 5, RGB(255, 255, 0));
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


	void Intersection::Render()
	{
		renderFrame(hBackDC);
	}

	void Intersection::click(int x, int y)
	{
		//�ش��ϴ� ��ȣ���� ���� Ŭ���ߴ°� -> ��ȣ���� �� ���� �ش��ϴ� �޼��� ����
		//�ݴ��� ��ȣ���� �� �ݴ� �޼��� ����
		//��ȣ ChangeTime�� reset
	}
}