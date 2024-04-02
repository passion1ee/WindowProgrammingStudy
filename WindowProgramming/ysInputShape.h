#pragma once
#include <windows.h>
#include <vector>
#include <sstream>


namespace ys
{
	enum class Shape : BYTE
	{
		kDot, kLine, kTriangle, kRect, kPentagon, kCircle, kCount, kEmpty
	};

	struct Object
	{
		int size;
		Shape shape;
		COLORREF color;
		RECT position;

		Object() : shape(Shape::kEmpty), color(0), position({0, 0, 0, 0}), size(300) {}

		Object(Shape _shape, COLORREF _color, RECT _position, int _size)
			: shape(_shape), color(_color), position(_position), size(_size)
		{}

	};

	class InputShape
	{

	public:
		static void setScreen(int width, int height);
		static void TextBoxClicked(HWND hWnd, int x, int y);

		static void Init();
		static void Run();
		static void render(HDC hDC);
		static void Add(WPARAM buff);

	private:
		static void Update();
		static void renderFrame(HDC hDC);
		static void renderObject(HDC hDC, Object object);
		static bool isVal();

	private://global
		static int screenWidth;
		static int screenHeight;

	private://textbox
		static RECT textBox;
		static bool isTextBoxClicked;
		static std::wstring stringBuff;
		static int nextCharIndex;
		static bool isUpper;
		static bool isInsert;

	private://Object
		static std::vector<Object> selectObj;
		static size_t selectedIndex;
		static std::vector<Shape> shapes;
	};

}
