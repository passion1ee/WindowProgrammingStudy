#include "PaintBoard.h"
#include "stdfax.h"
#include "resource.h"

namespace ys
{
	void PaintBoard::Init(HWND hWnd_, RECT& screenSize)
	{
		hWnd = hWnd_;
		hDc = GetDC(hWnd_);
		hBackDc = CreateCompatibleDC(hDc);

		setScreen(screenSize);
		
		selectIndex = -1;
		isDot = false;
		isBoarder = true;
		isMove = true;
		printInform = false;

		mouseLclick = false;
		mouseRclick = false;
		InputManager::Init();
		Timer::Init();
	}

	void PaintBoard::Lclick(const int& x, const int& y)
	{
		if (mouseRclick) return;

		mousePosition.x = x / xGrid + 1;
		mousePosition.y = y / yGrid + 1;
	}

	void PaintBoard::Rclick(const int& x, const int& y)
	{
		if (mouseLclick) return;
		mousePosition.x = x / xGrid + 1;
		mousePosition.y = y / yGrid + 1;
		if (mouseRclick)
		{
			if (prevMousePosition.x - mousePosition.x > 0)	selectObject->Move(false, false);
			if (prevMousePosition.x - mousePosition.x < 0)	selectObject->Move(false, true);
			if (prevMousePosition.y - mousePosition.y > 0)	selectObject->Move(true, false);
			if (prevMousePosition.y - mousePosition.y < 0)	selectObject->Move(true, true);
		}
		prevMousePosition = mousePosition;
	}

	void PaintBoard::Run()
	{
		InputManager::BeforeUpdate();
		Timer::Update();

		if (Timer::getDeltaTime() > 0.0f)
		{
			Update();
			Render();
		}
		if (InputManager::getKeyDown((UINT)Key::Q))
		{
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
		InputManager::AfterUpdate();
	}

	void PaintBoard::command(const WORD& command)
	{
		switch (command)
		{
		case ID_GRID_SOLIDE:
			isDot = false;
			break;
		case ID_GRID_DOT:
			isDot = true;
			break;
		case ID_COLOR_RED:
			color = Color::RED;
			break;
		case ID_COLOR_BLUE:
			color = Color::BLUE;
			break;
		case ID_COLOR_GREEN:
			color = Color::GREEN;
			break;
		case ID_COLOR_MAGENTA:
			color = Color::MAGENTA;
			break;
		case ID_COLOR_CYAN:
			color = Color::Cyan;
			break;
		case ID_COLOR_YELLOW:
			color = Color::Yellow;
			break;
		case ID_BORDER_ON:
			isBoarder = true;
			break;
		case ID_BORDER_OFF:
			isBoarder = false; 
			break;
		case ID_MOVE_ON:
			isMove = true;
			break;
		case ID_MOVE_OFF:
			isMove = false;
			break;
		case ID_INFORM_ON:
			printInform = true;
			break;
		case ID_INFORM_OFF:
			printInform = false;
			break;
		default:
			break;
		}
	}

	void PaintBoard::Update()
	{
		if (objects.size() < 5)
		{
			if (InputManager::getKeyDown(VK_LBUTTON))
			{
				mouseLclick = true;
				startMousePosition.x = mousePosition.x - 1;
				startMousePosition.y = mousePosition.y - 1;
			}
			if (InputManager::getKeyUp(VK_LBUTTON))
			{
				if ((startMousePosition.x < mousePosition.x && startMousePosition.y < mousePosition.y) && color != Color::BLACK)
					objects.push_back(Object(RECT({ startMousePosition.x, startMousePosition.y, mousePosition.x, mousePosition.y }),
						ColorToColorRef(color)));
				mouseLclick = false;
			}
		}

		if(isMove)
		{
			if (InputManager::getKeyDown(VK_RBUTTON))
			{
				for (auto iter = objects.begin(); iter != objects.end(); ++iter)
				{
					if (PtInRect(&iter->size, POINT(mousePosition.x - 1, mousePosition.y - 1)))
					{
						selectObject = iter;
						mouseRclick = true;
						break;
					}
				}
			}
			if (InputManager::getKeyUp(VK_RBUTTON))	mouseRclick = false;
		}

		if (InputManager::getKeyDown((UINT)Key::Num1)) if(objects.size() > 0) selectIndex = 0;
		if (InputManager::getKeyDown((UINT)Key::Num2)) if(objects.size() > 1)  selectIndex = 1;
		if (InputManager::getKeyDown((UINT)Key::Num3)) if(objects.size() > 2)  selectIndex = 2;
		if (InputManager::getKeyDown((UINT)Key::Num4)) if(objects.size() > 3)  selectIndex = 3;
		if (InputManager::getKeyDown((UINT)Key::Num5)) if(objects.size() > 4)  selectIndex = 4;

		if (selectIndex != -1)
		{
			if (InputManager::getKey(VK_LEFT))	objects.at(selectIndex).Move(false, false);
			if (InputManager::getKey(VK_RIGHT))	objects.at(selectIndex).Move(false, true);
			if (InputManager::getKey(VK_UP))	objects.at(selectIndex).Move(true, false);
			if (InputManager::getKey(VK_DOWN))	objects.at(selectIndex).Move(true, true);
			if (InputManager::getKeyDown((UINT)Key::D)) 
			{
				objects.erase(objects.begin() + selectIndex);
				selectIndex = -1;
			}
		}
		if (InputManager::getKeyDown((UINT)Key::D) && mouseRclick)
		{
			objects.erase(selectObject);
			mouseRclick = false;
			selectIndex = -1;
		}

	}

	void PaintBoard::Render()
	{ 
		PatBlt(hBackDc, 0, 0, width, height, BLACKNESS);
		SetROP2(hBackDc, R2_COPYPEN);
		renderFrame();

		SetROP2(hBackDc, R2_XORPEN);
		for (auto& object : objects)
		{
			HPEN pen;
			if(selectIndex != -1)
			{
				if (objects.at(selectIndex) == object)
					pen = CreatePen(PS_SOLID, 5, RGB(255, 0, 255));
				else
					pen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
			}
			else
				pen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));

			auto oldPen = SelectObject(hBackDc, pen);
			auto brush = CreateSolidBrush(NULL_BRUSH);
			auto oldBrush = SelectObject(hBackDc, brush);
			if(isBoarder)
			{
				Rectangle(hBackDc, object.size.left * xGrid, object.size.top * yGrid,
					object.size.right * xGrid, object.size.bottom * yGrid);
			}
			SelectObject(hBackDc, oldBrush);
			DeleteObject(brush);
			SelectObject(hBackDc, oldPen);
			DeleteObject(pen);

			pen = CreatePen(PS_SOLID, 1, object.color);
			oldPen = SelectObject(hBackDc, pen);
			brush = CreateSolidBrush(object.color);
			oldBrush = SelectObject(hBackDc, brush);
			for (auto& child : object.childObjs)
				Rectangle(hBackDc, child.position.x * xGrid, child.position.y * yGrid,
					(child.position.x + 1) * xGrid, (child.position.y + 1) * yGrid);
	/*		for (int j = object.size.top; j < object.size.bottom; ++j)
				for (int i = object.size.left; i < object.size.right; ++i)
					Rectangle(hBackDc, i * xGrid, j * yGrid, (i + 1) * xGrid, (j + 1) * yGrid);*/
			SelectObject(hBackDc, oldBrush);
			DeleteObject(brush);
			SelectObject(hBackDc, oldPen);
			DeleteObject(pen);
		}

		if (printInform)
		{
			std::wstring line = L"도형 수" + std::to_wstring(objects.size()) + L"개";
			SIZE size;  GetTextExtentPoint32(hBackDc, line.c_str(), line.size(), &size);
			TextOut(hBackDc, width / 2 - size.cx, height / 10, line.c_str(), line.size());
			int i = 0;
			for (auto& object : objects)
			{
				std::wstring line = std::to_wstring(i) + L"번 도형 가로: " + std::to_wstring(object.size.right - object.size.left);
				TextOut(hBackDc, object.size.left * xGrid, object.size.top * yGrid, line.c_str(), line.size());
				line = std::to_wstring(i) + L"번 도형 세로: " + std::to_wstring(object.size.bottom - object.size.top);
				TextOut(hBackDc, object.size.left * xGrid, object.size.top * yGrid + 20, line.c_str(), line.size());
				line = L"색상: " + ColorTostring(object.color);
				TextOut(hBackDc, object.size.left * xGrid, object.size.top * yGrid + 40, line.c_str(), line.size());
				++i;
			}
		}

		if(mouseLclick)
		{
			auto pen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
			auto oldPen = SelectObject(hBackDc, pen);
			auto brush = CreateSolidBrush(NULL_BRUSH);
			auto oldBrush = SelectObject(hBackDc, brush);
			if (isBoarder)
			{
				Rectangle(hBackDc, startMousePosition.x * xGrid, startMousePosition.y * yGrid,
					mousePosition.x * xGrid, mousePosition.y * yGrid);
			}
			SelectObject(hBackDc, oldBrush);
			DeleteObject(brush);
			SelectObject(hBackDc, oldPen);
			DeleteObject(pen);

			pen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
			oldPen = SelectObject(hBackDc, pen);
			brush = CreateSolidBrush(ColorToColorRef(color));
			oldBrush = SelectObject(hBackDc, brush);
			for (int j = startMousePosition.y; j < mousePosition.y; ++j)
				for (int i = startMousePosition.x; i < mousePosition.x; ++i)
					Rectangle(hBackDc, i * xGrid, j * yGrid, (i + 1) * xGrid, (j + 1) * yGrid);
			SelectObject(hBackDc, oldBrush);
			DeleteObject(brush);
			SelectObject(hBackDc, oldPen);
			DeleteObject(pen);
		}

		SetROP2(hBackDc, R2_COPYPEN);
		BitBlt(hDc, 0, 0, width, height, hBackDc, 0, 0, SRCCOPY);
	}

	void PaintBoard::renderFrame()
	{
		HPEN pen;
		if(isDot)
			pen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
		else
			pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		auto oldPen = SelectObject(hBackDc, pen);
		for (int i = 1; i < kMapSize; ++i)
		{
			MoveToEx(hBackDc, 0, i * yGrid, NULL);
			LineTo(hBackDc, width, i * yGrid);
		}

		for (int i = 1; i < kMapSize; ++i)
		{
			MoveToEx(hBackDc, i * xGrid, 0, NULL);
			LineTo(hBackDc, i * xGrid, height);
		}
		SelectObject(hBackDc, oldPen);
		DeleteObject(pen);
	}

	void PaintBoard::setScreen(RECT& screenSize)
	{
		::AdjustWindowRect(&screenSize, WS_OVERLAPPEDWINDOW, FALSE);
		screen = screenSize;
		width = screen.right + screen.left; height = screen.bottom + screen.top;
		SetWindowPos(hWnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);

		hBitmap = CreateCompatibleBitmap(hDc, width, height);
		auto oldBitmap = SelectObject(hBackDc, hBitmap);
		DeleteObject(oldBitmap);
		xGrid = width / static_cast<double>(kMapSize);
		yGrid = height / static_cast<double>(kMapSize);
	}
	COLORREF PaintBoard::ColorToColorRef(const Color& color)
	{
		switch (color)
		{
		case ys::PaintBoard::Color::BLACK:
			return RGB(0, 0, 0);
		case ys::PaintBoard::Color::RED:
			return RGB(255, 0, 0);
		case ys::PaintBoard::Color::BLUE:
			return RGB(0, 0, 255);
		case ys::PaintBoard::Color::GREEN:
			return RGB(0, 255, 0);
		case ys::PaintBoard::Color::MAGENTA:
			return RGB(255, 0, 255);
		case ys::PaintBoard::Color::Cyan:
			return RGB(0, 255, 255);
		case ys::PaintBoard::Color::Yellow:
			return RGB(255, 255, 0);
		default:
			return RGB(255, 255, 255);
		}
	}
	std::wstring PaintBoard::ColorTostring(const COLORREF& color)
	{
		switch (color)
		{
		case RGB(0, 0, 0):
			return std::wstring(L"BLACK");
		case RGB(255, 0, 0):
			return std::wstring(L"RED");
		case RGB(0, 0, 255):
			return std::wstring(L"BLUE");
		case RGB(0, 255, 0):
			return std::wstring(L"GREEN");
		case RGB(255, 0, 255):
			return std::wstring(L"MAGENTA");
		case RGB(0, 255, 255):
			return std::wstring(L"Cyan");
		case RGB(255, 255, 0):
			return std::wstring(L"Yellow");
		default:
			return std::wstring();
		}
	
	}
}
