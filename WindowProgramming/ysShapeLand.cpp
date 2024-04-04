#include "ysShapeLand.h"
#include "stdfax.h"

constexpr BYTE kObjMax = 10;

namespace ys
{
	int ShapeLand::screenWidth;
	int ShapeLand::screenHeight;
	BYTE ShapeLand::mapSize;

	double ShapeLand::xGrid;
	double ShapeLand::yGrid;

	size_t ShapeLand::selected;
	std::vector<ShapeLand::Object> ShapeLand::objects;
	std::vector<ShapeLand::Object> ShapeLand::prevObjects;
	std::vector<COLORREF> ShapeLand::colors;

	std::uniform_int_distribution<> ObjectSameShape(static_cast<int>(ShapeLand::Shape::kCount) + 1, static_cast<int>(ShapeLand::Shape::kEmpty) - 1);
	std::uniform_int_distribution<> colorsIndex(0, kObjMax / 3 - 1);

	void ShapeLand::setScreen(int width, int height)
	{
		screenWidth = width;
		screenHeight = height;
		xGrid = screenWidth / static_cast<double>(mapSize);
		yGrid = screenHeight / static_cast<double>(mapSize);
	}

	void ShapeLand::Init()
	{
		mapSize = 40;

		selected = 0;
		objects.reserve(10);
		colors.reserve(kObjMax / 3);
		for (int i = 0; i < kObjMax / 3; ++i)
			colors.push_back(RGB(ObjectColor(randomEngine), ObjectColor(randomEngine), ObjectColor(randomEngine)));
	}

	void ShapeLand::Run(HWND hWnd)
	{
		InputManager::BeforeUpdate();
		Update(hWnd);
		InputManager::AfterUpdate();
	}

	void ShapeLand::render(HDC hDC)
	{
		renderFrame(hDC);
		if (selected > 0)
		{
			for (const auto& object : objects)
			{
				if (object == objects[selected - 1])
					continue;
				auto brush = CreateSolidBrush(object.color);
				auto oldBrush = SelectObject(hDC, brush);

				renderObject(hDC, object);

				SelectObject(hDC, oldBrush);
				DeleteObject(brush);
			}

			auto highlightPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
			auto playerBrush = CreateSolidBrush(objects[selected - 1].color);
			auto oldPen = SelectObject(hDC, highlightPen);
			auto oldBrush = SelectObject(hDC, playerBrush);

			renderObject(hDC, objects[selected - 1]);

			SelectObject(hDC, oldPen);
			SelectObject(hDC, oldBrush);
			DeleteObject(highlightPen);
			DeleteObject(playerBrush);

			std::wstring printPosition = L"x: " + std::to_wstring(objects[selected - 1].position.x) + L" y: " + std::to_wstring(objects[selected - 1].position.y);
			TextOut(hDC, screenWidth - 200, screenHeight - 140, printPosition.c_str(), printPosition.size());
		}
		std::wstring printPosition = L"index: " + std::to_wstring(selected) + L" size: " + std::to_wstring(objects.size());
		TextOut(hDC, screenWidth - 200, screenHeight - 100, printPosition.c_str(), printPosition.size());
	}


	void ShapeLand::Update(HWND hWnd)
	{
		if(prevObjects.empty()){
			bool isKeyDown{ false };
			std::uniform_int_distribution<> position(0, mapSize - 1);

			if (InputManager::getKeyDown((UINT)Key::S))
			{
				mapSize = 30;
				setScreen(screenWidth, screenHeight);
				for (auto& object : objects)
				{
					if (object.position.x > mapSize - 1)
						object.position.x = mapSize - 1;
					if (object.position.y > mapSize - 1)
						object.position.y = mapSize - 1;
				}
			}
			if (InputManager::getKeyDown((UINT)Key::M))
			{
				mapSize = 40;
				setScreen(screenWidth, screenHeight);
				for (auto& object : objects)
				{
					if (object.position.x > mapSize - 1)
						object.position.x = mapSize - 1;
					if (object.position.y > mapSize - 1)
						object.position.y = mapSize - 1;
				}
			}
			if (InputManager::getKeyDown((UINT)Key::L))
			{
				mapSize = 50;
				setScreen(screenWidth, screenHeight);
				for (auto& object : objects)
				{
					if (object.position.x > mapSize - 1)
						object.position.x = mapSize - 1;
					if (object.position.y > mapSize - 1)
						object.position.y = mapSize - 1;
				}
			}

			if (InputManager::getKeyDown((UINT)Key::E))
			{
				if (objects.size() >= kObjMax)
					objects.erase(objects.begin());

				objects.push_back(ShapeLand::Object(ShapeLand::Shape::kCircle, colors[colorsIndex(randomEngine)], POINT({ position(randomEngine), position(randomEngine) }), 100));

				selected = objects.size();
			}
			if (InputManager::getKeyDown((UINT)Key::T))
			{
				if (objects.size() >= kObjMax)
					objects.erase(objects.begin());

				objects.push_back(ShapeLand::Object(ShapeLand::Shape::kTriangle, colors[colorsIndex(randomEngine)], POINT({ position(randomEngine), position(randomEngine) }), 100));

				selected = objects.size();
			}
			if (InputManager::getKeyDown((UINT)Key::R))
			{
				if (objects.size() >= kObjMax)
					objects.erase(objects.begin());

				objects.push_back(ShapeLand::Object(ShapeLand::Shape::kRect, colors[colorsIndex(randomEngine)], POINT({ position(randomEngine), position(randomEngine) }), 100));

				selected = objects.size();
			}

			if (selected > 0) {
				if (InputManager::getKeyDown(VK_LEFT))
				{
					if (objects[selected - 1].position.x == 0)
						objects[selected - 1].position.x = mapSize - 1;
					else
						objects[selected - 1].position.x--;
					isKeyDown = true;
				}
				if (InputManager::getKeyDown(VK_RIGHT))
				{
					if (objects[selected - 1].position.x == mapSize - 1)
						objects[selected - 1].position.x = 0;
					else
						objects[selected - 1].position.x++;
					isKeyDown = true;
				}
				if (InputManager::getKeyDown(VK_UP))
				{
					if (objects[selected - 1].position.y == 0)
						objects[selected - 1].position.y = mapSize - 1;
					else
						objects[selected - 1].position.y--;
					isKeyDown = true;
				}
				if (InputManager::getKeyDown(VK_DOWN))
				{
					if (objects[selected - 1].position.y == mapSize - 1)
						objects[selected - 1].position.y = 0;
					else
						objects[selected - 1].position.y++;
					isKeyDown = true;
				}

				if (InputManager::getKeyDown(VK_ADD))
				{
					if (objects[selected - 1].size < 99)
					{
						objects[selected - 1].size += 2;
					}
				}
				if (InputManager::getKeyDown(VK_SUBTRACT))
				{
					if (objects[selected - 1].size > 3)
					{
						objects[selected - 1].size -= 2;
					}
				}
			}

			if (InputManager::getKeyDown((UINT)Key::D))
			{
				if (selected > 0)
				{
					objects.erase(objects.begin() + --selected);
					if (selected == 0)
						selected = objects.size();
				}
			}
			if (InputManager::getKeyDown((UINT)Key::P))
			{
				selected = 0;
				objects.clear();
			}
			if (InputManager::getKeyUp((UINT)Key::Q))
			{
				PostMessage(hWnd, WM_CLOSE, 0, 0);
			}

			if (isKeyDown)
			{
				collide(hWnd);
				isKeyDown = false;
			}
		}
		if (InputManager::getKeyDown((UINT)Key::C))
		{
			Beep(199, 100);
			if(prevObjects.empty()){
				std::vector<Shape> sameColorShape;
				for (int i = 0; i < kObjMax / 3; ++i)
					sameColorShape.push_back(static_cast<Shape>(ObjectSameShape(randomEngine)));

				for (auto& object : objects)
				{
					auto it = std::find(colors.begin(), colors.end(), object.color);
					size_t index = std::distance(colors.begin(), it);
					prevObjects.push_back(object);
					object.shape = sameColorShape[index];
				}
			}
			else
			{
				objects = prevObjects;
				prevObjects.clear();
			}
		}
	}

	void ShapeLand::select(WPARAM w)
	{
		bool isDigit{ true };
		std::wstring input; input = w;
		for (auto& ch : input)
			if (!iswdigit(ch))
				isDigit = false;
		if (isDigit && std::stoi(input) > 0 && std::stoi(input) < objects.size())
				selected = std::stoi(input);
		if (input == L"0" && objects.size() == 10)
			selected = 10;
	}

	void ShapeLand::renderFrame(HDC hDC)
	{
		for (int i = 0; i < mapSize; ++i)
		{
			MoveToEx(hDC, 0, i * yGrid, NULL);
			LineTo(hDC, screenWidth, i * yGrid);
		}
		for (int i = 0; i < mapSize; ++i)
		{
			MoveToEx(hDC, i * xGrid, 0, NULL);
			LineTo(hDC, i * xGrid, screenHeight);
		}
	}

	void ShapeLand::renderObject(HDC hDC, ShapeLand::Object object)
	{
		if (object.shape == ShapeLand::Shape::kEmpty)
			return;
		auto realXGrid = xGrid * 0.01 * object.size;
		auto realYGrid = yGrid * 0.01 * object.size;
		auto realPositionX = object.position.x * xGrid + (xGrid - realXGrid) / 2.0;
		auto realPositionY = object.position.y * yGrid + (yGrid - realYGrid) / 2.0;
		switch (object.shape) {
		case ShapeLand::Shape::kCircle:
		{
			Ellipse(hDC, realPositionX, realPositionY, realPositionX + realXGrid, realPositionY + realYGrid);
			break;
		}
		case ShapeLand::Shape::kTriangle:
		{
			POINT vertex[3] = { {static_cast<LONG>(realPositionX + realXGrid / 2.0), realPositionY},
								{realPositionX, realPositionY + realYGrid},
								{realPositionX + realXGrid, realPositionY + realYGrid}
			};

			Polygon(hDC, vertex, 3);
			break;
		}
		case ShapeLand::Shape::kHourglass:
		{
			POINT vertex[5] = { {realPositionX, realPositionY},
								{realPositionX + realXGrid, realPositionY},
								{static_cast<LONG>(realPositionX + realXGrid / 2.0), static_cast<LONG>(realPositionY + realYGrid * 5.0 / 10.0)},
								{realPositionX, realPositionY + realYGrid},
								{realPositionX + realXGrid, realPositionY + realYGrid}
			};

			Polygon(hDC, vertex, 5);
			break;
		}
		case ShapeLand::Shape::kPentagon:
		{
			POINT vertex[5] = { {static_cast<LONG>(realPositionX + realXGrid / 2.0), realPositionY},
								{realPositionX, static_cast<LONG>(realPositionY + realYGrid * 4.5 / 10.0)},
								{static_cast<LONG>(realPositionX + realXGrid * 3.0 / 10.0), realPositionY + realYGrid},
								{static_cast<LONG>(realPositionX + realXGrid * 7.0 / 10.0), realPositionY + realYGrid},
								{realPositionX + realXGrid, static_cast<LONG>(realPositionY + realYGrid * 4.5 / 10.0)},
			};

			Polygon(hDC, vertex, 5);
			break;
		}
		case ShapeLand::Shape::kPie:
		{
			Pie(hDC, realPositionX, realPositionY, realPositionX + realXGrid, realPositionY + realYGrid,
				static_cast<LONG>(realPositionX + realXGrid / 2.0), realPositionY, realPositionX + realXGrid, static_cast<LONG>(realPositionY + realYGrid / 2.0));
			break;
		}
		case ShapeLand::Shape::kRect:
		{
			Rectangle(hDC, realPositionX, realPositionY, realPositionX + realXGrid, realPositionY + realYGrid);
			break;
		}
		case ShapeLand::Shape::kStar:
		{
			POINT vertex[8] = { {static_cast<LONG>(realPositionX + realXGrid / 2.0), realPositionY},
								{static_cast<LONG>(realPositionX + realXGrid * 4 / 10.0), static_cast<LONG>(realPositionY + realYGrid * 4 / 10.0)},
								{realPositionX, static_cast<LONG>(realPositionY + realYGrid * 5.0 / 10.0)},
								{static_cast<LONG>(realPositionX + realXGrid * 4 / 10.0), static_cast<LONG>(realPositionY + realYGrid * 6 / 10.0)},
								{static_cast<LONG>(realPositionX + realXGrid / 2.0), realPositionY + realYGrid},
								{static_cast<LONG>(realPositionX + realXGrid * 6 / 10.0), static_cast<LONG>(realPositionY + realYGrid * 6 / 10.0)},
								{realPositionX + realXGrid, static_cast<LONG>(realPositionY + realYGrid * 5.0 / 10.0)},
								{static_cast<LONG>(realPositionX + realXGrid * 6 / 10.0), static_cast<LONG>(realPositionY + realYGrid * 4 / 10.0)},
			};

			Polygon(hDC, vertex, 8);
			break;
		}
		default:
			break;
		}
	}
	void ShapeLand::collide(HWND hWnd)
	{
		auto iter = std::find_if(objects.begin(), objects.end(), [](const Object& other) {
			return objects[selected - 1].position.x == other.position.x && 
				objects[selected - 1].position.y == other.position.y &&
				objects[selected - 1] != other;
		});

		if (objects.end() != iter)
		{
			Beep(1760, 40);
			Beep(2218, 80);
			iter->color = objects[selected - 1].color;
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}
}