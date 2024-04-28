#ifndef BOARD
#define BOARD

#include <vector>
#include "Piece.h"
#include "Player.h"

class Player;

class Board {
	friend class Piece;

private:
	std::vector<Piece*> board;//30ĭ������ ����� 29ĭ(27���� ��� X, 22�� ���Ͻ�)
	//��������
	//	5	-> jump 20
	//	10	-> jump 25
	//	22	-> jump 28
public:
	Board();
	std::vector<Piece*> getBoard() const;
	std::vector<int> availablePosition(const int& stoneIndex, const YutSticks& yutRoll);
	Piece* operator[](const int _Pos);
};

#endif // BOARD


