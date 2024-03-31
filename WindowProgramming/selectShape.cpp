#include "selectShape.h"
#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#include <random>

namespace ys
{
	std::vector<ys::Shape> selectShape::shapes;
	int selectShape::screenWidth;
	int selectShape::screenHeight;
	RECT selectShape::mainRect;
	std::vector<RECT> selectShape::selectRect;
	Shape selectShape::selected;

	std::random_device rd;
	std::mt19937 re(rd());
	std::uniform_int_distribution<> select(0, 3);
	std::uniform_int_distribution<> color(0, 255);

	void selectShape::setScreen(int width, int height)
	{
		screenWidth = width;
		screenHeight = height;
		mainRect = RECT(static_cast<LONG>(screenWidth / 3.0), static_cast<LONG>(screenHeight / 3.0), static_cast<LONG>(screenWidth * 2.0 / 3.0), static_cast<LONG>(screenHeight * 2.0 / 3.0));  //(1, 1), (2, 2)
		
		selectRect.clear();
		selectRect.push_back(RECT(mainRect.left, 0, mainRect.right, mainRect.top));  //(1, 0), (2, 1)
		selectRect.push_back(RECT(0, mainRect.top, mainRect.left, mainRect.bottom));  //(0, 1), (1, 2)
		selectRect.push_back(RECT(mainRect.left, mainRect.bottom, mainRect.right, screenHeight));  //(1, 2), (2, 3)
		selectRect.push_back(RECT(mainRect.right, mainRect.top, screenWidth, mainRect.bottom));  //(2, 1), (3, 2)
	}

	void selectShape::render(HDC hDC)
	{
		Rectangle(hDC, mainRect.left, mainRect.top, mainRect.right, mainRect.bottom);
		for (int index = 0; index < shapes.size(); ++index)
		{
			auto brush = CreateSolidBrush(shapes[index].color);
			auto oldBrush = SelectObject(hDC, brush);
			
			renderShape(hDC, shapes[index], selectRect[index]);

			SelectObject(hDC, oldBrush);
			DeleteObject(brush);
		}

		auto brush = CreateSolidBrush(selected.color);
		auto oldBrush = SelectObject(hDC, brush);

		renderShape(hDC, selected, mainRect);

		SelectObject(hDC, oldBrush);
		DeleteObject(brush);
	}

	void selectShape::Run()
	{
		InputManager::BeforeUpdate();
		Update();
		InputManager::AfterUpdate();
	}

	void selectShape::Init()
	{
		shapes.push_back({ static_cast<Shapes>(Shapes::kTriangle), 0 });
		shapes.push_back({ static_cast<Shapes>(Shapes::kHourglass), 0 });
		shapes.push_back({ static_cast<Shapes>(Shapes::kPentagon), 0 });
		shapes.push_back({ static_cast<Shapes>(Shapes::kPie), 0 });
		std::shuffle(shapes.begin(), shapes.end(), re);
	}

	void selectShape::Update()
	{
		if (ys::InputManager::getKeyDown((UINT)Key::W))
		{
			shapes[0].color = RGB(color(re), color(re), color(re));
			selected = shapes[0];
		}
		if (ys::InputManager::getKeyDown((UINT)Key::A))
		{
			shapes[1].color = RGB(color(re), color(re), color(re));
			selected = shapes[1];
		}
		if (ys::InputManager::getKeyDown((UINT)Key::S))
		{
			shapes[2].color = RGB(color(re), color(re), color(re));
			selected = shapes[2];
		}
		if (ys::InputManager::getKeyDown((UINT)Key::D))
		{
			shapes[3].color = RGB(color(re), color(re), color(re));
			selected = shapes[3];
		}

		if (ys::InputManager::getKeyUp((UINT)Key::W))
		{
			shapes[0].color = 0;
			selected = shapes[0];
		}
		if (ys::InputManager::getKeyUp((UINT)Key::A))
		{
			shapes[1].color = 0;
			selected = shapes[0];
		}
		if (ys::InputManager::getKeyUp((UINT)Key::S))
		{
			shapes[2].color = 0;
			selected = shapes[0];
		}
		if (ys::InputManager::getKeyUp((UINT)Key::D))
		{
			shapes[3].color = 0;
			selected = shapes[0];
		}

		if (ys::InputManager::getKeyDown(VK_LEFT))
		{
			std::rotate(shapes.begin(), shapes.begin() + 1, shapes.end());
			selected = shapes[0];
		}
		if (ys::InputManager::getKeyDown(VK_RIGHT))
		{
			std::rotate(shapes.begin(), shapes.end() - 1, shapes.end());
			selected = shapes[0];
		}
	}

	void selectShape::renderShape(HDC hDC, Shape shape, RECT rect)
	{
		auto width = rect.right - rect.left;
		auto height = rect.bottom - rect.top;
		switch (shape.shape) {
		case ys::Shapes::kTriangle:
		{
			POINT vertex[3] = { {rect.left + width / 2.0, rect.top + height / 10.0},
								{rect.left + width / 10.0, rect.top + height * 9.0 / 10.0},
								{rect.left + width * 9.0 / 10.0, rect.top + height * 9.0 / 10.0}
			};

			Polygon(hDC, vertex, 3);
			break;
		}
		case ys::Shapes::kHourglass:
		{
			POINT vertex[5] = {	{rect.left + width / 10.0, rect.top + height / 10.0},
								{rect.left + width * 9.0 / 10.0, rect.top + height / 10.0},
								{rect.left + width / 2.0, rect.top + height * 5.0 / 10.0},
								{rect.left + width / 10.0, rect.top + height * 9.0 / 10.0},
								{rect.left + width * 9.0 / 10.0, rect.top + height * 9.0 / 10.0}
			};

			Polygon(hDC, vertex, 5);
			break;
		}
		case ys::Shapes::kPentagon:
		{
			POINT vertex[5] = {	{rect.left + width / 2.0, rect.top + height / 10.0},
								{rect.left + width / 10.0, rect.top + height * 4.5 / 10.0},
								{rect.left + width * 3.0 / 10.0, rect.top + height * 9.0 / 10.0},
								{rect.left + width * 7.0 / 10.0, rect.top + height * 9.0 / 10.0},
								{rect.left + width * 9.0 / 10.0, rect.top + height * 4.5 / 10.0},
			};

			Polygon(hDC, vertex, 5);
			break;
		}
		case ys::Shapes::kPie:
		{
			Pie(hDC, rect.left + width / 10.0, rect.top + height / 10.0, rect.left + width * 9.0 / 10.0, rect.top + height * 9.0 / 10.0,
				rect.left + width / 2.0, rect.top + height / 10.0, rect.left + width * 9.0 / 10.0, rect.top + height / 2.0);
			break;
		}
		default:
			break;
		}
	}
}