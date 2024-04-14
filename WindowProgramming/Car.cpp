#include "Car.h"

void Car::Init()
{
	size = 20;
}

void Car::Update(ys::TrffLightSignal signal, RECT screen)
{
	if (ys::InputManager::getKeyDown(VK_ADD)) {	Accel(); }
	if (ys::InputManager::getKeyDown(VK_SUBTRACT)) { Decel(); }

	switch (signal)
	{
	case ys::TrffLightSignal::RED:
		Stop();
		break;
	case ys::TrffLightSignal::REDtoYELLOW:
		MoveAccel();
		break;
	case ys::TrffLightSignal::GREENtoYELLOW:
		StopAccel();
		break;
	case ys::TrffLightSignal::GREEN:
		Move(screen);
		break;
	}
}

void Car::Render(HDC hdc, RECT screen)
{
	if (myState)
		myState->Render(hdc, this, screen);
}

void Car::Stop()
{
	velocity = { 0.0f, 0.0f };
}

