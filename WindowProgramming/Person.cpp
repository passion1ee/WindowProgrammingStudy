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
	//� �� Rect�� �ִ��� �˻� �� ���� -> �»�ܺ��� �ð�������� 1 2 3 4 1
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
	case ys::TrffLightSignal::RED://�ǳʱ�
		//Move()�� 
		setVelocity(screen);
		if (collideNum == 2 || collideNum == 4) Move(screen);
		break;
	case ys::TrffLightSignal::GREENtoYELLOW://��������
		setVelocity(screen);
		//�ش� �� Rect�� ���� ��ȣ�� �̵��ϵ��� direction ����
		//���⼭ width�� �������� height�� �������� ���ϱ�
		// (������ ����)(�Һ�) = (velocity)(�긦 �ٲ�߰ڳ�) * (���θ� �ǳ� �ð�(1 / getRealFPS()))(�Һ�)
		// velocity�� ���� �� �̵�
		break;
	case ys::TrffLightSignal::GREEN://�ǳ�����
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
