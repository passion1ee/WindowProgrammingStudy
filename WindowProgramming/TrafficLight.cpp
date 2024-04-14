#include "TrafficLight.h"
#include "ysObserver.h"

void TrafficLight::Notify(RECT screen)
{
	for (auto observer : observers)
	{
		observer->Update(state, screen);
	}
}
