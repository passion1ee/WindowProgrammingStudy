#include "Ball.h"
#include "stdfax.h"
constexpr BYTE scalar = 100;

Ball::Ball() : position({ 0.0, 0.0 }), Direction({ 0, 0 }), velocity(0), size(10) {}

void Ball::Init(ys::fVector position_)
{
	position = { position_.x, position_.y };
	Direction = { 1, 1 };
	velocity = 1.0f;
	size = 30;
}

void Ball::Update(RECT& clientRect)
{
	position.x += 1 / ys::Timer::getRealFPS() * Direction.x * scalar * velocity;
	position.y += 1 / ys::Timer::getRealFPS() * Direction.y * scalar * velocity;

	if (ys::InputManager::getKey(VK_ADD))
	{
		velocity += 0.01f;
	}
	if (ys::InputManager::getKey(VK_SUBTRACT))
	{
		velocity -= 0.01f;
	}
	// ȭ�� ��� �浹 �˻�
	if (position.x < 0.0 || position.x + size > clientRect.right) 
	{
		Beep(1415, 30);
		Direction.x = -Direction.x;// ���� ���� ������� ������ȯ
		if (position.x < 0.0)
			position.x = 0;
		if (position.x + size > clientRect.right)
			position.x = clientRect.right - size;
	}

	if (position.y < 0.0) 
	{
		Beep(1415, 30);
		Direction.y = -Direction.y;//�� ���� ������� �� ��������
		Direction.x = 0.0;
		if (position.y < 0.0)
			position.y = 0;
	}
	if (position.y + size > clientRect.bottom && Direction.x != 0.0) //�� ���� ������� ���� �ö󰡱�
	{
		Beep(1415, 30);
		Direction.y = -Direction.y;
		if (position.y + size > clientRect.bottom)
			position.y = clientRect.bottom - size;
	}
	else if (position.y + size > clientRect.bottom && Direction.x == 0.0)//�� �������� �� �� �������
	{
		Beep(1415, 30);
		Direction.x = 2.0f;
		Direction = { (ys::uid(ys::randomEngine) % 2 == 0 ? -Direction.x : Direction.x), -Direction.y };
		if (position.y + size > clientRect.bottom)
			position.y = clientRect.bottom - size;
	}
}

void Ball::Render(HDC hdc)
{
	std::wstring text;
	text += L"x: " + std::to_wstring(position.x) + L", y: " + std::to_wstring(position.y) +
		L", speed: " + std::to_wstring(velocity);
	TextOut(hdc, 200, 0, text.c_str(), text.size()); 
	std::wstring text2;
	text2 += L"Direction: " + std::to_wstring(Direction.x) + L", Direction: " + std::to_wstring(Direction.y);
	TextOut(hdc, 200, 20, text2.c_str(), text2.size());
	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
	Ellipse(hdc, position.x, position.y, position.x + size, position.y + size);
	SelectObject(hdc, oldBrush);
	DeleteObject(brush);
}
