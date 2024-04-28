#include "Player.h"
#include <iostream>
#include <string>
#include <random>

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_int_distribution<> chance(1, 10000);

Player::Player(const Symbol& symbol, const Color& color) {
	stones.assign(4, Stone(symbol, color));
}

std::vector<int> Player::isSamePos(const int& comp) {
	std::vector<int> compResult;
	if (comp == 22) {
		for (auto iter = stones.begin(); iter != stones.end(); ++iter)
			if (comp == iter->getPos() || 27 == iter->getPos())
				compResult.push_back(iter->getPos());
	}
	else if (comp == 27) {
		for (auto iter = stones.begin(); iter != stones.end(); ++iter)
			if (comp == iter->getPos() || 22 == iter->getPos())
				compResult.push_back(iter->getPos());
	}
	else {
		for (auto iter = stones.begin(); iter != stones.end(); ++iter)
			if (comp == iter->getPos())
				compResult.push_back(iter->getPos());
	}

	return compResult;
}

bool Player::setStonePos(const int& stonePos, const int& id) {
	auto prevSize = stones.size();
	auto stone = std::find_if(stones.begin(), stones.end(), [stonePos](const Stone& comp) { return comp.getPos() == stonePos; });
	while (stone != stones.end()) {
		if (id == 0)
			stones.erase(stone);
		else if (stonePos == -1) {
			stone->setPos(id);
			break;
		}
		else
			stone->setPos(id);

		stone = std::find_if(stones.begin(), stones.end(), [stonePos](const Stone& comp) { return comp.getPos() == stonePos; });
	}
	auto curSize = stones.size();
	if (prevSize > curSize)
		return true;
	else
		return false;
}

const size_t& Player::stoneQuantity() {
	return stones.size();
}

int Player::selectStone() const {
	std::vector<std::string> unduplicate;
	for (const auto& stone : stones) {
		const auto stonePos = stone.getPos();
		if (std::find(unduplicate.begin(), unduplicate.end(), std::to_string(stonePos)) == unduplicate.end())
			unduplicate.push_back(std::to_string(stonePos));
		else
			unduplicate.push_back("업혀있음");
	}

	std::cout << "선택 가능한 위치: \n";
	for (const auto& pos : unduplicate)
		std::cout << pos << '\t';
	std::cout << std::endl;

	while (true) {
		std::string pos{};
		std::cout << "움직일 돌의 위치를 선택하세요:";
		std::cin >> pos;
		if (pos != "업혀있음" && std::find(unduplicate.begin(), unduplicate.end(), pos) != unduplicate.end())
			return stoi(pos);
	}
}

//std::vector<int> Player::GetAllStonePos() const {
//	std::vector<int> result;
//	for (auto iter = stones.begin(); iter != stones.end(); ++iter)
//		result.push_back(iter->getPos());// Pos가 같으면 출력 X로 해야함;;
//	return result;
//}

YutSticks Player::castYut() {
	int probability = chance(dre);
	//	도:10.79%, 개:35.89%, 걸:33.49%, 윷:12.94%, 모:3.29%, 뒷도:3.6%
	if (probability <= 1079)
		return YutSticks::kDo;
	else if (probability <= 4668)
		return YutSticks::kGae;
	else if (probability <= 8017)
		return YutSticks::kGeol;
	else if (probability <= 9311)
		return YutSticks::kYut;
	else if (probability <= 9640)
		return YutSticks::kMo;
	else
		return YutSticks::kBackDo;
}

Symbol Player::getSymbol() const {
	return stones.front().getSymbol();
}

Color Player::getColor() const {
	return stones.front().getColor();
}

//int Player::SelectStone(const int& index) {
//
//}
