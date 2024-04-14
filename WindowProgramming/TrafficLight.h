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
	TrafficLight(const std::list<std::shared_ptr<ys::Observer>>& observers, const ys::TrffLightSignal& state, const RECT& position)
		: observers(observers), state(state), position(position) {}

	virtual void Attach(std::shared_ptr<ys::Observer> observer) override
	{
		observers.push_back(observer);
	}
	virtual void Detach(std::shared_ptr<ys::Observer> observer) override
	{
		observers.remove(observer);
	}

	void Clear()
	{
		observers.clear();
	}

	virtual void Notify(RECT screen) override;

	void SetState(ys::TrffLightSignal signal, RECT screen)
	{
		state = signal;
		Notify(screen);
	}
	ys::TrffLightSignal GetState() const { return state; }

private:
	std::list<std::shared_ptr<ys::Observer>> observers;
	ys::TrffLightSignal state;
	RECT position;
};

