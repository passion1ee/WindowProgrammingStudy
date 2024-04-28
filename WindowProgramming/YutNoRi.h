#ifndef YUTNORI
#define YUTNORI

#include "Board.h"
#include "Player.h"
#include "Renderer.h"
#include <queue>

namespace ys
{
	class Yutnori {
	public:
		void Init();
		void Run();	
		void Update();
		void Render();

	private:
		bool getWhoTurn() const;
		Player& switchPlayer();
		bool advanceTurn();

	private:
		std::pair<Quantity, Quantity> SumQuantity(const std::pair<Quantity, Quantity>&, const std::pair<Quantity, Quantity>&);
		std::string castAndnSaveYut(Player&);

	private:
		std::queue<YutSticks> rollHistory;
		bool isTwoTurn{ false };
		std::pair<Player, Player> players;
		Board board;

	private:
		HWND hWND;
		HDC hDC;
		HDC hBackDC;
		HBITMAP hBitmap;

		RECT screen;
		POINT mousePosition;
	};
}

#endif // !YUTNORI

