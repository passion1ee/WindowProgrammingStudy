#include "Stone.h"
#include "stdfax.h"

void Stone::render(HDC hdc, const int& quantt)
{
	HBRUSH brush{};
	switch (team)
	{
	case Color::kRed:
		brush = CreateSolidBrush(RGB(255, 0, 0));
		break;
	case Color::kBlue:
		brush = CreateSolidBrush(RGB(0, 0, 255));
		break;
	case Color::kGreen:
		brush = CreateSolidBrush(RGB(0, 255, 0));
		break;
	case Color::kYellow:
		brush = CreateSolidBrush(RGB(255, 255, 0));
		break;
	case Color::dump:
		brush = CreateSolidBrush(RGB(255, 255, 255));
		break;
	case Color::prevMove:
		brush = CreateSolidBrush(RGB(255, 0, 144));
		break;
	case Color::predictedMove:
		brush = CreateSolidBrush(RGB(0, 183, 235));
		break;
	}
	auto oldBrush = SelectObject(hdc, brush);

	for(int i = 0; i < quantt; ++i)
	{
		POINT offset = { (i % 2), (i / 2)};
		switch (shape)
		{
		case Symbol::Hash:

			Rectangle(hdc, position.x + 10 + offset.x * 30, position.y + 10 + offset.y * 30, 
				position.x + 30 + offset.x * 30, position.y + 30 + offset.y * 30);
			break;
		case Symbol::circle:
			Ellipse(hdc, position.x + 10 + offset.x * 30, position.y + 10 + offset.y * 30,
				position.x + 30 + offset.x * 30, position.y + 30 + offset.y * 30);
			break;
		case Symbol::dump:
			break;
		default:
			break;
		}
	}
	SelectObject(hdc, oldBrush);
	DeleteObject(brush);
}

bool Stone::collide(const POINT& mouse) const
{
    RECT rect = { position.x + 10, position.y + 10, position.x + 30, position.y + 30 };
    if (PtInRect(&rect, mouse))
        return true;
    return false;
}