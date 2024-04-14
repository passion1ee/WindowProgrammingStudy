#pragma once
#include "ysObserver.h"
#include "stdfax.h"
#include <Windows.h>

class Car : public ys::Observer
{
public:
	virtual void Update(BYTE signal) override { currentState = signal; }
	BYTE GetState() const { return currentState; }

private:
	BYTE id;
	BYTE currentState;
	RECT position;
};