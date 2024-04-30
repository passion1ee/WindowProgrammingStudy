#ifndef STONE
#define STONE

#include <Windows.h>

enum class Symbol {
	Hash, circle, quantity, dump
	//	31m	34m 32m 33m
};

enum class Color {
	kRed, kBlue, kGreen, kYellow, quantity, dump, prevMove, predictedMove
};

class Stone {
public:
	Stone() = default;
	Stone(const Symbol& symbol, const Color& color) : curPieceId(-1), shape(symbol), team(color) {}

	void render(HDC hdc, const int& quantt);

public:
	bool collide(const POINT& point) const;
	int getCurId() const { return curPieceId; }
	void setId(const int& setVal) { curPieceId = setVal; }
	void setPos(const POINT& setVal) { position = setVal; }
	Symbol getSymbol() const { return shape; }
	Color getColor() const { return team; }

private:
	POINT position{};
	int curPieceId;
	Symbol shape;
	Color team;
};
#endif // STONE