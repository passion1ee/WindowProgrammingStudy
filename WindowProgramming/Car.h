#pragma once
#include "ysObserver.h"
#include "CarState.h"
#include "stdfax.h"
#include <Windows.h>

class Car : public ys::Observer
{
public:

	void Init();
	virtual void Update(ys::TrffLightSignal signal, RECT screen) override;
	void Render(HDC hdc, RECT screen);

	void setState(CarState* state_) { myState = state_; }

	void setVelocity(ys::fVector val) { velocity = val; }
	ys::fVector getVelocity() const { return velocity; }

	void setPosition(ys::fVector position_) { position = position_; }
	ys::fVector getPosition() const { return position; }

	void setSpeed(float speed) { maxSpeed = speed; }
	float getSpeed() const { return maxSpeed; }
	float getSize() const { return size; }

	void Stop();
	void Move(RECT screen) { if (myState) myState->Move(this, screen); }
	void StopAccel() { if (myState) myState->StopAccel(this); }
	void MoveAccel() { if (myState) myState->MoveAccel(this); }
	void Accel() { if (myState) myState->Accel(this); }
	void Decel() { if (myState) myState->Decel(this); }

private:
	CarState* myState;
	BYTE size;

	ys::fVector position;
	ys::fVector velocity;// H(1, 0)rH(-1, 0), V(0, 1)rV(0, -1)
	float maxSpeed;
};
//+ - maxSpeed도 올리고 velocity도 올리고