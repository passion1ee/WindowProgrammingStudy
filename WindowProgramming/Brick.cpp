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

void Brick::Render(HDC hdc, const BYTE& color)
{
	HBRUSH brush;
	HBRUSH oldBrush;
	int R{}, G{}, B{};
	switch (color)
	{
	case 1:
		R = 0; G = 162; B = 211;
		break;
	case 2:
		R = 255; G = 0; B = 144;
		break;
	case 3:
		R = 255; G = 255; B = 0;
		break;
	default:
		R = 255; G = 0; B = 0;
		break;
	}
	if (hp <= maxHp / 2)// 벽돌 색상 선택기능 추가 예정
	{
		R += (255 - R) / 2;
		G += (255 - G) / 2;
		B += (255 - B) / 2;
	}
	brush = CreateSolidBrush(RGB(R, G, B));
	oldBrush = (HBRUSH)SelectObject(hdc, brush);
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	SelectObject(hdc, oldBrush);
	DeleteObject(brush);
	
}


std::pair<bool, bool> Brick::CheckCollision(Ball& ball)
{
	auto radius = ball.getSize() / 2.0f;
	RECT circleCollider{ rect.left - radius, rect.top - radius, rect.right + radius, rect.bottom + radius };
	if (PtInRect(&circleCollider, POINT((LONG)(ball.getPosition().x + radius), (LONG)(ball.getPosition().y + radius))))
	{
		bool isItem{ false };
		Beep(345, 20);
		if (--hp <= 0)
		{
			isAct = false;
			isItem = ((ys::uid(ys::randomEngine) % 10000) > 5000) ? false : true;
		}

		if (rect.left - radius >= ball.getPosition().x)
			ball.setPosition(rect.left - radius - (ball.getSize() + 5), ball.getPosition().y);// ball size를 받아서 그 값만큼 위치 조정
		if(rect.right + radius <= ball.getPosition().x)
			ball.setPosition(rect.right + radius + (ball.getSize() + 5), ball.getPosition().y);
		if(rect.top - radius >= ball.getPosition().y)
			ball.setPosition(ball.getPosition().x, rect.top - radius - (ball.getSize() + 5));
		if(rect.bottom + radius <= ball.getPosition().y)
			ball.setPosition(ball.getPosition().x, rect.bottom + radius + (ball.getSize() + 5));

		switch (ys::uid(ys::randomEngine) % 3)//ball 기울기(속도)
		{
		case 0:// x = 200, y = (반전)100
			ball.setDirection({ ball.getDirection().x < 0 ? 2.0f : -2.0f, ball.getDirection().y < 0 ? 1.0f : -1.0f });
			break;
		case 1:// x = 200, y = (반전)200
			ball.setDirection({ ball.getDirection().x < 0 ? 2.0f : -2.0f, ball.getDirection().y < 0 ? 2.0f : -2.0f });
			break;
		case 2:// x = 100, y = (반전)200
			ball.setDirection({ ball.getDirection().x < 0 ? 1.0f : -1.0f, ball.getDirection().y < 0 ? 2.0f : -2.0f });
			break;
		}
		return std::make_pair(true, isItem);
	}
	return std::make_pair(false, false);
}



