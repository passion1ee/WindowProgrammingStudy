#include "Intersection.h"
#include <cmath>

constexpr float setTrafficTime = 3.0f;

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
		person = std::make_shared<Person>();
		person->Init(ys::fVector{ width / 3 - person->getSize(),  height / 3 - person->getSize() }, ys::fVector{1, 0});

		horizontalTL.Clear();
		horizontalTL.SetState(TrffLightSignal::GREEN, screen);
		horizontalTL.Init(RECT(width / 12, 20, width * 7 / 24, height / 6));

		verticalTL.Clear();
		verticalTL.SetState(TrffLightSignal::RED, screen);
		verticalTL.Init(RECT(width * 2 / 3 + width / 12,  height * 2 / 3 + 20,  width * 2 / 3 + width * 7 / 24, height * 2 / 3 + height / 6));

		trafficTime = 0;
		isAuto = true;
		isBeep = true;
		isCenterEmpty = true;

		//TrafficLight에 Observer등록
		horizontalTL.Attach(person);
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
		if (Timer::getRealFPS() == 0.0f)
			InputManager::BeforeUpdate();
		Timer::Update();
		if(Timer::getRealFPS() != 0.0f){
			if (1 / Timer::getRealFPS() <= (frameCheck += Timer::getDeltaTime()))
			{
				InputManager::BeforeUpdate();
				frameCheck -= 1 / Timer::getRealFPS();
				Update();
				Render();
				InputManager::AfterUpdate();
			}
		}
		else if (Timer::getRealFPS() == 0.0f)
		{
			PatBlt(hBackDC, 0, 0, screen.right - screen.left, screen.bottom - screen.top, WHITENESS);
			std::wstring line;
			line += L"일시 정지";
			SIZE size;  GetTextExtentPoint32(hBackDC, line.c_str(), line.size(), &size);
			TextOut(hBackDC, (screen.right - screen.left) / 2 - size.cx, (screen.bottom - screen.top) / 2, line.c_str(), line.size());
			BitBlt(hDC, 0, 0, screen.right - screen.left, screen.bottom - screen.top, hBackDC, 0, 0, SRCCOPY);
			InputManager::AfterUpdate();
		}
	}

	void Intersection::Render()
	{
		PatBlt(hBackDC, 0, 0, screen.right - screen.left, screen.bottom - screen.top, WHITENESS);
		renderFrame(hBackDC);
		for (auto& car : cars)
			car->Render(hBackDC, screen);
		person->Render(hBackDC);

		horizontalTL.Render(hBackDC);
		verticalTL.Render(hBackDC);
		Timer::Render(hBackDC, POINT(screen.right - screen.left, screen.bottom - screen.top));
		std::wstring line;
		line += std::to_wstring(trafficTime);
		TextOut(hBackDC, 100, 400, line.c_str(), line.size());
		BitBlt(hDC, 0, 0, screen.right - screen.left, screen.bottom - screen.top, hBackDC, 0, 0, SRCCOPY);
	}

	void Intersection::Update()
	{
		KeyUpdate();

		if (isAuto)//자동이면 타이머를 쓴다 아니면 0.0으로 고정한다
			trafficTime += 1 / ys::Timer::getRealFPS();
		else
			trafficTime = 0.0f;
		//YELLOW
		if (trafficTime >= setTrafficTime - 1.0f)
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
		//RED && GREEN
		if (trafficTime >= setTrafficTime && isCenterEmpty)
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
		{
			horizontalTL.SetState(horizontalTL.GetState(), screen);
			if (horizontalTL.CenterEmpty())
				isCenterEmpty = true;
			else
				isCenterEmpty = false;
		}

		if (verticalTL.GetState() == TrffLightSignal::GREENtoYELLOW)
		{
			verticalTL.SetState(verticalTL.GetState(), screen);
			if (verticalTL.CenterEmpty())
				isCenterEmpty = true;
			else
				isCenterEmpty = false;
		}


		if (horizontalTL.GetState() == TrffLightSignal::GREEN)
			horizontalTL.SetState(horizontalTL.GetState(), screen);

		if (verticalTL.GetState() == TrffLightSignal::GREEN)
			verticalTL.SetState(verticalTL.GetState(), screen);

	}

	void Intersection::click(int x, int y)
	{
		mousePosition = { static_cast<float>(x), static_cast<float>(y) };
		//해당하는 신호등의 원을 클릭했는가 -> 신호등은 그 원에 해당하는 메세지 전달
		//반대쪽 신호등은 그 반대 메세지 전달
		//신호 ChangeTime을 reset
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
				trafficTime = setTrafficTime - 1.0f;
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
				trafficTime = setTrafficTime - 1.0f;
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
		//중앙선
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
		//가운데
		{
			auto pen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
			auto oldPen = SelectObject(hdc, pen);

			Rectangle(hdc, width / 3, height / 3, width * 2 / 3, height * 2 / 3);

			SelectObject(hdc, oldPen);
			DeleteObject(pen);
		}
		//외각블럭
		{
			auto pen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
			auto oldPen = SelectObject(hdc, pen);
			Rectangle(hdc, 0, 0, width / 3, height / 3);
			Rectangle(hdc, width * 2 / 3, 0, width, height / 3);
			Rectangle(hdc, 0, height * 2 / 3, width / 3, height);
			Rectangle(hdc, width * 2 / 3, height * 2 / 3, width, height);
			SelectObject(hdc, oldPen);
			DeleteObject(pen);
		}
	}

}