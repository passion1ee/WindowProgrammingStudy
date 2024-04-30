#pragma once
#include "stdfax.h"
#include <cmath>

namespace ys
{
	class Planet
	{
		struct Circle
		{
			COLORREF color;
			Vector2 center;
			float radius;
			double radian;
			double satelliteRadian;
			int angSpeed;
			int satelliteAngSpeed;

			bool isRightRotate;
			//Circle¿« center.x + radius * cos(@), center.y + radius * sin(@)
			bool isSatelliteRightRotate;

			Circle(const COLORREF& color, const Vector2& center, float radius)
				: color(color), center(center), radius(radius), radian(0), satelliteRadian(0), angSpeed(30), satelliteAngSpeed(50), isRightRotate(true), isSatelliteRightRotate(true)
			{
			}
		};
	public:
		void Init(HWND hWnd_, RECT screenSize);
		void click(const int& x, const int& y);
		void Run();

	private:
		void Update();
		void Render();
		void setScreen(RECT screenSize);

	private:
		HWND hWnd;
		HDC hDc;
		HDC hBackDc;
		HBITMAP hBitmap;

		RECT screen;
		float width;
		float height;
		Vector2 mousePosition;

	private:
		
		std::vector<Circle> planets;
		std::vector<Circle>::iterator selectPlanet;

		bool mouseclick;
	};
}

