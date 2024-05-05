#pragma once
#include "stdfax.h"
namespace ys
{
	constexpr BYTE kMapSize = 40;
	class PaintBoard
	{
		struct Object
		{
			struct childObj
			{
				POINT position;
				void Move(const bool& isVertical, const bool& isPlus)
				{
					if(isVertical)
					{
						if (isPlus)
						{
							if (position.y < kMapSize - 1)
								position.y++;
							else
								position.y = 0;
						}
						else
						{
							if (position.y > 0)
								position.y--;
							else
								position.y = kMapSize - 1;
						}
					}
					else
					{
						if (isPlus)
						{
							if (position.x < kMapSize - 1)
								position.x++;
							else
								position.x = 0;
						}
						else
						{
							if (position.x > 0)
								position.x--;
							else
								position.x = kMapSize - 1;
						}
					}
				}
			};
			RECT size;
			COLORREF color;
			std::vector<childObj> childObjs;

			void Move(const bool& isVertical, const bool& isPlus)
			{
				if (isVertical)
				{
					if (isPlus)
					{
						if (size.top < kMapSize - 1)
							OffsetRect(&size, 0, 1);
						else
							SetRect(&size, size.left, 0, size.right, size.bottom - size.top);
					}
					else
					{
						if (size.bottom > 0)
							OffsetRect(&size, 0, -1);
						else
							SetRect(&size, size.left, kMapSize - 1 - (size.bottom - size.top), size.right, kMapSize - 1);
					}
				}
				else
				{
					if (isPlus)
					{
						if (size.left < kMapSize - 1)
							OffsetRect(&size, 1, 0);
						else
							SetRect(&size, 0, size.top, size.right - size.left, size.bottom);
					}
					else
					{
						if (size.right > 0)
							OffsetRect(&size, -1, 0);
						else
							SetRect(&size, kMapSize - 1 - (size.right - size.left), size.top, kMapSize - 1, size.bottom);
					}
				}
				for (auto& childObj : childObjs)
					childObj.Move(isVertical, isPlus);
			}

			Object(const RECT& size, const COLORREF& color)
				: size(size), color(color), childObjs(childObjs)
			{
				for (int j = size.top; j < size.bottom; ++j)
					for (int i = size.left; i < size.right; ++i)
						childObjs.push_back(childObj(POINT(i, j)));
			}

			bool operator==(const Object& other) const
			{
				return size.left == other.size.left
					&& size.right == other.size.right
					&& size.top == other.size.top
					&& size.bottom == other.size.bottom
					&& color == other.color;
			}
		};
		enum class Color
		{
			BLACK, RED, BLUE, GREEN, MAGENTA, Cyan, Yellow
		};
	public:
		void Init(HWND hWnd_, RECT& screenSize);
		void Lclick(const int& x, const int& y);
		void Rclick(const int& x, const int& y);
		void Run();
		void command(const WORD& command);

	private:
		void Update();
		void Render();
		void renderFrame();
		void setScreen(RECT& screenSize);
		COLORREF ColorToColorRef(const Color& color);
		std::wstring ColorTostring(const COLORREF& color);
	private:
		HWND hWnd;
		HDC hDc;
		HDC hBackDc;
		HBITMAP hBitmap;

		RECT screen;
		float width;
		float height;
		double xGrid;
		double yGrid;

	private:
		std::vector<Object> objects;
		std::vector<Object>::iterator selectObject;
		int selectIndex;

	private:
		POINT mousePosition;
		POINT prevMousePosition;
		POINT startMousePosition;
		bool mouseLclick;
		bool mouseRclick;

	private:
		Color color;
		bool isDot;
		bool isBoarder;
		bool isMove;
		bool printInform;
	};

}

