#include "BrickOut.h"
#include "stdfax.h"

namespace ys
{
	void BrickOut::setScreen(RECT& screenSize)
	{
		::AdjustWindowRect(&screenSize, WS_OVERLAPPEDWINDOW, FALSE);
		screen = screenSize;

		SetWindowPos(hWnd, nullptr, 0, 0, screen.right - screen.left, screen.bottom - screen.top, 0);

		hBitmap = CreateCompatibleBitmap(hDC, screen.right - screen.left, screen.bottom - screen.top);
		auto oldBitmap = SelectObject(hBackDC, hBitmap);
		DeleteObject(oldBitmap);
	}

	void BrickOut::Init(HWND hWnd_, RECT& screenSize)
	{
		hWnd = hWnd_;
		hDC = GetDC(hWnd_);
		hBackDC = CreateCompatibleDC(hDC);

		BrickOut::setScreen(screenSize);
		Bricks.Init(60, screen);
		player.Init(fVector((screen.right - screen.left) / 2.0, screen.top));
		Timer::Init();
	}

	void BrickOut::Run()
	{
		if (InputManager::getKeyUp((UINT)Key::S))
			isRun = true;
		if (!isRun) return;

		if (InputManager::getKeyUp((UINT)Key::P) || !isStop)
		{
			if (InputManager::getKeyUp((UINT)Key::P))
			{
				isStop = isStop ? false : true;
			}
			Timer::Update();
			if (1 / Timer::getRealFPS() <= (frameCheck += Timer::getDeltaTime()))
			{
				InputManager::BeforeUpdate();
				frameCheck -= 1 / Timer::getRealFPS();
				Update();
				Render();
				Timer::Render(hDC, POINT(screen.right - screen.left, screen.bottom - screen.top));
				InputManager::AfterUpdate();
			}
		}
		else if (isStop)
		{
			std::wstring line;
			line += L"색이 변한 벽돌의 개수: " + std::to_wstring(Bricks.CountInAct().first) + L", 없어진 개수: " + std::to_wstring(Bricks.CountInAct().second);
			SIZE size;  GetTextExtentPoint32(hDC, line.c_str(), line.size(), &size);
			TextOut(hBackDC, (screen.right - screen.left) / 2 - size.cx, (screen.bottom - screen.top) / 2, line.c_str(), line.size());
		}
	}

	void BrickOut::Update()
	{
		Bricks.Update();
		player.Update(screen);

		if (ys::InputManager::getKeyUp((UINT)ys::Key::N))
		{
			Init(hWnd, screen);
		}
		if (ys::InputManager::getKeyUp((UINT)ys::Key::Q))
		{
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
	}

	void BrickOut::Render()
	{
		PatBlt(hBackDC, 0, 0, screen.right - screen.left, screen.bottom - screen.top, WHITENESS);

		Bricks.Render(hBackDC);
		player.Render(hBackDC);

		BitBlt(hDC, 0, 0, screen.right - screen.left, screen.bottom - screen.top, hBackDC, 0, 0, SRCCOPY);
	}
}