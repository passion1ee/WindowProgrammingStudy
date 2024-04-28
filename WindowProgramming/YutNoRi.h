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
		void Update();
		void Render();

		void click(const int&, const int&);
	private:
		void setScreen(RECT screen);

		bool getWhoTurn() const;
		Player& switchPlayer();
		bool advanceTurn();

		COLORREF getColor(const Color& color);
	private:
		std::pair<Quantity, Quantity> SumQuantity(const std::pair<Quantity, Quantity>&, const std::pair<Quantity, Quantity>&);
		std::string castAndnSaveYut(Player&);

	private:
		std::queue<YutSticks> rollHistory;
		bool isTwoTurn{ false };
		bool isSet{ true };
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
	};
}

#endif // !YUTNORI

