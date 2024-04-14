#pragma once
#include "stdfax.h"
#include <Windows.h>
class Car;

class CarState
{
public:
	virtual void Move(Car* car, RECT screen) = 0;
	virtual void StopAccel(Car* car) = 0;
	virtual void MoveAccel(Car* car) = 0;
	virtual void Accel(Car* car) = 0;
	virtual void Decel(Car* car) = 0;

	virtual void Render(HDC hdc, Car* car, RECT screen) = 0;
};

class HorizontalMove: public CarState
{
public:
	void Move(Car* car, RECT screen) override;
	void StopAccel(Car* car) override;
	void MoveAccel(Car* car) override;
	void Accel(Car* car) override;
	void Decel(Car* car) override;

	void Render(HDC hdc, Car* car, RECT screen) override;
};

class HorizontalReverseMove : public CarState
{
public:
	void Move(Car* car, RECT screen) override;
	void StopAccel(Car* car) override;
	void MoveAccel(Car* car) override;
	void Accel(Car* car) override;
	void Decel(Car* car) override;

	void Render(HDC hdc, Car* car, RECT screen) override;
};

class VerticalMove : public CarState
{
public:
	void Move(Car* car, RECT screen) override;
	void StopAccel(Car* car) override;
	void MoveAccel(Car* car) override;
	void Accel(Car* car) override;
	void Decel(Car* car) override;

	void Render(HDC hdc, Car* car, RECT screen) override;
};

class VerticalReverseMove : public CarState
{
public:
	void Move(Car* car, RECT screen) override;
	void StopAccel(Car* car) override;
	void MoveAccel(Car* car) override;
	void Accel(Car* car) override;
	void Decel(Car* car) override;

	void Render(HDC hdc, Car* car, RECT screen) override;
};

