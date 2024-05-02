#pragma once
#include "Ball.h"
#include "Brick.h"
#include "stdfax.h"
#include <vector>

class BrickManager
{
public:
	void Init(const int& quantity, const RECT& screenSize, bool reset, const BYTE& lineQuan_ = 3);
	void Update();
	void Render(HDC hdc);

	std::pair<bool, bool> CheckCollision(Ball& ball);
	const std::pair<int, int>& CountInAct() const;

public:
	void setColor(const BYTE& color) { this->color = color; }

private:
	std::vector<Brick> bricks; // 모든 벽돌 관리
	BYTE lineQuan;
	BYTE color;
};

