#include "BrickOut.h"
#include "stdfax.h"
#include "resource.h"

namespace ys
{
	void BrickOut::setScreen(RECT screenSize)
	{
		defaultScreen = screenSize;
		::AdjustWindowRect(&screenSize, WS_OVERLAPPEDWINDOW, TRUE);
		screen = screenSize;

		SetWindowPos(hWnd, nullptr, 0, 0, screen.right - screen.left, screen.bottom - screen.top, SWP_NOMOVE | SWP_NOZORDER);

		hBitmap = CreateCompatibleBitmap(hDC, screen.right - screen.left, screen.bottom - screen.top);
		auto oldBitmap = SelectObject(hBackDC, hBitmap);
		DeleteObject(oldBitmap);
	}

	void BrickOut::command(const WORD& command)
	{
		switch (command)
		{
		case ID_GAME_START:
			isRun = true;
			break;
		case ID_GAME_RESET:
			Bricks.Init(60, screen, true);
			break;
		case ID_GAME_END:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case ID_SPEED_FAST:
			player.setSpeed(4.0f);
			break;
		case ID_SPEED_MEDIUM:
			player.setSpeed(2.5f);
			break;
		case ID_SPEED_SLOW:
			player.setSpeed(1.0f);
			break;
		case ID_COLOR_CYAN:
			Bricks.setColor(1);
			break;
		case ID_COLOR_MAGENTA:
			Bricks.setColor(2);
			break;
		case ID_COLOR_YELLOW:
			Bricks.setColor(3);
			break;
		case ID_SIZE_SMALL:
			player.setSize(20);
			break;
		case ID_SIZE_BIG:
			player.setSize(30);
			break;
		case ID_NUMBER_3:
			Bricks.Init(60, screen, false, 3);
			break;
		case ID_NUMBER_4:
			Bricks.Init(60, screen, false, 4);
			break;
		case ID_NUMBER_5:
			Bricks.Init(60, screen, false, 5);
			break;
		default:
			break;
		}
	}

	void BrickOut::Init(HWND hWnd_, RECT& screenSize)
	{
		hWnd = hWnd_;
		hDC = GetDC(hWnd_);
		hBackDC = CreateCompatibleDC(hDC);

		BrickOut::setScreen(screenSize);
		Bricks.Init(60, screen, true);
		player.Init(fVector(screen.right / 2.0, 0));

		copyPlayers.reserve(3);
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
		{
			std::vector<bool> isCollide;
			isCollide.reserve(copyPlayers.size() + 1);

			auto bools = Bricks.CheckCollision(player);
			isCollide.push_back(bools.first);
			if (bools.second)
			{
				Beep(1760, 10);
				Beep(2218, 30);
				Ball copy;
				copy.Init(player.getPosition());
				copyPlayers.push_back(std::make_pair(copy, 20.0f));
			}

			for (auto& copyPlayer : copyPlayers)
			{
				bools = Bricks.CheckCollision(copyPlayer.first);
				isCollide.push_back(bools.first);
				if (bools.second)
				{
					Beep(1760, 10);
					Beep(2218, 30);
					Ball copy;
					copy.Init(copyPlayer.first.getPosition());
					copyPlayers.push_back(std::make_pair(copy, 20.0f));
				}
			}
			for(auto collide : isCollide)
				if (bools.first) 
				{
					isCoolTime = 1 / Timer::getRealFPS();
					break;
				}
		}

		for (auto iter = copyPlayers.begin(); iter != copyPlayers.end();)
		{
			iter->second -= 1 / ys::Timer::getRealFPS();
			if (iter->second <= 0.0f)
				iter = copyPlayers.erase(iter);
			else
			{
				iter->first.Update(screen);
				++iter;
			}
		}

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
		player.Render(hBackDC, false);
		for (auto& copyPlayer : copyPlayers)
			copyPlayer.first.Render(hBackDC);

		BitBlt(hDC, 0, 0, screen.right - screen.left, screen.bottom - screen.top, hBackDC, 0, 0, SRCCOPY);
	}
}