#include "Person.h"
#include "stdfax.h"

void Person::Init(const ys::fVector& position, const ys::fVector& velocity)
{
	this->position = position;
	this->velocity = velocity;
}

bool Person::Update(ys::TrffLightSignal signal, RECT screen)
{
	auto width = screen.right - screen.left;
	auto height = screen.bottom - screen.top;
	//어떤 블럭 Rect에 있는지 검사 후 저장 -> 좌상단부터 시계방향으로 1 2 3 4 1
	RECT personRect = { position.x, position.y, position.x + size, position.y + size };
	RECT rect1 = { 0, 0, width / 3, height / 3 };
	RECT rect2 = { width * 2 / 3, 0, width, height / 3 };
	RECT rect3 = { width * 2 / 3, height * 2 / 3, width, height };
	RECT rect4 = { 0, height * 2 / 3, width / 3, height };

	RECT collideRect;
	if (IntersectRect(&collideRect, &personRect, &rect1)&& sameRect(collideRect, personRect)) collideNum = 1;
	if (IntersectRect(&collideRect, &personRect, &rect2)&& sameRect(collideRect, personRect)) collideNum = 2;
	if (IntersectRect(&collideRect, &personRect, &rect3)&& sameRect(collideRect, personRect)) collideNum = 3;
	if (IntersectRect(&collideRect, &personRect, &rect4)&& sameRect(collideRect, personRect)) collideNum = 4;

	switch (signal)
	{
	case ys::TrffLightSignal::RED://건너기
		//Move()는 
		setVelocity(screen);
		if (collideNum == 2 || collideNum == 4) Move(screen);
		break;
	case ys::TrffLightSignal::GREENtoYELLOW://방향정해
		setVelocity(screen);
		//해당 블럭 Rect의 다음 번호로 이동하도록 direction 조정
		//여기서 width값 도로인지 height값 도로인지 정하기
		// (도로의 길이)(불변) = (velocity)(얘를 바꿔야겠네) * (도로를 건널 시간(1 / getRealFPS()))(불변)
		// velocity값 조정 후 이동
		break;
	case ys::TrffLightSignal::GREEN://건너지마
		setVelocity(screen);
		if(collideNum == 1 || collideNum == 3) Move(screen);
		break;
	default:
		return false;
	}
	return true;
}

void Person::Render(HDC hdc)
{
	auto brush = CreateSolidBrush(RGB(128, 128, 255));
	auto oldBrush = (HBRUSH)SelectObject(hdc, brush);
	Ellipse(hdc, position.x, position.y, position.x + size, position.y + size);
	SelectObject(hdc, oldBrush);
	DeleteObject(brush);
}

void Person::Move(const RECT& screen)
{
	auto time = (1 / ys::Timer::getRealFPS());
	switch (collideNum)
	{
	case 1://to2
	{
		auto width = screen.right - screen.left;
		if (position.x < width * 2.0f / 3)
		{
			position.x += time * velocity.x;
			position.y += time * velocity.y;
		}
		else
		{
			velocity = { 0, 0 };
		}
		break;
	}
	case 2://to3
	{
		auto height = screen.bottom - screen.top;
		if (position.y < height * 2.0f / 3)
		{
			position.x += time * velocity.x;
			position.y += time * velocity.y;
		}
		break;
	}
	case 3://to4
	{
		auto width = screen.right - screen.left;
		if (position.x + size > width / 3.0f)
		{
			position.x += time * velocity.x;
			position.y += time * velocity.y;
		}
		break;
	}
	case 4://to1
	{
		auto height = screen.bottom - screen.top;
		if (position.y + size > height / 3.0f)
		{
			position.x += time * velocity.x;
			position.y += time * velocity.y;
		}
		break;
	}
	default:
		break;
	}
}

inline void Person::setVelocity(const RECT& screen)
{
	auto wVeloc = ((screen.right - screen.left) / 3.0f + size * 2) / 2.0f;
	auto hVeloc = ((screen.bottom - screen.top) / 3.0f + size * 2) / 2.0f;
	switch (collideNum)
	{
	case 1://to2
		velocity = { wVeloc, 0 };
		break;
	case 2://to3
		velocity = { 0, hVeloc };
		break;
	case 3://to4
		velocity = { -wVeloc, 0 };
		break;
	case 4://to1
		velocity = { 0, -hVeloc };
		break;
	default:
		velocity = { 0, 0 };
		break;
	}
}

bool Person::sameRect(const RECT& one, const RECT& other)
{
	return one.left == other.left && one.top == other.top && one.right == other.right && one.bottom == other.bottom;
}
