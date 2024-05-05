#include "Planet.h"
#include <algorithm>
#include "../../WinProgramming/MyEngine_source/ysMath.h"

std::uniform_int_distribution<> randomRadius(10, 60);

void ys::Planet::Init(HWND hWnd_, RECT screenSize)
{
	hWnd = hWnd_;
	hDc = GetDC(hWnd_);
	hBackDc = CreateCompatibleDC(hDc);

	setScreen(screenSize);

	planets.reserve(20);
	for(int i = 0; i < 10; ++i)
	{
		planets.push_back(Circle(RGB(ObjectColor(randomEngine), ObjectColor(randomEngine), ObjectColor(randomEngine)),
			Vector2({ static_cast<float>(uid(randomEngine) % 1280),  static_cast<float>(uid(randomEngine) % 720) }),
			randomRadius(randomEngine)));
	}
	mouseclick = false;

	InputManager::Init();
	Timer::Init();
}

void ys::Planet::click(const int& x, const int& y)
{
	mousePosition.x = x;
	mousePosition.y = y;

	if(mouseclick)
	{
		selectPlanet->center.x = x;
		selectPlanet->center.y = y;
	}
}

void ys::Planet::Run()
{
	InputManager::BeforeUpdate();
	Timer::Update();

	if (Timer::getDeltaTime() > 0.0f) 
	{
		Update();
		Render();
	}
	if (InputManager::getKeyDown((UINT)Key::R))
	{
		planets.clear();
		planets.reserve(20);
		for (int i = 0; i < 10; ++i)
		{
			planets.push_back(Circle(RGB(ObjectColor(randomEngine), ObjectColor(randomEngine), ObjectColor(randomEngine)),
				Vector2({ static_cast<float>(uid(randomEngine) % 1220),  static_cast<float>(uid(randomEngine) % 660) }),
				randomRadius(randomEngine)));
		}
		mouseclick = false;
	}
	if (InputManager::getKeyDown((UINT)Key::Q))
	{
		PostMessage(hWnd, WM_CLOSE, 0, 0);
	}
	InputManager::AfterUpdate();
}

void ys::Planet::Update()
{
	for (auto& planet : planets)
	{
		if (planet.isRightRotate)
			planet.radian = std::fmod((planet.radian + (planet.angSpeed / Timer::getRealFPS()) * kPi / 18), (2 * kPi));
		else
			planet.radian = std::fmod((planet.radian - (planet.angSpeed / Timer::getRealFPS()) * kPi / 18), (2 * kPi));
		if (planet.isSatelliteRightRotate)
			planet.satelliteRadian = std::fmod((planet.satelliteRadian + (planet.satelliteAngSpeed / Timer::getRealFPS()) * kPi / 18), (2 * kPi));
		else
			planet.satelliteRadian = std::fmod((planet.satelliteRadian - (planet.satelliteAngSpeed / Timer::getRealFPS()) * kPi / 18), (2 * kPi));
	}

	if (InputManager::getKeyDown((UINT)Key::Num1))
		for (auto& planet : planets)
			planet.isRightRotate = planet.isRightRotate ? false : true;
	
	if (InputManager::getKeyDown((UINT)Key::Num2))
		for (auto& planet : planets)
			planet.isSatelliteRightRotate = planet.isSatelliteRightRotate ? false : true;

	if (InputManager::getKeyDown((UINT)Key::U) && mouseclick)
		if (!InputManager::getKey(VK_SHIFT))
			selectPlanet->radius += 1;
		else if (selectPlanet->radius > 1)
			selectPlanet->radius -= 1;

	if (InputManager::getKeyDown((UINT)Key::Num3) && mouseclick)
	{
		if (!InputManager::getKey(VK_SHIFT))
		{
			if (selectPlanet->angSpeed < 75)
				selectPlanet->angSpeed += 5;
		}
		else if(selectPlanet->angSpeed > 5)
			selectPlanet->angSpeed -= 5;
	}
	
	if (InputManager::getKeyDown((UINT)Key::Num4) && mouseclick)
	{
		if (!InputManager::getKey(VK_SHIFT))
		{
			if (selectPlanet->satelliteAngSpeed < 150)
				selectPlanet->satelliteAngSpeed += 5;
		}
		else if(selectPlanet->satelliteAngSpeed > 5)
			selectPlanet->satelliteAngSpeed -= 5;
	}

	if (InputManager::getKeyDown((UINT)Key::C))
	{
		for (auto& planet : planets)
		{
			if (uid(randomEngine) % 3 != 0) continue;
			planet.color = planet.color ^ 0xFFFFFF;
		}
	}

	if (InputManager::getKeyDown(VK_LBUTTON))
	{
		for(auto iter = planets.begin(); iter != planets.end(); ++iter)
		{
			if (circlePointCollide(iter->center, iter->radius, mousePosition))
			{
				selectPlanet = iter;
				mouseclick = true;
				break;
			}
		}

		if(!mouseclick)
		{
			if (planets.size() >= 20)
			{
				std::reverse(planets.begin(), planets.end());
				planets.pop_back();
				std::reverse(planets.begin(), planets.end());
			}
			planets.push_back(Circle(RGB(ObjectColor(randomEngine), ObjectColor(randomEngine), ObjectColor(randomEngine)),
				Vector2({ mousePosition.x, mousePosition.y }),
				randomRadius(randomEngine)));
		}
	}
	if (InputManager::getKeyUp(VK_LBUTTON))
	{
		mouseclick = false;
	}
}

void ys::Planet::Render()
{
	PatBlt(hBackDc, 0, 0, screen.right - screen.left, screen.bottom - screen.top, WHITENESS);

	std::wstring line = L"(Num3): 속도 Up      (Shift + Num3): 속도 Down";
	SIZE size;  GetTextExtentPoint32(hBackDc, line.c_str(), line.size(), &size);
	TextOut(hBackDc, width / 2 - size.cx / 2, height / 15, line.c_str(), line.size());
	line = L"(Num4): 위성 속도 Up      (Shift + Num4): 위성 속도 Down";
	GetTextExtentPoint32(hBackDc, line.c_str(), line.size(), &size);
	TextOut(hBackDc, width / 2 - size.cx / 2, height * 2 / 15, line.c_str(), line.size());
	line = L"(U): 크기 증가      (Shift + U): 크기 감소";
	GetTextExtentPoint32(hBackDc, line.c_str(), line.size(), &size);
	TextOut(hBackDc, width / 2 - size.cx / 2, height * 3 / 15, line.c_str(), line.size());

	for (auto& planet : planets)
	{
		auto brush = CreateSolidBrush(planet.color);
		auto oldBrush = SelectObject(hBackDc, brush);
		Ellipse(hBackDc, planet.center.x - planet.radius, planet.center.y - planet.radius,
			planet.center.x + planet.radius, planet.center.y + planet.radius);
		SelectObject(hBackDc, oldBrush);
		DeleteObject(brush);

		double radian = planet.radian;
		do
		{
			auto edgeX = planet.center.x + planet.radius * std::cos(radian);
			auto edgeY = planet.center.y + planet.radius * std::sin(radian);
			auto OthSideEdgeX = planet.center.x + planet.radius * std::cos(radian + kPi);
			auto OthSideEdgeY = planet.center.y + planet.radius * std::sin(radian + kPi);

			MoveToEx(hBackDc, edgeX, edgeY, NULL);
			LineTo(hBackDc, OthSideEdgeX, OthSideEdgeY);

			radian += kPi / 4;
		} while (radian < planet.radian + 2 * kPi);

		auto edgeX = planet.center.x + planet.radius * std::cos(planet.satelliteRadian);
		auto edgeY = planet.center.y + planet.radius * std::sin(planet.satelliteRadian);

		brush = CreateSolidBrush(RGB(255,255,0));
		oldBrush = SelectObject(hBackDc, brush);
		Ellipse(hBackDc, edgeX - 5, edgeY - 5, edgeX + 5, edgeY + 5);
		SelectObject(hBackDc, oldBrush);
		DeleteObject(brush);
	}
	BitBlt(hDc, 0, 0, screen.right - screen.left, screen.bottom - screen.top, hBackDc, 0, 0, SRCCOPY);
}

void ys::Planet::setScreen(RECT screenSize)
{
	screen = screenSize;
	width = screen.right - screen.left;
	height = screen.bottom - screen.top;
	SetWindowPos(hWnd, nullptr, 0, 0, screen.right - screen.left, screen.bottom - screen.top, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

	hBitmap = CreateCompatibleBitmap(hDc, screen.right - screen.left, screen.bottom - screen.top);
	auto oldBitmap = SelectObject(hBackDc, hBitmap);
	DeleteObject(oldBitmap);
}