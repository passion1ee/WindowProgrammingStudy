#pragma once
#ifdef PRACTICE__2_6
#include <windows.h>
#include <vector>
//#include "../../WinProgramming/MyEngine_source/ysInputManager.h"
namespace ys
{
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
#endif PRACTICE__2_6