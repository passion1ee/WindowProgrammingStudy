#pragma once
#include "TrafficLight.h"
#include "Car.h"

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
		void Update();
		inline void KeyUpdate();
		void renderFrame(HDC hdc);
		
	private:
		HWND hWnd;
		HDC hDC;
		HDC hBackDC;
		HBITMAP hBitmap;

		RECT screen;

		float frameCheck;
		float trafficTime;
		bool isAuto;
		bool isBeep;

		std::vector<std::shared_ptr<Car>> cars;
		TrafficLight horizontalTL;
		TrafficLight verticalTL;
	};
	
}

