#pragma once
#include "stdfax.h"
#include "Ball.h"
#include "BrickManager.h"

namespace ys
{
	class BrickOut
	{
	public:
		void Init(HWND hWnd_, RECT& screenSize);
		void Run();
		void Update();
		void Render();

		void setScreen(RECT screenSize);
		void command(const WORD& command);

	private:
		HWND hWnd;
		HDC hDC;
		HDC hBackDC;
		HBITMAP hBitmap;

		RECT defaultScreen;
		RECT screen;

		float isCoolTime;
		float secondBall;
		float frameCheck;
		bool isRun;

		std::vector<std::pair<Ball, float>> copyPlayers;
		Ball player;
		BrickManager Bricks;
	};
}

