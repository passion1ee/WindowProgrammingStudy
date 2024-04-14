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

	void Init(const RECT& position);
	void Render(HDC hdc);

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

public:
	std::pair<ys::fVector, float> getREDcircle() const;
	std::pair<ys::fVector, float> getYELLOWcircle() const;
	std::pair<ys::fVector, float> getGREENcircle() const;

	bool CenterEmpty() const { return isCenterEmpty; }
private:
	std::list<std::shared_ptr<ys::Observer>> observers;
	ys::TrffLightSignal state;
	RECT position;
	bool isCenterEmpty;
};

