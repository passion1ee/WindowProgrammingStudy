#include "Brick.h"

Brick::Brick(const RECT& rect, int hp, bool isAct) : rect(rect), maxHp(hp), hp(hp), isAct(isAct) {}

void Brick::Update()
{
	if (ys::InputManager::getKey(VK_LBUTTON))
	{
		OffsetRect(&rect, (1 / ys::Timer::getRealFPS()) * -300, 0);
	}
	if (ys::InputManager::getKey(VK_RBUTTON))
	{
		OffsetRect(&rect, (1 / ys::Timer::getRealFPS()) * 300, 0);
	}
}

void Brick::Render(HDC hdc)
{
	HBRUSH brush;
	HBRUSH oldBrush;
	if (hp <= maxHp / 2)
	{
		brush = CreateSolidBrush(RGB(255, 125, 125));
		oldBrush = (HBRUSH)SelectObject(hdc, brush);
	}
	else
	{
		brush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, brush);
	}
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	SelectObject(hdc, oldBrush);
	DeleteObject(brush);
	
}


bool Brick::CheckCollision(Ball& ball)
{
	auto radius = ball.getSize() / 2.0f;
	RECT circleCollider{ rect.left - radius, rect.top - radius, rect.right + radius, rect.bottom + radius };
	if (PtInRect(&circleCollider, POINT((LONG)(ball.getPosition().x + radius), (LONG)(ball.getPosition().y + radius))))
	{
		Beep(345, 20);
		if (--hp <= 0)
			isAct = false;

		if (rect.left >= ball.getPosition().x + radius)
			ball.setPosition(rect.left - radius - 10, ball.getPosition().y);
		if(rect.right <= ball.getPosition().x + radius)
			ball.setPosition(rect.right + radius + 10, ball.getPosition().y);
		if(rect.top >= ball.getPosition().y + radius)
			ball.setPosition(ball.getPosition().x, rect.top - radius - 10);
		if(rect.bottom <= ball.getPosition().y + radius)
			ball.setPosition(ball.getPosition().x, rect.bottom + radius + 10);

		switch (ys::uid(ys::randomEngine) % 3)//ball 기울기(속도)
		{
		case 0:// x = 200, y = (반전)100
			ball.setDirection({ 2.0f, ball.getDirection().y < 0 ? 1.0f : -1.0f });
			break;
		case 1:// x = 200, y = (반전)200
			ball.setDirection({ 2.0f, ball.getDirection().y < 0 ? 2.0f : -2.0f });
			break;
		case 2:// x = 100, y = (반전)200
			ball.setDirection({ 1.0f, ball.getDirection().y < 0 ? 2.0f : -2.0f });
			break;
		}
		ball.setDirection( (ys::uid(ys::randomEngine) % 2 == 0 ? -ball.getDirection().x : ball.getDirection().x), ball.getDirection().y );
		return true;
	}
	return false;
}



