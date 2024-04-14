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

	switch (signal)
	{
	case ys::TrffLightSignal::RED:
		Stop();
		break;
	case ys::TrffLightSignal::GREENtoYELLOW:
	{
		auto width = screen.right - screen.left;
		auto height = screen.bottom - screen.top;
		RECT carRect = { position.x, position.y, position.x + size, position.y + size };
		RECT rect = { width / 3, height / 3, width * 2 / 3, height * 2 / 3 };
		RECT collideRect;
		if (!IntersectRect(&collideRect, &carRect, &rect))
		{
			StopAccel(*this);
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
		auto brush = CreateSolidBrush(RGB(255, 128, 0));
		auto oldBrush = (HBRUSH)SelectObject(hdc, brush);
		auto pen = CreatePen(PS_SOLID, 8, RGB(128, 64, 0));
		auto oldPen = SelectObject(hdc, pen);
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