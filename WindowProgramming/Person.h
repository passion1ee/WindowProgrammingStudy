#pragma once
#include "ysObserver.h"
class Person : public ys::Observer
{
public:
	Person() : size(40) {}

	void Init(const ys::fVector& position, const ys::fVector& velocity);
	virtual void Update(ys::TrffLightSignal signal, RECT screen) final;
	void Render(HDC hdc);

	void setVelocity(ys::fVector val) { velocity = val; }
	ys::fVector getVelocity() const { return velocity; }

	float getSize() const { return size; }

private:
	void Move(const RECT& screen);
	void setVelocity(const RECT& screen);
	bool sameRect(const RECT& one, const RECT& other);
private:
	ys::fVector velocity;
	ys::fVector position;
	BYTE size;

	BYTE collideNum;
};

