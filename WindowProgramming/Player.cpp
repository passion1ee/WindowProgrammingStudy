#include "Player.h"
#include <iostream>
#include <string>
#include <random>

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_int_distribution<> chance(1, 10000);

Player::Player(const Symbol& symbol, const Color& color, const int& y) {
	stones.assign(4, Stone(symbol, color));
	this->y = y;

	int i{};
	for (auto iter = stones.begin(); iter != stones.end(); ++iter)
		if (-1 == iter->getCurId())
		{
			iter->setPos(POINT(900 + i * 30, y));
			i++;
		}
}

std::vector<int> Player::isSameId(const int& comp) {
	std::vector<int> compResult;
	if (comp == 22) {
		for (auto iter = stones.begin(); iter != stones.end(); ++iter)
			if (comp == iter->getCurId() || 27 == iter->getCurId())
				compResult.push_back(iter->getCurId());
	}
	else if (comp == 27) {
		for (auto iter = stones.begin(); iter != stones.end(); ++iter)
			if (comp == iter->getCurId() || 22 == iter->getCurId())
				compResult.push_back(iter->getCurId());
	}
	else {
		for (auto iter = stones.begin(); iter != stones.end(); ++iter)
			if (comp == iter->getCurId())
				compResult.push_back(iter->getCurId());
	}

	return compResult;
}

bool Player::setStoneCurId(const int& stoneId, const int& id) {
	auto prevSize = stones.size();
	auto stone = std::find_if(stones.begin(), stones.end(), [stoneId](const Stone& comp) { return comp.getCurId() == stoneId; });
	while (stone != stones.end()) {//같은 Id에 있는 돌 모두 옮기기
		if (id == 0)
			stones.erase(stone);
		else if (stoneId == -1) {
			stone->setId(id);
			break;
		}
		else
			stone->setId(id);

		stone = std::find_if(stones.begin(), stones.end(), [stoneId](const Stone& comp) { return comp.getCurId() == stoneId; });
	}
	int i{};

	auto curSize = stones.size();
	if (prevSize > curSize)
		return true;
	else
		return false;
}

void Player::render(HDC hdc)
{
	std::vector<int> sameCheck;
	for (auto& stone : stones)
	{
		const int& id = stone.getCurId();

		if (id == -1) stone.setPos({ 900, y });
		if (id == 0) stone.setPos({ 540, 540 });
		if (id == 5) stone.setPos({ 540, 30 });
		if (id == 10) stone.setPos({ 30, 30 });
		if (id == 15) stone.setPos({ 30, 540 });
		if (id == 22 || id == 27) stone.setPos({ 285, 285 });

		if (id >= 1 && id <= 4) stone.setPos({ 540, (4 - id) * 90 + 150 });
		if (id >= 6 && id <= 9) stone.setPos({ (9 - id) * 90 + 150, 30 });
		if (id >= 11 && id <= 14) stone.setPos({ 30, (id - 11) * 90 + 150});
		if (id >= 16 && id <= 19) stone.setPos({ (id - 16) * 90 + 150, 540 });

		if (id == 25 || id == 26) stone.setPos({ (id - 25) * 50 + 135, (id - 25) * 80 + 115 });
		if (id == 28 || id == 29) stone.setPos({ (id - 28) * 50 + 390, (id - 28) * 80 + 370 });
		if (id == 20 || id == 21) stone.setPos({ 510 - ((id - 20) * 50 + 75), (id - 20) * 80 + 115 });
		if (id == 23 || id == 24) stone.setPos({ 510 - ((id - 23) * 50 + 330), (id - 23) * 80 + 370 });

		if (std::find(sameCheck.begin(), sameCheck.end(), id) != sameCheck.end()) continue;
		sameCheck.push_back(id);
		stone.render(hdc, isSameId(id).size());
	}
}

const size_t& Player::curStoneCount() {
	return stones.size();
}

int Player::selectStone(const POINT& mouse) const {
	for (auto& stone : stones)
	{
		if (stone.collide(mouse))
			return stone.getCurId();
	}
	return -2;
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
