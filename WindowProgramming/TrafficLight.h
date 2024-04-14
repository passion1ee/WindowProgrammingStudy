#pragma once
#include "ysSubject.h"
#include "stdfax.h"
#include <Windows.h>
#include <list>

class ys::Observer;

class TrafficLight : public ys::Subject
{
public:
	TrafficLight() : state(0) {}

	virtual void Attach(ys::Observer* observer) override
	{
		observers.push_back(observer);
	}
	virtual void Detach(ys::Observer* observer) override
	{
		observers.remove(observer);
	}

	virtual void Notify() override;

	void SetState(BYTE signal)
	{
		state = signal;
		Notify();
	}
	BYTE GetState() const { return state; }

private:
	std::list<ys::Observer*> observers;
	BYTE state;
	RECT position;
};