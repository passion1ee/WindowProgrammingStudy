#include <iostream>
#include <sstream>

#include "Board.h"
#include "Player.h"

Board::Board() {
	for (int id = 0; id < 30; ++id) {
		switch (id) {
		case 5:
		case 20:
			board.push_back(new PieceOf5(id));
			break;
		case 10:
		case 25:
			board.push_back(new PieceOf10(id));
			break;
		case 22:
			board.push_back(new PieceOf22(id));
			break;
		default:
			board.push_back(new PieceNomal(id));
			break;
		}
	}
}

std::vector<Piece*> Board::getBoard() const {
	return board;
}

std::vector<int> Board::availablePosition(const int& stoneIndex, const YutSticks& yutRoll) {
	if (stoneIndex == -1)
		return board[0]->newID(yutRoll);
	return board[stoneIndex]->newID(yutRoll);
}

Piece* Board::operator[](const int _Pos) {
	if (_Pos == -1)
		return board[0];
	return board[_Pos];
}
