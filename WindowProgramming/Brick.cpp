#include "Brick.h"

Brick::Brick(const RECT& rect, int hp, bool isAct) : rect(rect), maxHp(hp), hp(hp), isAct(isAct) {}

void Brick::Update()
{
    if (ys::InputManager::getKey(VK_LBUTTON))
    {
        OffsetRect(&rect, (1 / ys::Timer::getRealFPS()) * -100, 0);
    }
    if (ys::InputManager::getKey(VK_RBUTTON))
    {
        OffsetRect(&rect, (1 / ys::Timer::getRealFPS()) * 100, 0);
    }
}

void Brick::Render(HDC hdc)
{
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
}


void Brick::CheckCollision(Ball& ball)
{
    if (PtInRect(&rect, POINT(ball.getPosition().x, ball.getPosition().y)))
    {
        if (--hp < 0)
            isAct = false;
        switch (ys::uid(ys::randomEngine) % 3)//ball 기울기(속도)
        {
        case 0:// x = 200, y = (반전)100
            ball.setVelocity({ 200, ball.getVelocity().y });
            break;
        case 1:// x = 200, y = (반전)200
            ball.setVelocity({ 200, ball.getVelocity().y });
            break;
        case 2:// x = 100, y = (반전)200
            ball.setVelocity({ 100, ball.getVelocity().y });
            break;
        }

        if (ys::uid(ys::randomEngine) % 2 == 0)
            ball.setVelocity({ -ball.getVelocity().x, ball.getVelocity().y });
    }
}



