#include "BrickManager.h"

void BrickManager::Init(const int& quantity, const RECT& screenSize, bool reset, const BYTE& lineQuan_)
{
	if(reset)
	{
		lineQuan = 3;
		color = 0;
		bricks.clear();
		bricks.reserve(quantity);
	}
	else
		lineQuan = lineQuan_;

	auto lineBrickQuan = quantity / lineQuan;
	for (auto i = 0; i < quantity; ++i)
	{
		auto curLineOffsetY = lineQuan - (i / lineBrickQuan);
		long x = (screenSize.right - (lineBrickQuan * 60)) / 2 + (i % lineBrickQuan) * 60;
		long y = screenSize.bottom - (8 - curLineOffsetY) * 30;

		if (reset)
			bricks.push_back(Brick(RECT(x, y, x + 60, y + 30), 2, true));
		else
			bricks[i].setPosition(RECT(x, y, x + 60, y + 30));
	}
}

void BrickManager::Update()
{
	for (auto& brick : bricks)
	{
		if (!brick.isActive()) continue;
		brick.Update();
	}
}

void BrickManager::Render(HDC hdc)
{
	for (auto& brick : bricks)
	{
		if (!brick.isActive()) continue;
		brick.Render(hdc, color);
	}
}

std::pair<bool, bool> BrickManager::CheckCollision(Ball& ball)
{
	bool isCollide{ false };
	bool isItem{ false };
	for (auto& brick : bricks)
	{
		if (!brick.isActive()) continue;
		if (isCollide) continue;

		auto bools = brick.CheckCollision(ball);

		if (bools.first)
			isCollide = true;
		if (bools.second)
			isItem = true;
	}
	return std::make_pair(isCollide, isItem);
}

const std::pair<int, int>& BrickManager::CountInAct() const
{
	std::pair<int, int> pair{};
	for (auto& brick : bricks)
	{
		if (brick.getHp() == 1)
			pair.first++;
		if (!brick.isActive())
			pair.second++;
	}
	return pair;
}
