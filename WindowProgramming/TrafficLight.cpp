#include "TrafficLight.h"
#include "ysObserver.h"

void TrafficLight::Init(const RECT& position)
{
	this->position = position;
}

void TrafficLight::Render(HDC hdc)
{
	auto width = position.right - position.left;
	auto height = position.bottom - position.top;
	HBRUSH brush;
	HBRUSH oldBrush;

	brush = CreateSolidBrush(RGB(0, 0, 0));
	oldBrush = (HBRUSH)SelectObject(hdc, brush);
	Rectangle(hdc, position.left, position.top, position.right, position.bottom);
	SelectObject(hdc, oldBrush);
	DeleteObject(brush);

	if(state == ys::TrffLightSignal::RED)
		brush = CreateSolidBrush(RGB(255, 0, 0));
	else
		brush = CreateSolidBrush(RGB(75, 0, 0));
	oldBrush = (HBRUSH)SelectObject(hdc, brush);
	Ellipse(hdc, position.left, position.top, position.left + width / 3, position.bottom);
	SelectObject(hdc, oldBrush);
	DeleteObject(brush);

	if (state == ys::TrffLightSignal::GREENtoYELLOW)
		brush = CreateSolidBrush(RGB(255, 255, 0));
	else
		brush = CreateSolidBrush(RGB(75, 75, 0));
	
	oldBrush = (HBRUSH)SelectObject(hdc, brush);
	Ellipse(hdc, position.left + width / 3, position.top, position.left + width * 2 / 3, position.bottom);
	SelectObject(hdc, oldBrush);
	DeleteObject(brush);

	if (state == ys::TrffLightSignal::GREEN)
		brush = CreateSolidBrush(RGB(0, 255, 0));
	else
		brush = CreateSolidBrush(RGB(0, 75, 0));
	oldBrush = (HBRUSH)SelectObject(hdc, brush);
	Ellipse(hdc, position.left + width * 2 / 3, position.top, position.left + width, position.bottom);
	SelectObject(hdc, oldBrush);
	DeleteObject(brush);
}

void TrafficLight::Notify(RECT screen)
{
	for (auto& observer : observers)
	{
		observer->Update(state, screen);
	}
}

