#include "TrafficLight.h"
#include "ysObserver.h"

void TrafficLight::Notify()
{
	for (auto observer : observers)
	{
		observer->Update(state);
	}
}
