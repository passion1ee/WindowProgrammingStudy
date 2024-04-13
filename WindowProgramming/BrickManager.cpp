#include "BrickManager.h"

void BrickManager::Init(const int& quantity, const RECT& screenSize)
{
	const auto& xSize = (screenSize.right - screenSize.left) / ((quantity / 3) + 10);
	const auto& ySize = (screenSize.bottom - screenSize.top) / ((quantity / 3) + 10);

	bricks.clear();
	bricks.reserve(quantity);
	for (auto i = 0; i < quantity; ++i)
	{
		long x = 5 * xSize + ((i % (quantity / 3)) * xSize);
		long y;
		if (i < quantity / 3)
			y = screenSize.bottom - 5 * ySize;
		else if(i < quantity * 2 / 3)
			y = screenSize.bottom - 4 * ySize;
		else
			y = screenSize.bottom - 3 * ySize;

		bricks.push_back(Brick(RECT(x, y, x + xSize, y + ySize), 2, true));
	}
}

void BrickManager::Update()
{
	for (auto& brick : bricks)
	{
		brick.Update();
	}
}

void BrickManager::Render(HDC hdc)
{
	for (auto& brick : bricks)
	{
		brick.Render(hdc);
	}
}

void BrickManager::CheckCollision(Ball& ball)
{
	for (auto& brick : bricks)
	{
		if (!brick.isActive()) continue;
		brick.CheckCollision(ball);
	}
	
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
