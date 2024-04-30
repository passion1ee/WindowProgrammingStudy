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

std::vector<int> Board::availablePosition(const int& stoneIndex, const int& yutRoll) {
	if (stoneIndex == -1)
		return board[0]->newID(yutRoll);
	return board[stoneIndex]->newID(yutRoll);
}

void Board::render(HDC hdc)
{
	auto brush = CreateSolidBrush(RGB(245, 236, 205));
	auto oldBrush = SelectObject(hdc, brush);
	Rectangle(hdc, 0, 0, 640, 640);
	SelectObject(hdc, oldBrush);
	DeleteObject(brush);

	brush = CreateSolidBrush(RGB(232, 218, 184));
	oldBrush = SelectObject(hdc, brush);
	Ellipse(hdc, 0, 0, 130, 130);
	Ellipse(hdc, 510, 0, 640, 130);
	Ellipse(hdc, 0, 510, 130, 640);
	Ellipse(hdc, 510, 510, 640, 640);
	for(int i = 0; i < 4; ++i)
	{
		Ellipse(hdc, i * 90 + 150, 30, i * 90 + 220, 100);
		Ellipse(hdc, 30, i * 90 + 150, 100, i * 90 + 220);
		Ellipse(hdc, i * 90 + 150, 540, i * 90 + 220, 610);
		Ellipse(hdc, 540, i * 90 + 150, 610, i * 90 + 220);
	}

	Ellipse(hdc, 320 - 65, 320 - 65, 385, 385);
	for(int i = 0; i < 2; ++i)
	{
		Ellipse(hdc, i * 50 + 135, i * 80 + 115, i * 50 + 205, i * 80 + 185);
		Ellipse(hdc, i * 50 + 390, i * 80 + 370, i * 50 + 460, i * 80 + 440);
	}
	for(int i = 0; i < 2; ++i)
	{
		Ellipse(hdc, 510 - (i * 50 + 75), i * 80 + 115, 580 - (i * 50 + 75), i * 80 + 185);
		Ellipse(hdc, 510 - (i * 50 + 330), i * 80 + 370, 580 - (i * 50 + 330), i * 80 + 440);
	}
	SelectObject(hdc, oldBrush);
	DeleteObject(brush);
}

Piece* Board::operator[](const int _Pos) {
	if (_Pos == -1)
		return board[0];
	return board[_Pos];
}

void Board::clear()
{
	for (auto piece : board)
		delete piece;
	board.clear();

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
