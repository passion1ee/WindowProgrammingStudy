#pragma once
#include "TrafficLight.h"
#include "Car.h"
#include "Person.h"

namespace ys
{
	class Intersection
	{
	public:
		void setScreen(RECT screenSize);
		void Init(HWND hWnd_, RECT screenSize);
		void Run();
		void Render();

	public:
		void click(int x, int y);

	private:
		inline bool circleCollide(std::pair<ys::fVector, float> circle, int x, int y) const
		{ return(sqrt(std::pow(x - circle.first.x, 2) + std::pow(y - circle.first.y, 2)) < circle.second); }

		void Update();
		inline void KeyUpdate();
		void renderFrame(HDC hdc);
		
	private:
		HWND hWnd;
		HDC hDC;
		HDC hBackDC;
		HBITMAP hBitmap;

		RECT screen;

		ys::fVector mousePosition;

		float frameCheck;
		float trafficTime;
		bool isAuto;
		bool isBeep;
		bool isCenterEmpty;

		std::shared_ptr<Person> person;
		std::vector<std::shared_ptr<Car>> cars;
		TrafficLight horizontalTL;
		TrafficLight verticalTL;
	};
	
}

