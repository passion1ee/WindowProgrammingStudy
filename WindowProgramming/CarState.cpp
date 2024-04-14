#include "CarState.h"
#include "Car.h"

void HorizontalMove::Move(Car* car, RECT screen)
{
	ys::fVector tmp = car->getPosition();
	tmp.x += (1 / ys::Timer::getRealFPS()) * car->getVelocity().x;
	car->setPosition(tmp);

	float width = screen.right - screen.left;
	if(car->getPosition().x > width)
	{
		tmp.x = 0;
		car->setPosition(tmp);
	}
}

void HorizontalMove::StopAccel(Car* car)
{
	ys::fVector tmp = car->getVelocity();
	auto dSpeed = car->getSpeed() / (1 / ys::Timer::getRealFPS());
	tmp.x -= dSpeed;
	car->setVelocity(tmp);
}

void HorizontalMove::MoveAccel(Car* car)
{
	ys::fVector tmp = car->getVelocity();
	auto dSpeed = car->getSpeed() / (1 / ys::Timer::getRealFPS());
	tmp.x += dSpeed;
	car->setVelocity(tmp);
}

void HorizontalMove::Accel(Car* car)
{
	car->setSpeed(car->getSpeed() + 10.0f);
	auto tmp = car->getVelocity();
	car->setVelocity({ tmp.x + 10.0f, tmp.y });
}

void HorizontalMove::Decel(Car* car)
{
	car->setSpeed(car->getSpeed() - 10.0f);
	auto tmp = car->getVelocity();
	car->setVelocity({ tmp.x - 10.0f, tmp.y });
}

void HorizontalMove::Render(HDC hdc, Car* car, RECT screen)
{
	auto position = car->getPosition();
	auto size = car->getSize();
	float width = screen.right - screen.left;

	if(position.x + size > width)
	{
		auto overflow = (position.x + size) - width;
		Rectangle(hdc, 0, position.y, overflow, position.y + size);
		Rectangle(hdc, position.x, position.y, position.x + size, position.y + size);

	}
	if(position.x >= 0 && position.x + size <= width)
	Rectangle(hdc, position.x, position.y, position.x + size, position.y + size);
}

//-------------------------------------------------------------------------------------------------

void HorizontalReverseMove::Move(Car* car, RECT screen)
{
	ys::fVector tmp = car->getPosition();
	tmp.x += (1 / ys::Timer::getRealFPS()) * car->getVelocity().x;
	car->setPosition(tmp);

	if (car->getPosition().x < 0)
	{
		float width = screen.right - screen.left;
		tmp.x = width;
		car->setPosition(tmp);
	}
}

void HorizontalReverseMove::StopAccel(Car* car)
{
	ys::fVector tmp = car->getVelocity();
	auto dSpeed = car->getSpeed() / (1 / ys::Timer::getRealFPS());
	tmp.x += dSpeed;
	car->setVelocity(tmp);
}

void HorizontalReverseMove::MoveAccel(Car* car)
{
	ys::fVector tmp = car->getVelocity();
	auto dSpeed = car->getSpeed() / (1 / ys::Timer::getRealFPS());
	tmp.x -= dSpeed;
	car->setVelocity(tmp);
}

void HorizontalReverseMove::Accel(Car* car)
{
	car->setSpeed(car->getSpeed() - 10.0f);
	auto tmp = car->getVelocity();
	car->setVelocity({ tmp.x - 10.0f, tmp.y });
}

void HorizontalReverseMove::Decel(Car* car)
{
	car->setSpeed(car->getSpeed() + 10.0f);
	auto tmp = car->getVelocity();
	car->setVelocity({ tmp.x + 10.0f, tmp.y });
}

void HorizontalReverseMove::Render(HDC hdc, Car* car, RECT screen)
{
	
}

//-------------------------------------------------------------------------------------------------

void VerticalMove::Move(Car* car, RECT screen)
{
	ys::fVector tmp = car->getPosition();
	tmp.y += (1 / ys::Timer::getRealFPS()) * car->getVelocity().y;
	car->setPosition(tmp);

	float height = screen.bottom - screen.top;
	if (car->getPosition().y > height)
	{
		tmp.y = 0;
		car->setPosition(tmp);
	}
}

void VerticalMove::StopAccel(Car* car)
{
	ys::fVector tmp = car->getVelocity();
	auto dSpeed = car->getSpeed() / (1 / ys::Timer::getRealFPS());
	tmp.y -= dSpeed;
	car->setVelocity(tmp);
}

void VerticalMove::MoveAccel(Car* car)
{
	ys::fVector tmp = car->getVelocity();
	auto dSpeed = car->getSpeed() / (1 / ys::Timer::getRealFPS());
	tmp.y += dSpeed;
	car->setVelocity(tmp);
}

void VerticalMove::Accel(Car* car)
{
	car->setSpeed(car->getSpeed() + 10.0f);
	auto tmp = car->getVelocity();
	car->setVelocity({ tmp.x, tmp.y + 10.0f});
}

void VerticalMove::Decel(Car* car)
{
	car->setSpeed(car->getSpeed() - 10.0f);
	auto tmp = car->getVelocity();
	car->setVelocity({ tmp.x, tmp.y - 10.0f });
}

void VerticalMove::Render(HDC hdc, Car* car, RECT screen)
{
}

//-------------------------------------------------------------------------------------------------

void VerticalReverseMove::Move(Car* car, RECT screen)
{
	ys::fVector tmp = car->getPosition();
	tmp.y += (1 / ys::Timer::getRealFPS()) * car->getVelocity().y;
	car->setPosition(tmp);

	if (car->getPosition().y < 0)
	{
		float height = screen.bottom - screen.top;
		tmp.x = height;
		car->setPosition(tmp);
	}
}

void VerticalReverseMove::StopAccel(Car* car)
{
	ys::fVector tmp = car->getVelocity();
	auto dSpeed = car->getSpeed() / (1 / ys::Timer::getRealFPS());
	tmp.y += dSpeed;
	car->setVelocity(tmp);
}

void VerticalReverseMove::MoveAccel(Car* car)
{
	ys::fVector tmp = car->getVelocity();
	auto dSpeed = car->getSpeed() / (1 / ys::Timer::getRealFPS());
	tmp.y -= dSpeed;
	car->setVelocity(tmp);
}

void VerticalReverseMove::Accel(Car* car)
{
	car->setSpeed(car->getSpeed() - 10.0f);
	auto tmp = car->getVelocity();
	car->setVelocity({ tmp.x, tmp.y - 10.0f });
}

void VerticalReverseMove::Decel(Car* car)
{
	car->setSpeed(car->getSpeed() + 10.0f);
	auto tmp = car->getVelocity();
	car->setVelocity({ tmp.x, tmp.y + 10.0f });
}

void VerticalReverseMove::Render(HDC hdc, Car* car, RECT screen)
{
}

