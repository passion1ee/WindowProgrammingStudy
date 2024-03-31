#pragma once
#include <windows.h>
#include <vector>
namespace ys
{
	enum class Shapes : BYTE
	{
		kTriangle, kHourglass, kPentagon, kPie
	};

	struct Shape
	{
		Shapes shape;
		COLORREF color;
	};

	class selectShape
	{
	public:

		static void setScreen(int width, int height);

		static void Run();
		static void Init();
		static void Update();

		static void render(HDC hDC);
		static void renderShape(HDC hDC, Shape shape, RECT rect);

	private:
		static int screenWidth;
		static int screenHeight;
		static RECT mainRect;
		static std::vector<RECT> selectRect;
		static Shape selected;
		static std::vector<Shape> shapes;
	};
}

