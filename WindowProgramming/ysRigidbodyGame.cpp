#include "ysRigidbodyGame.h"
#include "stdfax.h"

constexpr BYTE row = 40;
constexpr BYTE col = 40;
constexpr BYTE effectObjMax = 20;

namespace ys
{
	int RigidbodyGame::screenWidth;
	int RigidbodyGame::screenHeight;
	Object RigidbodyGame::player;
	POINT RigidbodyGame::playerPrevPosition;
	Object RigidbodyGame::goal;
	
	std::uniform_int_distribution<> ObjectShape(0, static_cast<int>(Shape::kCount) - 1);
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
		std::uniform_int_distribution<> positionX(0, row - 1);
		std::uniform_int_distribution<> positionY(0, col - 1);
		std::uniform_int_distribution<> ColorRGB(0, 2);
		std::uniform_int_distribution<> effectObj(0, 3);
		std::uniform_int_distribution<> goalSize(5, 10);
		
		plain.clear();
		colors.clear();
		plain.assign(row, std::vector<Object>(col, Object()));
		//frame
		for (int i = 0; i < row; ++i)
			for (int j = 0; j < col; ++j)
			{
				plain[i][j].position.x = j;
				plain[i][j].position.y = i;
			}
		//colorObj
		for (int count = 0; count < row * col / 10;)
		{
			auto x = positionX(randomEngine); auto y = positionY(randomEngine);
			if (plain[x][y].shape != Shape::kEmpty || (x == col - 1 && y == row - 1) || (x == 0 && y == 0))
				continue;

			BYTE RGBVal[3];
			for (int i = 0; i < 3; ++i)
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

			plain[x][y].shape = Shape::kStar;
			colors.push_back(RGB(RGBVal[0], RGBVal[1], RGBVal[2]));
			plain[x][y].color = colors.back();
			++count;
		}
		//effectiveObject
		for (int count = 0; count < row * col / 10;)
		{
			auto x = positionX(randomEngine); auto y = positionY(randomEngine);
			if (plain[x][y].shape != Shape::kEmpty || (x == col - 1 && y == row - 1) || (x == 0 && y == 0))//빈 좌표
				continue;

			for (int i = 0; i < effectObjMax; ++i)
				switch (effectObj(randomEngine)) {
				case 0://장애물
				{
					plain[x][y].color = RGB(255, 0, 0);
					break;
				}
				case 1://축소
				{
					plain[x][y].color = RGB(125, 0, 255);
					break;
				}
				case 2://확대
				{
					plain[x][y].color = RGB(255, 255, 0);
					break;
				}
				case 3://모양변경
				{
					plain[x][y].color = RGB(0, 0, 255);
					break;
				}
				default:
					break;
				}

			plain[x][y].shape = Shape::kRect;
			++count;
		}
		//player
		std::uniform_int_distribution<> setIndex(0, colors.size() - 1);
		player.size = 10;
		player.position = { 0, 0 };
		player.shape = static_cast<Shape>(ObjectShape(randomEngine));
		player.color = colors[setIndex(randomEngine)];
		//goal
		goal.size = goalSize(randomEngine);
		goal.shape = static_cast<Shape>(ObjectShape(randomEngine));
		goal.color = colors[setIndex(randomEngine)];
		goal.position = { col - 1, row - 1 };

	}

	void RigidbodyGame::Run(HWND hWnd)
	{
		ys::InputManager::BeforeUpdate();
		Update(hWnd);
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

	void RigidbodyGame::Update(HWND hWnd)
	{
		bool isKeyDown{ false };
		playerPrevPosition = player.position;
		if (ys::InputManager::getKeyDown((UINT)Key::W))
		{
			if (player.position.y == 0)
				player.position.y = 39;
			else
				player.position.y--;
			isKeyDown = true;
		}
		if (ys::InputManager::getKeyDown((UINT)Key::A))
		{
			if (player.position.x == 0)
				player.position.x = 39;
			else
				player.position.x--;
			isKeyDown = true;
		}
		if (ys::InputManager::getKeyDown((UINT)Key::S))
		{
			if (player.position.y == 39)
				player.position.y = 0;
			else
				player.position.y++;
			isKeyDown = true;
		}
		if (ys::InputManager::getKeyDown((UINT)Key::D))
		{
			if (player.position.x == 39)
				player.position.x = 0;
			else
				player.position.x++;
			isKeyDown = true;
		}

		if (ys::InputManager::getKeyUp((UINT)Key::R))
		{
			Init();
		}
		if (ys::InputManager::getKeyUp((UINT)ys::Key::Q))
		{
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}

		if (isKeyDown)
		{
			
			collide(hWnd);
			isKeyDown = false;
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
		auto realXGrid = xGrid * 0.1 * object.size;
		auto realYGrid = yGrid * 0.1 * object.size;
		auto realPositionX = object.position.x * xGrid + (xGrid - realXGrid) / 2.0;
		auto realPositionY = object.position.y * yGrid + (yGrid - realYGrid) / 2.0;
		switch (object.shape) {
		case ys::Shape::kTriangle:
		{
			POINT vertex[3] = { {static_cast<LONG>(realPositionX + realXGrid / 2.0), realPositionY},
								{realPositionX, realPositionY + realYGrid},
								{realPositionX + realXGrid, realPositionY + realYGrid}
			};

			Polygon(hDC, vertex, 3);
			break;
		}
		case ys::Shape::kHourglass:
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
		case ys::Shape::kPentagon:
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
		case ys::Shape::kPie:
		{
			Pie(hDC, realPositionX, realPositionY, realPositionX + realXGrid, realPositionY + realYGrid,
				static_cast<LONG>(realPositionX + realXGrid / 2.0), realPositionY, realPositionX + realXGrid, static_cast<LONG>(realPositionY + realYGrid / 2.0));
			break;
		}
		case ys::Shape::kRect:
		{
			Rectangle(hDC, realPositionX, realPositionY, realPositionX + realXGrid, realPositionY + realYGrid);
			break;
		}
		case ys::Shape::kStar:
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
	void RigidbodyGame::collide(HWND hWnd)
	{
		if (plain[player.position.y][player.position.x].color == RGB(255, 0, 0))
		{//장애물
			player.position = playerPrevPosition;
		}
		if (plain[player.position.y][player.position.x].color == RGB(125, 0, 255))
		{//축소
			if(player.size > 5)
				player.size--;
		}
		if (plain[player.position.y][player.position.x].color == RGB(255, 255, 0))
		{//확대
			if (player.size < 10)
				player.size++;
		}
		if (plain[player.position.y][player.position.x].color == RGB(0, 0, 255))
		{//모양변경
			player.shape = static_cast<Shape>(ObjectShape(randomEngine));
		}
		if (colors.end() != std::find(colors.begin(), colors.end(), plain[player.position.y][player.position.x].color))
		{//색변경
			player.color = plain[player.position.y][player.position.x].color;
		}
		if (player.position.x == goal.position.x && player.position.y == goal.position.y)
		{//골 체크
			bool isSame{true};
			if (player.size != goal.size)
				isSame = false;
			if (player.shape != goal.shape)
				isSame = false;
			if (player.color != goal.color)
				isSame = false;
			if (isSame)
			{
				victorySound();
				MessageBox(hWnd, L"VICTORY!", L"", NULL);
			}
			else
			{
				defeatSound();
				MessageBox(hWnd, L"DEFEAT!", L"", NULL);
			}
			Init();
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}
	void RigidbodyGame::victorySound()
	{
		Beep(130, 100);
		Beep(262, 100);
		Beep(330, 100);
		Beep(392, 100);
		Beep(523, 100);
		Beep(660, 100);
		Beep(784, 300);
		Beep(660, 300);
		Beep(146, 100);
		Beep(262, 100);
		Beep(311, 100);
		Beep(415, 100);
		Beep(523, 100);
		Beep(622, 100);
		Beep(831, 300);
		Beep(622, 300);
		Beep(155, 100);
		Beep(294, 100);
		Beep(349, 100);
		Beep(466, 100);
		Beep(588, 100);
		Beep(699, 100);
		Beep(933, 300);
		Beep(933, 100);
		Beep(933, 100);
		Beep(933, 100);
		Beep(1047, 400);
	}
	void RigidbodyGame::defeatSound()
	{
		Beep(330, 200);
		Beep(294, 200);
		Beep(262, 200);
		Beep(220, 200);
		Beep(196, 200);
		Beep(175, 200);
		Beep(165, 200);
	}
}