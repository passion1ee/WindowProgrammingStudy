#include "Car.h"
#include "ysSubject.h"
#include <cmath>

void Car::Init(const std::shared_ptr<CarState>& myState, const ys::fVector& position, const ys::fVector& velocity)
{
	size = 20;
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
		MoveAccel(*this);
		break;
	case ys::TrffLightSignal::GREENtoYELLOW:
		StopAccel(*this);
		break;
	case ys::TrffLightSignal::GREEN:
		Move(*this, screen);
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
