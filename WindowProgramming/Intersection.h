#pragma once
#include "TrafficLight.h"
#include "Car.h"

namespace ys
{
	class Intersection
	{
	public:
		void Init();
		void Update();
		void Render();

	private:
		TrafficLight horizontalTL;
		TrafficLight verticalTL;
		std::vector<std::shared_ptr<Car>> cars;
	};
}

