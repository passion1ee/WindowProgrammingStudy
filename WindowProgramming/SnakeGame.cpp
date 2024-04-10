#include "SnakeGame.h"
#include "stdfax.h"

#include <iostream>

constexpr BYTE kMapSize = 40;

constexpr BYTE kHorizon = 0x1;
constexpr BYTE kVertical = 0x2;
constexpr BYTE kRect = 0x4;
constexpr BYTE kStop = 0x8;

constexpr BYTE kRectLtoR = 0x10;
constexpr BYTE kRectRtoL = 0x20;
constexpr BYTE kRectBtoT = 0x40;
constexpr BYTE kRectTtoB = 0x80;

namespace ys
{
	SnakeGame::SnakeGame()
		: hWnd(nullptr), hDC(nullptr), hBackDC(nullptr), hBitmap(nullptr), screen({ 0, 0 }), xGrid(0), yGrid(0), frameCheck(0), keyCoolTime(0)
	{
		std::uniform_int_distribution<> position(0, kMapSize - 1);
		player = Object(0, 100, { 0, 0 }, RGB(ObjectColor(randomEngine), ObjectColor(randomEngine), ObjectColor(randomEngine)), kHorizon, true);
		for (int i = 0; i < 20;)
		{
			auto x = position(randomEngine); auto y = position(randomEngine);
			if (feeds.end() != std::find(feeds.begin(), feeds.end(), [x, y](Object& comp) {return comp.position.x == x && comp.position.y == y; }) || (x == 0 && y == 0))
				continue;
			feeds.push_back(Object(0, 100, {position(randomEngine), position(randomEngine)},
				RGB(ObjectColor(randomEngine), ObjectColor(randomEngine), ObjectColor(randomEngine)), kStop, true));
			++i;
		}

	}

	void SnakeGame::setScreen(RECT screenSize)
	{
		//::AdjustWindowRect(&screenSize, WS_OVERLAPPEDWINDOW, FALSE);
		screen.x = screenSize.right - screenSize.left; screen.y = screenSize.bottom - screenSize.top;

		SetWindowPos(hWnd, nullptr, 0, 0, screen.x, screen.y, 0);

		hBitmap = CreateCompatibleBitmap(hDC, screen.x, screen.y);
		auto oldBitmap = SelectObject(hBackDC, hBitmap);
		DeleteObject(oldBitmap);
		xGrid = screen.x / static_cast<double>(kMapSize);
		yGrid = screen.y / static_cast<double>(kMapSize);
	}

	void SnakeGame::Init(HWND hWnd_, RECT screenSize)
	{
		hWnd = hWnd_;
		hDC = GetDC(hWnd_);
		hBackDC = CreateCompatibleDC(hDC);

		SnakeGame::setScreen(screenSize);

		keyCoolTime = 0;
		Timer::Init();
	}

	void SnakeGame::Run()
	{
		static bool isRun = false;
		InputManager::BeforeUpdate();
		Timer::Update();
		if (InputManager::getKeyUp((UINT)Key::S) || isRun) {
			if (1 / Timer::getRealFPS() <= (frameCheck += Timer::getDeltaTime())) {
				frameCheck = 0.0f;
				Update();
				Render();
			}
			isRun = true;
		}
		InputManager::AfterUpdate();
	}

	void SnakeGame::Render()
	{
		PatBlt(hBackDC, 0, 0, screen.x, screen.y, WHITENESS);
		
		renderFrame();


		BitBlt(hDC, 0, 0, screen.x, screen.y, hBackDC, 0, 0, SRCCOPY);
	}

	void SnakeGame::Update()
	{
		move(player);
		if (InputManager::getKey(VK_LEFT))
		{
		}
		if (InputManager::getKey(VK_RIGHT))
		{
		}
		if (InputManager::getKey(VK_UP))
		{
		}
		if (InputManager::getKey(VK_DOWN))
		{
		}

		if (ys::InputManager::getKeyUp((UINT)ys::Key::Q))
		{
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
	}

	void SnakeGame::move(Object object)
	{		
		switch (object.moveState)
		{
		case kHorizon:
		{
			if(object.isPlus)
			{
				if(object.position.x == 39)
				{
					object.position.y++;
					object.isPlus = false;
				}
				else
					object.position.x++;
			}
			else
			{
				if (object.position.x == 0)
				{
					object.position.y--;
					object.isPlus = true;
				}
				else
					object.position.x--;
			}
			break;
		}
		case kVertical:
		{
			if (object.isPlus)
			{
				if (object.position.y == 39)
				{
					object.position.x++;
					object.isPlus = false;
				}
				else
					object.position.y++;
			}
			else
			{
				if (object.position.y == 0)
				{
					object.position.x--;
					object.isPlus = true;
				}
				else
					object.position.y--;
			}
			break;
		}
		case kRect:
		{
			if (object.isPlus)
				object.moveState |= kRectLtoR;
			else
				object.moveState |= kRectRtoL;
			break;
		}
		case kStop:
			break;
		default:
			if (object.moveState & (kRect | kRectLtoR))
			{
				object.position.x++;
				object.rectMoveCnt++;
				if (object.rectMoveCnt >= 3)
				{
					object.moveState = (object.moveState & ~kRectLtoR) | kRectTtoB;
					object.rectMoveCnt = 0;
				}
			}
			else if (object.moveState & (kRect | kRectRtoL))
			{
				object.position.x--;
				object.rectMoveCnt++;
				if (object.rectMoveCnt >= 3)
				{
					object.moveState = object.moveState & ~kRectRtoL | kRectBtoT;
					object.rectMoveCnt = 0;
				}
			}
			else if (object.moveState & (kRect | kRectBtoT))
			{
				object.position.y--;
				object.rectMoveCnt++;
				if (object.rectMoveCnt >= 3)
				{
					object.moveState = object.moveState & ~kRectBtoT | kRectLtoR;
					object.rectMoveCnt = 0;
				}
			}
			else if (object.moveState & (kRect | kRectTtoB))
			{
				object.position.y++;
				object.rectMoveCnt++;
				if (object.rectMoveCnt >= 3)
				{
					object.moveState = object.moveState & ~kRectTtoB | kRectRtoL;
					object.rectMoveCnt = 0;
				}
			}
			break;
		}
	}

	void SnakeGame::renderFrame()
	{
		for (int i = 0; i < kMapSize; ++i)
		{
			MoveToEx(hBackDC, 0, i * yGrid, NULL);
			LineTo(hBackDC, screen.x, i * yGrid);
		}
		for (int i = 0; i < kMapSize; ++i)
		{
			MoveToEx(hBackDC, i * xGrid, 0, NULL);
			LineTo(hBackDC, i * xGrid, screen.y);
		}
	}
}


