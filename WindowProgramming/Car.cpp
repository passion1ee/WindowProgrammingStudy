#include "Car.h"
#include "ysSubject.h"
#include <cmath>

void Car::Init(const std::shared_ptr<CarState>& myState, const ys::fVector& position, const ys::fVector& velocity)
{
	this->myState = myState;
	this->position = position;
	this->velocity = velocity;
	maxSpeed = std::abs(velocity.x) >= std::abs(velocity.y) ? std::abs(velocity.x) : std::abs(velocity.y);
}

bool Car::Update(ys::TrffLightSignal signal, RECT screen)
{
	if (ys::InputManager::getKeyDown(VK_ADD)) {	Accel(*this); }
	if (ys::InputManager::getKeyDown(VK_SUBTRACT)) { Decel(*this); }

	auto width = screen.right - screen.left;
	auto height = screen.bottom - screen.top;
	RECT carRect = { position.x, position.y, position.x + size, position.y + size };
	RECT rect1 = { width / 3 - 100, height / 3 - 100, width * 2 / 3 + 100, height * 2 / 3 + 100 };//Áß¾Ó RECT
	RECT rect2 = stateRect(screen);//¹þ¾î³­ ÀÌÈÄ RECT
	RECT collideRect;

	switch (signal)
	{
	case ys::TrffLightSignal::RED:
		if(!IntersectRect(&collideRect, &carRect, &rect1))
		{
			MoveAccel(*this);
			Move(*this, screen);
		}
		else
			StopAccel(*this);
		break;
	case ys::TrffLightSignal::GREENtoYELLOW:
	{
		if (!IntersectRect(&collideRect, &carRect, &rect1))
		{
			Move(*this, screen);
			return true;
		}
		else
		{
			MoveAccel(*this);
			Move(*this, screen);
			return false;
		}
		break;
	}
	case ys::TrffLightSignal::GREEN:
		MoveAccel(*this);
		Move(*this, screen);
		break;
	default:
		break;
	}
	return true;
}

void Car::Render(HDC hdc, RECT screen)
{
	if (myState)
	{
		auto pen = CreatePen(PS_SOLID, 8, RGB(128, 64, 0));
		auto oldPen = SelectObject(hdc, pen);
		auto brush = CreateSolidBrush(RGB(255, 128, 0));
		auto oldBrush = (HBRUSH)SelectObject(hdc, brush);
		myState->Render(hdc, *this, screen);
		SelectObject(hdc, oldBrush);
		SelectObject(hdc, oldPen);
		DeleteObject(brush);
		DeleteObject(pen);
	}
}

void Car::Stop()
{
	velocity = { 0.0f, 0.0f };
}

bool Car::sameRect(const RECT& one, const RECT& other)
{
	return one.left == other.left && one.top == other.top && one.right == other.right && one.bottom == other.bottom;
}

RECT Car::stateRect(RECT screen)
{
	auto width = screen.right - screen.left;
	auto height = screen.bottom - screen.top;
	if (std::dynamic_pointer_cast<HorizontalMove>(myState))
		return RECT(width * 2 / 3, height / 2, width, height * 2 / 3);
	if (std::dynamic_pointer_cast<HorizontalReverseMove>(myState))
		return RECT(0, height / 3, width / 3, height / 2);
	if (std::dynamic_pointer_cast<VerticalMove>(myState))
		return RECT(width / 3, height * 2 / 3, width / 2, height);
	if (std::dynamic_pointer_cast<VerticalReverseMove>(myState))
		return RECT(width / 2, 0, width * 2 / 3, height / 3);
}
