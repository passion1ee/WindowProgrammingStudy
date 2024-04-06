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
		static void setScreen(int width, int height);

		static void Init();
		static void Run(HWND hWnd);
		static void render(HDC hDC);
		static void select(WPARAM w);

	private:
		static void Update(HWND hWnd);
		static void renderFrame(HDC hDC);
		static void renderObject(HDC hDC, Object object);
	private:
		static int screenWidth;
		static int screenHeight;

		static BYTE mapSize;
		static double xGrid;
		static double yGrid;

		static BYTE mod;// 1 2 3

		static POINT player;
		static std::vector<Object> alphabets;
	};
}