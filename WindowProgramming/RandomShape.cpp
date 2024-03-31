#ifdef PRACTICE__2_6
#include "RandomShape.h"

#include <random>
#include <cmath>

namespace ys
{
	std::vector<ys::Section> RandomShape::sections;
	int RandomShape::screenWidth;
	int RandomShape::screenHeight;
	std::random_device rd;
	std::mt19937 re(rd());
	std::uniform_int_distribution<int> randShape(0, (int)Shape::kCount - 1);
	std::uniform_int_distribution<> color(0, 255);
	std::uniform_int_distribution<> randSymbol(0, 5);

	void RandomShape::setScreen(int width, int height)
	{
		screenWidth = width;
		screenHeight = height;
	}

	void RandomShape::setSection(int row, int col)
	{
		sections.clear();
		sections.assign(row * col, Section());

		for (int i = 0; i < row; ++i)
			for (int j = 0; j < col; ++j)
			{
				sections[(i * col) + j].rect.left = screenWidth / col * j;
				sections[(i * col) + j].rect.top = screenHeight / row * i;
				sections[(i * col) + j].rect.right = screenWidth / col * (j + 1);
				sections[(i * col) + j].rect.bottom = screenHeight / row * (i + 1);

				sections[(i * col) + j].shape = static_cast<Shape>(randShape(re));
			}
	}

	void RandomShape::render(HDC hDC)
	{
		SetBkColor(hDC, RGB(color(re), color(re), color(re)));
		SetTextColor(hDC, RGB(color(re), color(re), color(re)));
		renderFrame(hDC);

		SetBkColor(hDC, RGB(color(re), color(re), color(re)));
		SetTextColor(hDC, RGB(color(re), color(re), color(re)));
		renderShape(hDC);
		//DrawText(hDC, line.c_str(), line.size(), &shapes[i].rect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);

	}

	void RandomShape::renderFrame(HDC hDC)
	{
		std::wstring frameX{};
		std::wstring frameY{};
		SIZE pieceX; GetTextExtentPoint32(hDC, L"¦¡", 1, &pieceX);
		SIZE pieceY; GetTextExtentPoint32(hDC, L"¦¢", 1, &pieceY);
		
		frameX += L"¦«";
		for (long i = 0; i < ((double)sections[0].rect.right / pieceX.cx) - 1; ++i)
			frameX += L"¦¡";

		for (int i = 0; i < sections.size(); ++i)
			for (int j = 0; j < (double)sections[0].rect.bottom / pieceX.cy; ++j)
				TextOut(hDC, sections[i].rect.right, sections[i].rect.top + j * pieceY.cy, L"¦¢", 1);

		for (int i = 0; i < sections.size(); ++i)
			TextOut(hDC, sections[i].rect.left, sections[i].rect.bottom, frameX.c_str(), frameX.size());
	}

	void RandomShape::renderShape(HDC hDC)
	{
		std::wstring pixel = L"#*+~^=";
		for(int index = 0; index < sections.size(); ++index)
		{
			auto& cur = sections[index].rect;
			auto width = cur.right - cur.left;
			auto height = cur.bottom - cur.top;
			auto selectPixel = &pixel[randSymbol(re)];
			SIZE pieceStar; GetTextExtentPoint32(hDC, selectPixel, 1, &pieceStar);
			//pieceStar.cx = 1;
			//pieceStar.cy = 1;
			switch (sections[index].shape) {
			case ys::Shape::kX:
			{
				auto slope = (double)(cur.bottom - cur.top) / (cur.right - cur.left);
				for (double x = cur.left + pieceStar.cx * 4; x < cur.right - pieceStar.cx * 4; x += pieceStar.cx)
				{
					TextOut(hDC, x, slope *(x - cur.left) + cur.top, selectPixel, 1);
					TextOut(hDC, x, -slope * (x - cur.left) + cur.bottom, selectPixel, 1);
					//Sleep(1);
				}
				break;
			}
			case ys::Shape::kSnake:
			{
				
				for (double x = cur.left + width * 2 / 10; x < cur.left + width * 8 / 10; x += pieceStar.cx)
				{ 
					TextOut(hDC, x, cur.top + height * 2 / 10, selectPixel, 1);
					TextOut(hDC, x, cur.top + height * 5 / 10, selectPixel, 1);
					TextOut(hDC, x, cur.top + height * 8 / 10, selectPixel, 1);
					//Sleep(1);
				}
				//for (double y = cur.top + height * 5 / 10; y < cur.top + height * 8 / 10; y += pieceStar.cy)
				//{
				//	TextOut(hDC, cur.left + pieceStar.cx * 4, y, selectPixel, 1);
				//	TextOut(hDC, cur.left + pieceStar.cx * 5, y, selectPixel, 1);
				//	//Sleep(1);
				//}
				//for (double y = cur.top + height * 2 / 10; y < cur.top + height * 5 / 10; y += pieceStar.cy)
				//{
				//	TextOut(hDC, cur.right - pieceStar.cx * 5, y, selectPixel, 1);
				//	TextOut(hDC, cur.right - pieceStar.cx * 6, y, selectPixel, 1);
				//	//Sleep(1);
				//}
				for (double y = cur.top + height * 2 / 10; y < cur.top + height * 5 / 10; y += pieceStar.cy)
				{
					TextOut(hDC, cur.left + width * 8 / 10, y, selectPixel, 1);
					//Sleep(1);
				}
				for (double y = cur.top + height * 5 / 10; y < cur.top + height * 8 / 10; y += pieceStar.cy)
				{
					TextOut(hDC, cur.left + width * 2 / 10, y, selectPixel, 1);
					//Sleep(1);
				}
				break;
			}
			case ys::Shape::kDiamond:
			{
				auto slope = (double)(cur.bottom - cur.top) / (cur.right - cur.left);
				for (double x = cur.left + width / 2; x < cur.right - pieceStar.cx * 2; x += pieceStar.cx)
				{
					TextOut(hDC, x - (width - pieceStar.cx * 4) / 2, slope * (x - cur.left) + cur.top, selectPixel, 1);
					TextOut(hDC, x - (width - pieceStar.cx * 4) / 2, -slope * (x - cur.left) + cur.bottom, selectPixel, 1);
					//Sleep(1);
				}
				for (double x = cur.left + pieceStar.cx * 2; x < cur.right - width / 2; x += pieceStar.cx)
				{
					TextOut(hDC, x + (width - pieceStar.cx * 4) / 2, slope * (x - cur.left) + cur.top, selectPixel, 1);
					TextOut(hDC, x + (width - pieceStar.cx * 4) / 2, -slope * (x - cur.left) + cur.bottom, selectPixel, 1);
					//Sleep(1);
				}
				break;
			}
			case ys::Shape::kButterfly:
			{
				auto slope = (double)(cur.bottom - cur.top) / (cur.right - cur.left);
				for (double x = cur.left + pieceStar.cx * 6; x < cur.right - pieceStar.cx * 6; x += pieceStar.cx)
				{
					TextOut(hDC, x, slope * (x - cur.left) + cur.top, selectPixel, 1);
					TextOut(hDC, x, -slope * (x - cur.left) + cur.bottom, selectPixel, 1);
					//Sleep(1);
				}
				//for (double y = cur.top + pieceStar.cy * 2; y < cur.bottom - pieceStar.cy * 2; y += pieceStar.cy)
				//{
				//	TextOut(hDC, cur.left + pieceStar.cx * 4, y, selectPixel, 1);
				//	TextOut(hDC, cur.right - pieceStar.cx * 5, y, selectPixel, 1);
				//	//Sleep(1);
				//}
				for (double y = slope * (cur.left + pieceStar.cx * 6 - cur.left) + cur.top; y < slope * (cur.right - pieceStar.cx * 6 - cur.left) + cur.top; y += pieceStar.cy)
				{
					TextOut(hDC, cur.left + pieceStar.cx * 6, y, selectPixel, 1);
					//TextOut(hDC, cur.left + pieceStar.cx * 2 + pieceStar.cx, y, selectPixel, 1);
					TextOut(hDC, cur.right - pieceStar.cx * 6, y, selectPixel, 1);
					//TextOut(hDC, cur.right - pieceStar.cx * 2 - pieceStar.cx, y, selectPixel, 1);
					//Sleep(1);
				}
				break;
			}
			case ys::Shape::kTriangle2:
			{
				auto slope = (double)(cur.bottom - cur.top) / (cur.right - cur.left);
				for (double x = cur.left + width / 2; x < cur.right - pieceStar.cx * 2; x += pieceStar.cx)
				{
					TextOut(hDC, x - (width - pieceStar.cx * 4) / 2, -slope * (x - cur.left) + cur.bottom, selectPixel, 1);
					TextOut(hDC, x - (width - pieceStar.cx * 4) / 2, cur.top + height / 2, selectPixel, 1);
					TextOut(hDC, x - (width - pieceStar.cx * 4) / 2, -slope * (x - cur.left) + cur.bottom + height / 2 - pieceStar.cy, selectPixel, 1);
					TextOut(hDC, x - (width - pieceStar.cx * 4) / 2, cur.top + height - pieceStar.cy, selectPixel, 1);
					//Sleep(1);
				}
				for (double x = cur.left + pieceStar.cx * 2; x < cur.right - width / 2; x += pieceStar.cx)
				{
					TextOut(hDC, x + (width - pieceStar.cx * 4) / 2, slope * (x - cur.left) + cur.top, selectPixel, 1);
					TextOut(hDC, x + (width - pieceStar.cx * 4) / 2, cur.top + height / 2, selectPixel, 1);
					TextOut(hDC, x + (width - pieceStar.cx * 4) / 2, slope * (x - cur.left) + cur.top + height / 2 - pieceStar.cy, selectPixel, 1);
					TextOut(hDC, x + (width - pieceStar.cx * 4) / 2, cur.top + height - pieceStar.cy, selectPixel, 1);
					//Sleep(1);
				}
				break;
			}
			case ys::Shape::kRect3:
			{
				for (double y = cur.top + height * 2 / 10; y < cur.top + height * 8 / 10; y += pieceStar.cy)
				{
					TextOut(hDC, cur.left + width * 1 / 10, y, selectPixel, 1);
					TextOut(hDC, cur.left + width * 3 / 10, y, selectPixel, 1);
					TextOut(hDC, cur.left + width * 4 / 10, y, selectPixel, 1);
					TextOut(hDC, cur.left + width * 6 / 10, y, selectPixel, 1);
					TextOut(hDC, cur.left + width * 7 / 10, y, selectPixel, 1);
					TextOut(hDC, cur.left + width * 9 / 10, y, selectPixel, 1);
					//Sleep(1);
				}
				for (double x = cur.left + width * 1 / 10; x < cur.left + width * 3 / 10; x += pieceStar.cx)
				{
					TextOut(hDC, x, cur.top + height * 2 / 10, selectPixel, 1);
					TextOut(hDC, x, cur.top + height * 8 / 10, selectPixel, 1);
					//Sleep(1);
				}
				for (double x = cur.left + width * 4 / 10; x < cur.left + width * 6 / 10; x += pieceStar.cx)
				{
					TextOut(hDC, x, cur.top + height * 2 / 10, selectPixel, 1);
					TextOut(hDC, x, cur.top + height * 8 / 10, selectPixel, 1);
					//Sleep(1);
				}
				for (double x = cur.left + width * 7 / 10; x < cur.left + width * 9 / 10; x += pieceStar.cx)
				{
					TextOut(hDC, x, cur.top + height * 2 / 10, selectPixel, 1);
					TextOut(hDC, x, cur.top + height * 8 / 10, selectPixel, 1);
					//Sleep(1);
				}
				break;
			}
			default:
				break;
			}
		}
		//DrawText(hDC, line.c_str(), line.size(), &shapes[i].rect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	}
}

#endif PRACTICE__2_6