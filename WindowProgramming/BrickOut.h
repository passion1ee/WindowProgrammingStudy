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

		void setScreen(RECT& screenSize);
	private:
		HWND hWnd;
		HDC hDC;
		HDC hBackDC;
		HBITMAP hBitmap;

		RECT screen;

		float frameCheck;
		bool isRun;
		bool isStop;

		Ball player;
		BrickManager Bricks;
	};
}

