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

void Car::Update(ys::TrffLightSignal signal, RECT screen)
{
	if (ys::InputManager::getKeyDown(VK_ADD)) {	Accel(*this); }
	if (ys::InputManager::getKeyDown(VK_SUBTRACT)) { Decel(*this); }

	switch (signal)
	{
	case ys::TrffLightSignal::RED:
		Stop();
		break;
	case ys::TrffLightSignal::GREENtoYELLOW:
		StopAccel(*this);
		Move(*this, screen);
		break;
	case ys::TrffLightSignal::GREEN:
		MoveAccel(*this);
		Move(*this, screen);
		break;
	default:
		break;
	}
}

void Car::Render(HDC hdc, RECT screen)
{
	if (myState)
		myState->Render(hdc, *this, screen);
}

void Car::Stop()
{
	velocity = { 0.0f, 0.0f };
}