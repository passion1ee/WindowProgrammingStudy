#include "BrickOut.h"
#include "stdfax.h"

namespace ys
{
	void BrickOut::setScreen(RECT screenSize)
	{
		defaultScreen = screenSize;
		::AdjustWindowRect(&screenSize, WS_OVERLAPPEDWINDOW, FALSE);
		screen = screenSize;

		SetWindowPos(hWnd, nullptr, 0, 0, screen.right - screen.left, screen.bottom - screen.top, SWP_NOMOVE | SWP_NOZORDER);

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
		player.Init(fVector(screen.right / 2.0, 0));
		Timer::Init();
	}

	void BrickOut::Run()
	{
		if (InputManager::getKeyUp((UINT)Key::S))
			isRun = true;
		if (!isRun) return;
		if(Timer::getRealFPS() == 0.0f)
			InputManager::BeforeUpdate();
		Timer::Update();
		if (InputManager::getKeyUp((UINT)Key::P) || Timer::getRealFPS() != 0.0f)
		{
			if (1 / Timer::getRealFPS() <= (frameCheck += Timer::getDeltaTime()))
			{
				InputManager::BeforeUpdate();
				frameCheck -= 1 / Timer::getRealFPS();
				if(isCoolTime > 0.0f)
					isCoolTime -= 1 / Timer::getRealFPS();
				Update();
				Render();
				Timer::Render(hDC, POINT(screen.right - screen.left, screen.bottom - screen.top));
				InputManager::AfterUpdate();
			}
		}
		else if (InputManager::getKeyUp((UINT)Key::P) || Timer::getRealFPS() == 0.0f)
		{
			PatBlt(hBackDC, 0, 0, screen.right - screen.left, screen.bottom - screen.top, WHITENESS);
			std::wstring line;
			auto& tmp = Bricks.CountInAct();
			line += L"색이 변한 벽돌의 개수: " + std::to_wstring(tmp.first) + L", 없어진 개수: " + std::to_wstring(tmp.second);
			SIZE size;  GetTextExtentPoint32(hBackDC, line.c_str(), line.size(), &size);
			TextOut(hBackDC, (screen.right - screen.left) / 2 - size.cx, (screen.bottom - screen.top) / 2, line.c_str(), line.size());
			BitBlt(hDC, 0, 0, screen.right - screen.left, screen.bottom - screen.top, hBackDC, 0, 0, SRCCOPY);
			InputManager::AfterUpdate();
		}
	}

	void BrickOut::Update()
	{
		Bricks.Update();
		player.Update(screen);
		if (isCoolTime <= 0.0f)
			Bricks.CheckCollision(player) ? isCoolTime = 1 / Timer::getRealFPS() : isCoolTime = 0.0f;

		if (ys::InputManager::getKeyUp((UINT)ys::Key::N))
		{
			Init(hWnd, defaultScreen);
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