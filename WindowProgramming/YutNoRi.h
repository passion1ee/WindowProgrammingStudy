#ifndef YUTNORI
#define YUTNORI

#include "Board.h"
#include "Player.h"
#include <windows.h>
#include <queue>
#include "stdfax.h"
namespace ys
{
	class Yutnori {
	public:
		void Init(HWND hWnd_, RECT screenSize);
		void Set();
		void Run();	
		void Update(Player& turnPlayer, Player& opponentPlayer,
			std::vector<int>& ablePos, int& prevStoneId, int& prevQuan);
		void LateUpdate(Player& turnPlayer, Player& opponentPlayer,
			std::vector<int>& ablePos, int& prevStoneId, int& prevQuan);
		void Render();

		void click(const int&, const int&);
	private:
		void setScreen(RECT screen);

		Player& switchPlayer();

		COLORREF getColor(const Color& color);
		void castAndnSaveYut(Player&);
		void victorySound();

		void renderYutQueue();
		void renderYutSet(const YutSticks& yut, const POINT& position);
		void renderYut(const POINT& position, bool isFront);

	private:
		std::queue<YutSticks> rollHistory;
		int yutCount{};
		std::pair<Player, Player> players;
		Board board;

	private:
		HWND hWnd;
		HDC hDc;
		HDC hBackDc;
		HBITMAP hBitmap;

		RECT screen;
		float width;
		float height;
		Vector2 mousePosition;

	private:
		std::vector<Color> colors;
		Symbol setSymbol = Symbol::dump;
		Color setColor = Color::dump;

	private:
		YutSticks curYut;
		bool isSet{ true };// V			Run();
		bool isGoal{ false };// V		LateUpdate();
		bool isGoalIn{ false };// V		LateUpdate();
		bool isTwoTurn{ false };// V	switchPlayer();
		bool isRoll{ false };// V		switchPlayer();
		bool ablePop{ true };//V		Update(), LateUpdate();
		bool ableSelect{ true };//V		Update(), LateUpdate();
		bool isMove{ false };// V		Update()
	};
}

#endif // !YUTNORI

