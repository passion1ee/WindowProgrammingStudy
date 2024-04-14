#pragma once
#include "ysObserver.h"
#include "CarState.h"
#include "stdfax.h"
#include <Windows.h>
#include "ysSubject.h"

class Car : public ys::Observer
{
public:
	Car() = default;

public:
	void Init(const std::shared_ptr<CarState>& myState, const ys::fVector& position, const ys::fVector& velocity);
	virtual void Update(ys::TrffLightSignal signal, RECT screen) final;
	void Render(HDC hdc, RECT screen);

	void setState(std::shared_ptr <CarState> state_) { myState = state_; }

	void setVelocity(ys::fVector val) { velocity = val; }
	ys::fVector getVelocity() const { return velocity; }

	void setPosition(ys::fVector position_) { position = position_; }
	ys::fVector getPosition() const { return position; }

	void setSpeed(float speed) { maxSpeed = speed; }
	float getSpeed() const { return maxSpeed; }
	float getSize() const { return size; }

	void Stop();
	void Move(Car& car, RECT screen) { if (myState) myState->Move(car, screen); }
	void StopAccel(Car& car) { if (myState) myState->StopAccel(car); }
	void MoveAccel(Car& car) { if (myState) myState->MoveAccel(car); }
	void Accel(Car& car) { if (myState) myState->Accel(car); }
	void Decel(Car& car) { if (myState) myState->Decel(car); }

private:
	std::shared_ptr <CarState> myState;
	BYTE size;

	ys::fVector position;
	ys::fVector velocity;// H(1, 0)rH(-1, 0), V(0, 1)rV(0, -1)
	float maxSpeed;
};