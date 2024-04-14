#pragma once
#include "ysSubject.h"
#include "stdfax.h"
#include <Windows.h>
#include <list>

class ys::Observer;

class TrafficLight : public ys::Subject
{
public:
	TrafficLight() : state(ys::TrffLightSignal::GREENtoYELLOW), position(RECT()) {}

	virtual void Attach(ys::Observer* observer) override
	{
		observers.push_back(observer);
	}
	virtual void Detach(ys::Observer* observer) override
	{
		observers.remove(observer);
	}

	virtual void Notify(RECT screen) override;
	void SetState(ys::TrffLightSignal signal, RECT screen)
	{
		state = signal;
		Notify(screen);
	}
	ys::TrffLightSignal GetState() const { return state; }

private:
	std::list<ys::Observer*> observers;
	ys::TrffLightSignal state;
	RECT position;
};