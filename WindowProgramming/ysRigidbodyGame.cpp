#include "ysRigidbodyGame.h"
#include "stdfax.h"

constexpr BYTE row = 40;
constexpr BYTE col = 40;

namespace ys
{
	int RigidbodyGame::screenWidth;
	int RigidbodyGame::screenHeight;
	Object RigidbodyGame::player;
	Object RigidbodyGame::goal;
	std::vector<std::vector<Object>> RigidbodyGame::plain;
	std::vector<COLORREF> RigidbodyGame::colors;

	double xGrid;
	double yGrid;

	void RigidbodyGame::setScreen(int width, int height)
	{
		screenWidth = width;
		screenHeight = height;
		xGrid = screenWidth / static_cast<double>(col);
		yGrid = screenHeight / static_cast<double>(row);
	}

	void RigidbodyGame::Init()
	{
		std::uniform_int_distribution<> ObjectShape(0, static_cast<int>(Shape::kCount) - 1);
		std::uniform_int_distribution<> positionX(0, row - 1);
		std::uniform_int_distribution<> positionY(0, col - 1);
		std::uniform_int_distribution<> ColorRGB(0, 2);

		
		plain.clear();
		colors.clear();
		plain.assign(row, std::vector<Object>(col, Object()));
		for (int i = 0; i < row; ++i)
			for (int j = 0; j < col; ++j)
			{
				plain[i][j].position.x = j;
				plain[i][j].position.y = i;
			}

		for (int count = 0; count < row * col / 10;)
		{
			auto x = positionX(randomEngine); auto y = positionY(randomEngine);
			if (plain[x][y].shape != Shape::kEmpty || (x == col - 1 && y == row - 1))
				continue;

			BYTE RGBVal[3];
			for (int i = 0; i < 3; ++i)
			{
				switch (ColorRGB(randomEngine)) {
				case 0:
				{
					RGBVal[i] = 50;
					break;
				}
				case 1:
				{
					RGBVal[i] = 125;
					break;
				}
				case 2:
				{
					RGBVal[i] = 200;
					break;
				}
				default:
					break;
				}
			}
			plain[x][y].shape = static_cast<Shape>(ObjectShape(randomEngine));
			colors.push_back(RGB(RGBVal[0], RGBVal[1], RGBVal[2]));
			plain[x][y].color = colors.back();
			++count;
		}

		std::uniform_int_distribution<> setIndex(0, colors.size() - 1);
		player.shape = static_cast<Shape>(ObjectShape(randomEngine));
		player.color = colors[setIndex(randomEngine)];

		goal.shape = static_cast<Shape>(ObjectShape(randomEngine));
		goal.color = colors[setIndex(randomEngine)];
		goal.position = { col - 1, row - 1 };

	}

	void RigidbodyGame::Run()
	{
		ys::InputManager::BeforeUpdate();
		Update();
		ys::InputManager::AfterUpdate();
	}

	void RigidbodyGame::render(HDC hDC)
	{
		renderFrame(hDC);

		for (int i = 0; i < plain.size(); ++i)
			for (int j = 0; j < plain[i].size(); ++j)
			{
				auto brush = CreateSolidBrush(plain[i][j].color);
				auto oldBrush = SelectObject(hDC, brush);
				renderObject(hDC, plain[i][j]);
				SelectObject(hDC, oldBrush);
				DeleteObject(brush);
			}

		auto playerBrush = CreateSolidBrush(player.color);
		auto goalBrush = CreateSolidBrush(goal.color);
		
		auto oldBrush = SelectObject(hDC, playerBrush);
		renderObject(hDC, player);
		
		SelectObject(hDC, goalBrush);
		renderObject(hDC, goal);
		
		SelectObject(hDC, oldBrush);
		DeleteObject(goalBrush);
		DeleteObject(playerBrush);

		std::wstring printPosition = L"x: " + std::to_wstring(player.position.x) + L"   y: " + std::to_wstring(player.position.y);
		TextOut(hDC, screenWidth - 200, screenHeight - 100, printPosition.c_str(), printPosition.size());
	}

	void RigidbodyGame::Update()
	{
		if (ys::InputManager::getKeyDown((UINT)Key::W))
		{
			if (player.position.y == 0)
				player.position.y = 39;
			else
				player.position.y--;

			const auto& tmp = player;
		}
		if (ys::InputManager::getKeyDown((UINT)Key::A))
		{
			if (player.position.x == 0)
				player.position.x = 39;
			else
				player.position.x--;
		}
		if (ys::InputManager::getKeyDown((UINT)Key::S))
		{
			if (player.position.y == 39)
				player.position.y = 0;
			else
				player.position.y++;
		}
		if (ys::InputManager::getKeyDown((UINT)Key::D))
		{
			if (player.position.x == 39)
				player.position.x = 0;
			else
				player.position.x++;
		}
		if (ys::InputManager::getKeyUp((UINT)Key::R))
		{
			Init();
		}

		if (colors.end() != std::find(colors.begin(), colors.end(), plain[player.position.y][player.position.x].color))
		{
			player.color = plain[player.position.y][player.position.x].color;
		}

	}

	void RigidbodyGame::renderFrame(HDC hDC)
	{
		for (int i = 0; i < row; ++i)
		{
			MoveToEx(hDC, plain[i][0].position.x * xGrid, plain[i][0].position.y * yGrid, NULL);
			LineTo(hDC, screenWidth, plain[i][0].position.y * yGrid);
		}
		for (int i = 0; i < col; ++i)
		{
			MoveToEx(hDC, plain[0][i].position.x * xGrid, plain[0][i].position.y * yGrid, NULL);
			LineTo(hDC, plain[0][i].position.x * xGrid, screenHeight);
		}
	}

	void RigidbodyGame::renderObject(HDC hDC, Object object)
	{
		auto realPositionX = object.position.x * xGrid;
		auto realPositionY = object.position.y * yGrid;
		switch (object.shape) {
		case ys::Shape::kTriangle:
		{
			POINT vertex[3] = { {static_cast<LONG>(realPositionX + xGrid / 2.0), realPositionY},
								{realPositionX, realPositionY + yGrid},
								{realPositionX + xGrid, realPositionY + yGrid}
			};

			Polygon(hDC, vertex, 3);
			break;
		}
		case ys::Shape::kHourglass:
		{
			POINT vertex[5] = { {realPositionX, realPositionY},
								{realPositionX + xGrid, realPositionY},
								{static_cast<LONG>(realPositionX + xGrid / 2.0), static_cast<LONG>(realPositionY + yGrid * 5.0 / 10.0)},
								{realPositionX, realPositionY + yGrid},
								{realPositionX + xGrid, realPositionY + yGrid}
			};

			Polygon(hDC, vertex, 5);
			break;
		}
		case ys::Shape::kPentagon:
		{
			POINT vertex[5] = { {static_cast<LONG>(realPositionX + xGrid / 2.0), realPositionY},
								{realPositionX, static_cast<LONG>(realPositionY + yGrid * 4.5 / 10.0)},
								{static_cast<LONG>(realPositionX + xGrid * 3.0 / 10.0), realPositionY + yGrid},
								{static_cast<LONG>(realPositionX + xGrid * 7.0 / 10.0), realPositionY + yGrid},
								{realPositionX + xGrid, static_cast<LONG>(realPositionY + yGrid * 4.5 / 10.0)},
			};

			Polygon(hDC, vertex, 5);
			break;
		}
		case ys::Shape::kPie:
		{
			Pie(hDC, realPositionX, realPositionY, realPositionX + xGrid, realPositionY + yGrid,
				static_cast<LONG>(realPositionX + xGrid / 2.0), realPositionY, realPositionX + xGrid, static_cast<LONG>(realPositionY + yGrid / 2.0));
			break;
		}
		default:
			break;
		}
	}
}