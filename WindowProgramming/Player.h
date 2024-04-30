#ifndef PLAYER
#define PLAYER

#include <vector>

#include "Stone.h"

enum class YutSticks {
	kDo = 1, kGae = 2, kGeol = 3, kYut = 4, kMo = 5, kBackDo = -1
	//	��:10.79%, ��:35.89%, ��:33.49%, ��:12.94%, ��:3.29%, �޵�:3.6%
};

class Player {
private:
	std::vector<Stone> stones;
	int y;

public:
	Player() = default;
	Player(const Symbol&, const Color&, const int& y);
	bool setStoneCurId(const int&, const int&);//������ �Ǿ��ٸ�(0�� �����ߴٸ�) true��ȯ
	void render(HDC hdc);

public:
	const size_t& curStoneCount();
	std::vector<int> isSameId(const int& comp);
	int selectStone(const POINT& mouse) const;
	//std::vector<int> GetAllStonePos() const;
	YutSticks castYut();
	Symbol getSymbol() const;
	Color getColor() const;
};

#endif // PLAYER


