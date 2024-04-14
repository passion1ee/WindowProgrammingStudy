#pragma once
#include "ysObserver.h"
class Person : public ys::Observer
{
public:
	Person() : size(30) {}

	void Init(const ys::fVector& position, const ys::fVector& velocity);
	virtual void Update(ys::TrffLightSignal signal, RECT screen) final;
	void Render(HDC hdc, RECT screen);

	void setVelocity(ys::fVector val) { velocity = val; }
	ys::fVector getVelocity() const { return velocity; }

private:
	bool isCrossHorizontalRoad;

	BYTE size;
	ys::fVector direction;
	ys::fVector velocity;
	ys::fVector position;
};

