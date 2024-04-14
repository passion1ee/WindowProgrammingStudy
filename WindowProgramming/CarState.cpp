#include "CarState.h"
#include "Car.h"

constexpr int fAccel = 100.0f;

void HorizontalMove::Move(Car& car, RECT screen)
{
	ys::fVector tmp = car.getPosition();
	tmp.x += (1 / ys::Timer::getRealFPS()) * car.getVelocity().x;
	car.setPosition(tmp);

	float width = screen.right - screen.left;
	if(car.getPosition().x > width)
	{
		tmp.x = 0;
		car.setPosition(tmp);
	}
}

void HorizontalMove::StopAccel(Car& car)
{
	ys::fVector tmp = car.getVelocity();
	auto dSpeed = car.getSpeed() /  ys::Timer::getRealFPS();
	if (tmp.x > dSpeed)
		tmp.x -= dSpeed;
	else
		tmp.x = 0.0f;
	car.setVelocity(tmp);
}

void HorizontalMove::MoveAccel(Car& car)
{
	ys::fVector tmp = car.getVelocity();
	auto dSpeed = car.getSpeed() /  ys::Timer::getRealFPS();
	if (tmp.x < car.getSpeed() - dSpeed)
		tmp.x += dSpeed;
	else
		tmp.x = car.getSpeed();
	car.setVelocity(tmp);
}

void HorizontalMove::Accel(Car& car)
{
	car.setSpeed(car.getSpeed() + fAccel);
	auto tmp = car.getVelocity();
	car.setVelocity({ tmp.x + fAccel, tmp.y });
}

void HorizontalMove::Decel(Car& car)
{
	auto tmp = car.getVelocity();
	if (tmp.x > fAccel)
	{
		car.setSpeed(car.getSpeed() - fAccel);
		car.setVelocity({ tmp.x - fAccel, tmp.y });
	}
}

void HorizontalMove::Render(HDC hdc, Car& car, RECT screen)
{
	auto position = car.getPosition();
	auto size = car.getSize();
	float width = screen.right - screen.left;

	if(position.x >= 0 && position.x + size <= width)//윈도우 속
		Rectangle(hdc, position.x, position.y, position.x + size, position.y + size);

	if(position.x + size > width)//끝 도달
	{
		auto overflow = (position.x + size) - width;
		Rectangle(hdc, 0, position.y, overflow, position.y + size);
		Rectangle(hdc, position.x, position.y, position.x + size, position.y + size);
	}
}

//-------------------------------------------------------------------------------------------------

void HorizontalReverseMove::Move(Car& car, RECT screen)
{
	ys::fVector tmp = car.getPosition();
	tmp.x += (1 / ys::Timer::getRealFPS()) * car.getVelocity().x;
	car.setPosition(tmp);

	if (car.getPosition().x < 0)
	{
		float width = screen.right - screen.left;
		tmp.x = width;
		car.setPosition(tmp);
	}
}

void HorizontalReverseMove::StopAccel(Car& car)
{
	ys::fVector tmp = car.getVelocity();
	auto dSpeed = car.getSpeed() /  ys::Timer::getRealFPS();
	if (tmp.x < dSpeed)
		tmp.x += dSpeed;
	else
		tmp.x = 0.0f;
	car.setVelocity(tmp);
}

void HorizontalReverseMove::MoveAccel(Car& car)
{
	ys::fVector tmp = car.getVelocity();
	auto dSpeed = car.getSpeed() /  ys::Timer::getRealFPS();
	if (tmp.x >= -car.getSpeed() + dSpeed)
		tmp.x -= dSpeed;
	else
		tmp.x = -car.getSpeed();
	car.setVelocity(tmp);
}

void HorizontalReverseMove::Accel(Car& car)
{
	car.setSpeed(car.getSpeed() + fAccel);
	auto tmp = car.getVelocity();
	car.setVelocity({ tmp.x - fAccel, tmp.y });
}

void HorizontalReverseMove::Decel(Car& car)
{
	auto tmp = car.getVelocity();
	if (tmp.x < -fAccel)
	{
		car.setSpeed(car.getSpeed() - fAccel);
		car.setVelocity({ tmp.x + fAccel, tmp.y });
	}
}

void HorizontalReverseMove::Render(HDC hdc, Car& car, RECT screen)
{
	auto position = car.getPosition();
	auto size = car.getSize();
	float width = screen.right - screen.left;

	if (position.x >= 0 && position.x + size <= width)//윈도우 속
		Rectangle(hdc, position.x, position.y, position.x + size, position.y + size);

	if (position.x + size > width)//끝 도달
	{
		auto overflow = (position.x + size) - width;
		Rectangle(hdc, 0, position.y, overflow, position.y + size);
		Rectangle(hdc, position.x, position.y, position.x + size, position.y + size);
	}
}

//-------------------------------------------------------------------------------------------------

void VerticalMove::Move(Car& car, RECT screen)
{
	ys::fVector tmp = car.getPosition();
	tmp.y += (1 / ys::Timer::getRealFPS()) * car.getVelocity().y;
	car.setPosition(tmp);

	float height = screen.bottom - screen.top;
	if (car.getPosition().y > height)
	{
		tmp.y = 0;
		car.setPosition(tmp);
	}
}

void VerticalMove::StopAccel(Car& car)
{
	ys::fVector tmp = car.getVelocity();
	auto dSpeed = car.getSpeed() /  ys::Timer::getRealFPS();
	if (tmp.y > dSpeed)
		tmp.y -= dSpeed;
	else
		tmp.y = 0;
	car.setVelocity(tmp);
}

void VerticalMove::MoveAccel(Car& car)
{
	ys::fVector tmp = car.getVelocity();
	auto dSpeed = car.getSpeed() /  ys::Timer::getRealFPS();
	if (tmp.y < car.getSpeed() - dSpeed)
		tmp.y += dSpeed;
	else
		tmp.y = car.getSpeed();
	car.setVelocity(tmp);
}

void VerticalMove::Accel(Car& car)
{
	car.setSpeed(car.getSpeed() + fAccel);
	auto tmp = car.getVelocity();
	car.setVelocity({ tmp.x, tmp.y + fAccel});
}

void VerticalMove::Decel(Car& car)
{
	auto tmp = car.getVelocity();
	if (tmp.y > fAccel)
	{
		car.setSpeed(car.getSpeed() - fAccel);
		car.setVelocity({ tmp.x, tmp.y - fAccel });
	}
}

void VerticalMove::Render(HDC hdc, Car& car, RECT screen)
{
	auto position = car.getPosition();
	auto size = car.getSize();
	float height = screen.bottom - screen.top;

	if (position.y >= 0 && position.y + size <= height)//윈도우 속
		Rectangle(hdc, position.x, position.y, position.x + size, position.y + size);

	if (position.y + size > height)//끝 도달
	{
		auto overflow = (position.y + size) - height;
		Rectangle(hdc, position.x, 0, position.x + size, overflow);
		Rectangle(hdc, position.x, position.y, position.x + size, position.y + size);
	}
}

//-------------------------------------------------------------------------------------------------

void VerticalReverseMove::Move(Car& car, RECT screen)
{
	ys::fVector tmp = car.getPosition();
	tmp.y += (1 / ys::Timer::getRealFPS()) * car.getVelocity().y;
	car.setPosition(tmp);

	if (car.getPosition().y < 0)
	{
		float height = screen.bottom - screen.top;
		tmp.y = height;
		car.setPosition(tmp);
	}
}

void VerticalReverseMove::StopAccel(Car& car)
{
	ys::fVector tmp = car.getVelocity();
	auto dSpeed = car.getSpeed() /  ys::Timer::getRealFPS();
	if (tmp.y < dSpeed)
		tmp.y += dSpeed;
	else
		tmp.y = 0.0f;
	car.setVelocity(tmp);
}

void VerticalReverseMove::MoveAccel(Car& car)
{
	ys::fVector tmp = car.getVelocity();
	auto dSpeed = car.getSpeed() /  ys::Timer::getRealFPS();
	if (tmp.y >= -car.getSpeed() + dSpeed)
		tmp.y -= dSpeed;
	else
		tmp.y = -car.getSpeed();
	car.setVelocity(tmp);
}

void VerticalReverseMove::Accel(Car& car)
{
	car.setSpeed(car.getSpeed() + fAccel);
	auto tmp = car.getVelocity();
	car.setVelocity({ tmp.x, tmp.y - fAccel });
}

void VerticalReverseMove::Decel(Car& car)
{
	auto tmp = car.getVelocity();
	if (tmp.y < -fAccel)
	{
		car.setSpeed(car.getSpeed() - fAccel);
		car.setVelocity({ tmp.x, tmp.y + fAccel });
	}
}

void VerticalReverseMove::Render(HDC hdc, Car& car, RECT screen)
{
	auto position = car.getPosition();
	auto size = car.getSize();
	float height = screen.bottom - screen.top;

	if (position.y >= 0 && position.y + size <= height)//윈도우 속
		Rectangle(hdc, position.x, position.y, position.x + size, position.y + size);

	if (position.y + size > height)//끝 도달
	{
		auto overflow = (position.y + size) - height;
		Rectangle(hdc, position.x, 0, position.x + size, overflow);
		Rectangle(hdc, position.x, position.y, position.x + size, position.y + size);
	}
}