#pragma once
#include "Ball.h"
#include "Brick.h"
#include "stdfax.h"
#include <vector>

class BrickManager
{
public:
	void Init(const int& quantity, const RECT& screenSize);
	void Update();
	void Render(HDC hdc);

	bool CheckCollision(Ball& ball);
	const std::pair<int, int>& CountInAct() const;
private:
	std::vector<Brick> bricks; // 모든 벽돌 관리
};

