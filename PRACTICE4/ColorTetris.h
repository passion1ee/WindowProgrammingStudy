#pragma once
#include "stdfax.h"
namespace ys
{
	constexpr BYTE kColSize = 10;
	constexpr BYTE kRowSize = 20;
	class ColorTetris
	{
		enum class Block: BYTE
		{
			Square, straight, shapeU, shapeT, shapeZ, shapeS, shapeJ, shapeL
		};
		struct childObj;
		struct Object;

		struct childObj
		{
			POINT position;
			COLORREF color;
			bool isActive = false;
			std::pair<bool, bool> MoveCollide(const bool& isVertical, const bool& isPlus, const Object& world);
			void Move(const bool& isVertical, const bool& isPlus)
			{
				if (isVertical && isPlus && position.y < kRowSize - 1)
					position.y++;
				else if (!isVertical && isPlus && position.x < kColSize - 1)
					position.x++;
				else if (!isVertical && !isPlus && position.x > 0)
					position.x--;
			}

			childObj(const POINT& position, const COLORREF& color)
				: position(position), color(color)
			{
			}
		};
		struct Object
		{
			RECT size;
			std::vector<std::vector<childObj>> childObjs;

			void SetBlock(const Block& block, const std::vector<COLORREF>& colors, 
				const size_t& maxIndex, const bool& isSameColor);

			bool Move(const bool& isVertical, const bool& isPlus, const Object& world);
			void Rotate(const Object& world);

			Object(const RECT& size) : size(size)
			{
				for (int j = size.top; j < size.bottom; ++j)
				{
					childObjs.push_back(std::vector<childObj>());
					for (int i = size.left; i < size.right; ++i)
						childObjs[j].push_back(childObj(POINT(i, j), RGB(0, 0, 0)));
				}
			}
			Object() = default;
		};

	public:
		void Init(HWND hWnd_, RECT& screenSize);
		void Run();
		void command(const WORD& command);

	private:
		void Update();
		void Render();
		void renderFrame();
		void setScreen(RECT& screenSize);
		std::map <std::string , POINT > searchSameColor(std::map<std::string, POINT>& search, const POINT& child);
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

		unsigned long long score;

	private:
		std::vector<COLORREF> colors;
		Object world;
		Object* activeObj;
		Object* nextObj;
		Object* keepObj;

	private:
		bool isRun;
		bool isSameColor;
		bool isKeep;
		float blockDownTime;
		float curTime;
		size_t blockMaxIndex;
		size_t colorMaxIndex;

	};
}

