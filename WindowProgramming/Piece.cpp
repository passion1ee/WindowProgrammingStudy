#include "Piece.h"
#include "Player.h"

Piece::Piece(const int& id)
	: pieceId(id), state({ Quantity::kEmpty ,Quantity::kEmpty }), curSymbol(Symbol::dump), curColor(Color::dump) {}

std::pair<Quantity, Quantity> Piece::getQuantity() const {
	return state;
}

Symbol Piece::getShape() const
{
	return curSymbol;
}

Color Piece::getColor() const
{
	return curColor;
}

int Piece::GetId() const {

	return pieceId;
}

void Piece::setState(const std::pair<Quantity, Quantity>& setQuan, const Symbol& setShape, const Color& setColor) {
	state = setQuan;
	curSymbol = setShape;
	curColor = setColor;
}

//void Piece::setQuantity(const std::pair<Quantity, Quantity>& setVal) {
//	state = setVal;
//}

//void Piece::setShape(const Symbol& setVal) {
//	curSymbol = setVal;
//}

void Piece::setColor(const Color& setVal) {
	curColor = setVal;
}
//--------------------------------------------------------------------------------------------------------------------------------------

PieceNomal::PieceNomal(const int& id) : Piece(id) {}

std::vector<int> PieceNomal::newID(const YutSticks& yutRoll) {
	auto id = GetId();

	if (yutRoll == YutSticks::kBackDo) {
		if (id == 1)
			return std::vector<int>(1, -1);
		else
			return std::vector<int>(1, id + static_cast<int>(yutRoll));
	}

	for (int yutCount = static_cast<int>(yutRoll); yutCount != 0; --yutCount) {
		if (id == 24)
			id = 15;
		else if (id == 19)
			return std::vector<int>(1, 0);
		else if (id == 29)
			return std::vector<int>(1, 0);
		else
			++id;
	}
	return std::vector<int>(1, id);
}

//--------------------------------------------------------------------------------------------------------------------------------------

PieceOf5::PieceOf5(const int& id) : Piece(id) {}

std::vector<int> PieceOf5::newID(const YutSticks& yutRoll) {
	constexpr int kStartNum = 5;
	constexpr int kMiddleStartNum = 20;
	constexpr int kMiddleEndNum = 24;
	constexpr int kEndNum = 15;
	auto id = GetId();//직진
	if (id == kStartNum) {
		if (yutRoll == YutSticks::kBackDo)
			return std::vector<int>(1, id + static_cast<int>(yutRoll));

		auto specialID = id;//대각선으로 갈 숫자
		specialID = kMiddleStartNum;//이동(점프)
		int yutCount = static_cast<int>(yutRoll) - 1;//대각선 진입했기 때문에 1 감소

		id += static_cast<int>(yutRoll);
		specialID += yutCount;

		std::vector<int> result;
		result.push_back(id);
		result.push_back(specialID);

		return result;
	}
	else {//id == kMiddleStartNum
		if (yutRoll == YutSticks::kBackDo)
			return std::vector<int>(1, kStartNum);

		for (int yutCount = static_cast<int>(yutRoll); yutCount != 0; --yutCount) {
			if (id == kMiddleEndNum)
				id = kEndNum;
			else
				++id;
		}
		return std::vector<int>(1, id);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------

PieceOf10::PieceOf10(const int& id) : Piece(id) {}

std::vector<int> PieceOf10::newID(const YutSticks& yutRoll) {
	constexpr int kStartNum = 10;
	constexpr int kMiddleStartNum = 25;
	constexpr int kMiddleEndNum = 29;
	constexpr int kEndNum = 0;
	auto id = GetId();//직진
	if (id == kStartNum) {
		if (yutRoll == YutSticks::kBackDo)
			return std::vector<int>(1, id + static_cast<int>(yutRoll));

		auto specialID = id;//대각선으로 갈 숫자
		specialID = kMiddleStartNum;//이동(점프)
		int yutCount = static_cast<int>(yutRoll) - 1;//대각선 진입했기 때문에 1 감소

		id += static_cast<int>(yutRoll);
		specialID += yutCount;

		std::vector<int> result;
		result.push_back(id);
		result.push_back(specialID);

		return result;
	}
	else {//id == kMiddleStartNum
		if (yutRoll == YutSticks::kBackDo)
			return std::vector<int>(1, kStartNum);

		for (int yutCount = static_cast<int>(yutRoll); yutCount != 0; --yutCount) {
			if (id == kMiddleEndNum)
				return std::vector<int>(1, kEndNum);
			else
				++id;
		}
		return std::vector<int>(1, id);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------

PieceOf22::PieceOf22(const int& id) : Piece(id) {}

std::vector<int> PieceOf22::newID(const YutSticks& yutRoll) {
	auto id = GetId();
	if (yutRoll == YutSticks::kBackDo)
		return std::vector<int>(1, id + static_cast<int>(yutRoll));


	if (yutRoll == YutSticks::kBackDo)
		return std::vector<int>(1, id + static_cast<int>(yutRoll));

	std::vector<int> result;

	if (id == 22) {
		for (int yutCount = static_cast<int>(yutRoll); yutCount != 0; --yutCount) {
			if (id == 24)
				id = 15;
			else
				++id;
		}
		result.push_back(id);//일반 진행

		auto specialID = 28;
		int yutCount = static_cast<int>(yutRoll) - 1;
		specialID += yutCount;
		if (specialID > 29)
			result.push_back(0);
		else
			result.push_back(specialID);//꺾음
	}
	else {//if(id == 27)
		for (int yutCount = static_cast<int>(yutRoll); yutCount != 0; --yutCount) {
			if (id == 29)
				return std::vector<int>(1, 0);
			else
				++id;
		}
	}
	return result;
}
