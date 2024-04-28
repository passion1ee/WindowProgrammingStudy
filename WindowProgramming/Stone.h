#ifndef STONE
#define STONE

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
	Stone(const Symbol& symbol, const Color& color) : position(-1), shape(symbol), team(color) {}

public:
	int getPos() const { return position; }
	void setPos(const int& setVal) { position = setVal; }
	Symbol getSymbol() const { return shape; }
	Color getColor() const { return team; }

private:
	int position;
	Symbol shape;
	Color team;
};
#endif // STONE