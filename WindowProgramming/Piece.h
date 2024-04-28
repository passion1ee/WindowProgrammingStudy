#ifndef PIECE
#define PIECE
#include <string>

#include "Stone.h"
#include "Player.h"

class Player;

enum class Quantity {//string '   ', '@  ', '@ @' ////// 0 1 2
	kEmpty, kOne, kTwo
};

class Piece {
private:
	std::pair<Quantity, Quantity> state;//블럭 첫줄 state.first / 둘쨋줄 state.secsond
	Symbol curSymbol;
	Color curColor;
	int pieceId; //0 ~ 29 // 22 == 27
public:
	Piece(const int&);
	virtual ~Piece() = default;

public:
	std::pair<Quantity, Quantity> getQuantity() const;
	Symbol getShape() const;
	Color getColor() const;
	int GetId() const;

public:
	virtual std::vector<int> newID(const YutSticks&) = 0;
	void setState(const std::pair<Quantity, Quantity>&, const Symbol&, const Color&);
	//void setQuantity(const std::pair<Quantity, Quantity>&);
	//void setShape(const Symbol&);
	void setColor(const Color&);
};

class PieceNomal : public Piece {
public:
	PieceNomal(const int&);
	std::vector<int> newID(const YutSticks& yutRoll) final;
/*
	1~4
	6~9
	11~19
	21, 23, 24
	26,  28, 29
*/
};

class PieceOf5 : public Piece {// 5 20
public:
	PieceOf5(const int&);
	std::vector<int> newID(const YutSticks& yutRoll) final;

};

class PieceOf10 : public Piece {//10 25
public:
	PieceOf10(const int&);
	std::vector<int> newID(const YutSticks& yutRoll) final;

};

class PieceOf22 : public Piece {//22 27
public:
	PieceOf22(const int&);
	std::vector<int> newID(const YutSticks& yutRoll) final;

};
#endif // PIECE