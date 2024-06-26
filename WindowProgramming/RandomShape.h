#pragma once
#include <windows.h>
#include <vector>
//#include "../../WinProgramming/MyEngine_source/ysInputManager.h"
enum class Shape : BYTE
{
	kX, kSnake, kDiamond, kButterfly, kTriangle2, kRect3, kCount
};
struct Section
{
	RECT rect;
	Shape shape;

	Section() = default;
};
namespace ys
{
	class RandomShape
	{
	public:
		static void setScreen(int width, int height);
		static void setSection(int row, int col);
		static void render(HDC hDC);

		static void renderFrame(HDC hDC);
		static void renderShape(HDC hDC);

	private:
		static int screenWidth;
		static int screenHeight;

		static std::vector<Section> sections;
	};
}