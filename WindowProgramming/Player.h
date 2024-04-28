#ifndef PLAYER
#define PLAYER

#include <vector>

#include "Stone.h"

enum class YutSticks {
	kDo = 1, kGae = 2, kGeol = 3, kYut = 4, kMo = 5, kBackDo = -1
	//	도:10.79%, 개:35.89%, 걸:33.49%, 윷:12.94%, 모:3.29%, 뒷도:3.6%
};

class Player {
private:
	std::vector<Stone> stones;
public:
	Player() = default;
	Player(const Symbol&, const Color&);
	bool setStonePos(const int&, const int&);//삭제가 되었다면(0에 도달했다면) true반환

public:
	const size_t& stoneQuantity();
	std::vector<int> isSamePos(const int& comp);
	int selectStone() const;
	//std::vector<int> GetAllStonePos() const;
	YutSticks castYut();
	Symbol getSymbol() const;
	Color getColor() const;
};

#endif // PLAYER


