#pragma once
#include "Ball.h"
#include "stdfax.h"
class Brick
{
public:
	Brick(const RECT& rect, int hp, bool isAct);

	void Update();
	void Render(HDC hdc);
	
	bool CheckCollision(Ball& ball);

	const bool& isActive() const { return isAct; }
	void InActive() { isAct = false; }

	const RECT& getPosition() const { return rect; }
	const int& getHp() const { return hp;}
private:
	RECT rect;
	const int maxHp;
	int hp;
	bool isAct;
};