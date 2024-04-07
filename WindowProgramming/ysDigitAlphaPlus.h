#pragma once
#include <windows.h>
#include <vector>
#include "../../WinProgramming/MyEngine_source/ysInputManager.h"

namespace ys
{
	class DigitAlphaPlus
	{
	public:
		struct Object
		{
			int id;
			int count;
			bool isAlpha;
			POINT position;

			Object() = delete;
			Object(int alphabet_, POINT _position, bool isAlpha_,int _id)
				: count(alphabet_), position(_position), isAlpha(isAlpha_), id(_id) {}

			bool operator==(const Object& other)
			{
				if (id != other.id)
					return false;
				if (count != other.count)
					return false;
				if (position.x != other.position.x || position.y != other.position.y)
					return false;
				return true;
			}
		};

	public:
		static void setScreen(RECT screenSize);

		static void Init(HWND hWnd_, RECT screenSize);
		static void Run();
		static void render();
		static void select(WPARAM w);

	private:
		static void Update();
		static void renderFrame();
		static void renderObject(Object object);
	private:
		static HWND hWnd;
		static HDC hDC;
		static HDC hBackDC;
		static HBITMAP hBitmap;

		static POINT screen;

		static BYTE mapSize;
		static double xGrid;
		static double yGrid;

		static BYTE mod;// 1 2 3
		static float coolTime;

		static POINT player;
		static std::vector<Object> alphabets;
	};
}