#include "Ball.h"
#include "stdfax.h"
Ball::Ball() : position({ 0.0, 0.0 }), Direction({ 0, 0 }), velocity({ 0, 0 }), size(10) {}

void Ball::Init(ys::fVector position_)
{
    position = { position_.x, position_.y };
    Direction = { 1, 1 };
    velocity = { 200, 200 };
    size = 30;
}

void Ball::Update(RECT& clientRect)
{
    position.x += (1 / ys::Timer::getRealFPS()) * Direction.x * velocity.x;
    position.y += (1 / ys::Timer::getRealFPS()) * Direction.y * velocity.y;

    if (ys::InputManager::getKeyUp(VK_ADD))
    {
        velocity++;
    }
    if (ys::InputManager::getKeyUp(VK_SUBTRACT))
    {
        velocity++;
    }
    // ȭ�� ��� �浹 �˻�
    if (position.x < 0.0 || position.x + size > clientRect.right) Direction.x = -Direction.x;// ���� ���� ������� ������ȯ

    if (position.y < 0.0) 
    {
        Direction.y = -Direction.y;//�� ���� ������� �� ��������
        velocity.x = 0.0;
    }
    if (position.y + size > clientRect.bottom && velocity.x != 0.0) //�� ���� ������� ���� �ö󰡱�
        Direction.y = -Direction.y;
    else if (position.y + size > clientRect.bottom && velocity.x == 0.0)
    {
        velocity = { 200.0, -velocity.y };
        Direction = { static_cast<float>(ys::uid(ys::randomEngine) % 2 == 0 ? -Direction.x : Direction.x), -Direction.y };
    }
}

void Ball::Render(HDC hdc)
{
    Ellipse(hdc, position.x, position.y, position.x + size, position.y + size);
}
