#pragma once
#include <windows.h>
#include <vector>
#include "../../WinProgramming/MyEngine_source/ysInputManager.h"

namespace ys
{
	class ShapeLand
	{
	public:
		enum class Shape : BYTE
		{
			kCircle, kTriangle, kRect, kCount, kPentagon, kPie, kHourglass, kStar, kEmpty
		};

		struct Object
		{
			int size;
			COLORREF color;
			Shape shape;
			POINT position;

			Object() = delete;
			Object(Shape _shape, COLORREF _color, POINT _position, int _size)
				: shape(_shape), color(_color), position(_position), size(_size) {}

			bool operator==(const Object& other)
			{
				if (size != other.size)
					return false;
				if (shape != other.shape)
					return false;
				if (color != other.color)
					return false;
				if (position.x != other.position.x || position.y != other.position.y)
					return false;
				return true;
			}
		};

	public:
		static void setScreen(int width, int height);

		static void Init();
		static void Run(HWND hWnd);
		static void render(HDC hDC);
		static void select(WPARAM w);

	private:
		static void Update(HWND hWnd);
		static void renderFrame(HDC hDC);
		static void renderObject(HDC hDC, Object object);
		static void collide(HWND hWnd);

	private:
		static int screenWidth;
		static int screenHeight;

		static BYTE mapSize;
		static double xGrid;
		static double yGrid;

		static size_t selected;
		static std::vector<Object> objects;
		static std::vector<Object> prevObjects;
		static std::vector<COLORREF> colors;
	};

}