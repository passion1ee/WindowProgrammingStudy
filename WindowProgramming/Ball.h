#pragma once
#include <Windows.h>
#include "stdfax.h"
class Ball
{
public:
	Ball();

	void Init(ys::fVector position_);
	void Update(RECT& clientRect);
	void Render(HDC hdc);

	void setVelocity(ys::fVector val) 
	{
		velocity.x = val.x;
		velocity.y = val.y;
	}

	ys::fVector& getVelocity() { return velocity; }
	const ys::fVector& getPosition() const { return position; }
private:
	ys::fVector position;
	ys::fVector Direction;
	ys::fVector velocity;
	int size;
};