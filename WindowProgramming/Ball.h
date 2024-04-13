#pragma once
#include <Windows.h>
#include "stdfax.h"
class Ball
{
public:
	Ball();

	void Init(ys::fVector position_);
	void Update(RECT& clientRect, bool isStop);
	void Render(HDC hdc);
	//bool CheckCollision(Brick& brick);

	void setDirection(ys::fVector val)
	{
		Direction.x = val.x;
		Direction.y = val.y;
	}
	void setDirection(float xVal, float yVal)
	{
		Direction.x = xVal;
		Direction.y = yVal;
	}
	void setPosition(float xVal, float yVal) {
		position.x = xVal;
		position.y = yVal;
	}

	const int& getSize() const { return size; }
	ys::fVector& getDirection() { return Direction; }
	const ys::fVector& getPosition() const { return position; }
private:
	ys::fVector position;
	ys::fVector Direction;
	float velocity;
	int size;
};