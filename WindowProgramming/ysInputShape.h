#pragma once
#include <windows.h>
#include <vector>
#include <sstream>


namespace ys
{
	class InputShape
	{
	public:
		enum class Shape : BYTE
		{
			kDot, kLine, kTriangle, kRect, kPentagon, kCircle, kCount, kEmpty
		};

		struct Object
		{
			Shape shape;
			std::pair<COLORREF, COLORREF> colors;//테두리, 내부
			RECT position;
			int size;
			int penWidth; //1~10

			Object() : size(100) {}

			Object(Shape _shape, std::pair<COLORREF, COLORREF> _color, RECT _position, int _size, int _penWidth)
				: shape(_shape), colors(_color), position(_position), size(_size), penWidth(_penWidth)
			{}

		};

	public:
		static void setScreen(int width, int height);
		static void TextBoxClicked(HWND hWnd, int x, int y);

		static void Init();
		static void Run();
		static void render(HWND hWnd, HDC hDC);
		static void Add(WPARAM buff);

	private:
		static void Update();
		static void renderObject(HDC hDC, Object object);
		static bool isVal(Object& input);

	private://global
		static int screenWidth;
		static int screenHeight;
		static bool isTextBoxClicked;
		static bool isPrintAll;

	private://textbox
		static RECT textBox;
		static std::wstring stringBuff;
		static int nextCharIndex;
		static bool isUpper;
		static bool isInsert;

	private://Object
		static std::vector<Object> objectPool;
		static std::vector<Shape> shapes;
	};

}
