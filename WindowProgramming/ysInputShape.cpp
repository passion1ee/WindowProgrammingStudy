#include "ysInputShape.h"
#include "stdfax.h"
#include <tchar.h>
#include <cwctype>

constexpr BYTE kPenWidthMax = 10;

namespace ys
{
	int InputShape::screenWidth;
	int InputShape::screenHeight;
	bool InputShape::isTextBoxClicked;
	bool InputShape::isPrintAll;

	RECT InputShape::textBox;
	std::wstring InputShape::stringBuff;
	int InputShape::nextCharIndex;
	bool InputShape::isUpper;
	bool InputShape::isInsert;

	std::vector<InputShape::Object> InputShape::objectPool;
	std::vector<InputShape::Shape> InputShape::shapes;

	void InputShape::setScreen(int width, int height)
	{
		screenWidth = width;
		screenHeight = height;

		textBox = { 20, screenHeight - 60, screenWidth - 20, screenHeight - 20 };
	}

	void InputShape::TextBoxClicked(HWND hWnd, int x, int y)
	{
		isTextBoxClicked = PtInRect(&textBox, { x,y });
		if (isTextBoxClicked)
			ShowCaret(hWnd);
		else
			HideCaret(hWnd);
	}

	void InputShape::Run()
	{
		InputManager::BeforeUpdate();
		Update();
		InputManager::AfterUpdate();
	}

	void InputShape::Init()
	{
		isTextBoxClicked = true;
		isPrintAll = false;
		
		nextCharIndex = 0;
		isInsert = false;
		isUpper = false;

		objectPool.reserve(10);
	}

	void InputShape::Update()
	{
		if (isTextBoxClicked)
		{//텍스트모드
			if (ys::InputManager::getKeyDown(VK_LEFT))
			{
				if (nextCharIndex > 0)
					--nextCharIndex;
			}
			if (ys::InputManager::getKeyDown(VK_RIGHT))
			{
				if (nextCharIndex < stringBuff.size())
					++nextCharIndex;
			}

			if (ys::InputManager::getKeyDown(VK_RETURN) && objectPool.size() < 10)
			{
				InputShape::Object inputObj;
				int tmp;
				std::wstringstream ss;
				ss << stringBuff;
				stringBuff = L"";
				nextCharIndex = 0;
				ss >> tmp >> inputObj.position.left >> inputObj.position.top >> inputObj.position.right >> inputObj.position.bottom >> inputObj.penWidth;
				inputObj.shape = static_cast<InputShape::Shape>(tmp);

				if (isVal(inputObj))
				{
					Beep(1760, 100);
					Beep(2218, 300);
					std::rotate(objectPool.begin(), objectPool.end(), objectPool.end());
					inputObj.colors.first = RGB(ObjectColor(randomEngine), ObjectColor(randomEngine), ObjectColor(randomEngine));
					inputObj.colors.second = RGB(ObjectColor(randomEngine), ObjectColor(randomEngine), ObjectColor(randomEngine));
					objectPool.push_back(inputObj);
				}

				ss.str(L"");
				ss.clear();
			}

			if (ys::InputManager::getKeyDown(VK_BACK))
			{
				if (nextCharIndex > 0)
					stringBuff.erase(stringBuff.begin() + --nextCharIndex);
			}
			if (ys::InputManager::getKeyDown(VK_DELETE))
			{
				if (nextCharIndex != stringBuff.size())
					stringBuff.erase(stringBuff.begin() + nextCharIndex);
			}
			if (ys::InputManager::getKeyDown(VK_ESCAPE))
			{
				stringBuff.clear();
				nextCharIndex = 0;
			}


			if (ys::InputManager::getKeyDown(VK_HOME))
			{
				nextCharIndex = 0;
			}
			if (ys::InputManager::getKeyDown(VK_END))
			{
				nextCharIndex = stringBuff.size();
			}
			if (ys::InputManager::getKeyDown(VK_F1))
			{
				isUpper = isUpper ? false : true;
			}
			if (ys::InputManager::getKeyDown(VK_INSERT))
			{
				isInsert = isInsert ? false : true;
			}
		}
		else if(!objectPool.empty())
		{//도형모드
			if (ys::InputManager::getKeyDown(VK_LEFT))
			{
				objectPool.back().position.left -= 10;
				objectPool.back().position.right -= 10;
			}
			if (ys::InputManager::getKeyDown(VK_RIGHT))
			{
				objectPool.back().position.left += 10;
				objectPool.back().position.right += 10;
			}
			if (ys::InputManager::getKeyDown(VK_UP))
			{
				objectPool.back().position.top -= 10;
				objectPool.back().position.bottom -= 10;
			}
			if (ys::InputManager::getKeyDown(VK_DOWN))
			{
				objectPool.back().position.top += 10;
				objectPool.back().position.bottom += 10;
			}

			if (ys::InputManager::getKeyDown('1'))
			{
				objectPool.back().colors.first = RGB(ObjectColor(randomEngine), ObjectColor(randomEngine), ObjectColor(randomEngine));
			}
			if (ys::InputManager::getKeyDown('2'))
			{
				objectPool.back().colors.second = RGB(ObjectColor(randomEngine), ObjectColor(randomEngine), ObjectColor(randomEngine));
			}

			if (ys::InputManager::getKeyDown(VK_ADD))
			{//도형 테두리 크기 +변환
				if (objectPool.back().penWidth < kPenWidthMax)
				{
					objectPool.back().penWidth++;
				}
				else if(objectPool.back().size < 200)
				{
					objectPool.back().size += 2;
				}
			}
			if (ys::InputManager::getKeyDown(VK_SUBTRACT))
			{//도형 테두리 크기 -변환
				if (objectPool.back().penWidth > 0)
				{
					objectPool.back().penWidth--;
				}
				else if(objectPool.back().size > 3)
				{
					objectPool.back().size -= 2;
				}
			}
			

			if (ys::InputManager::getKeyDown((UINT)Key::A))//이전 Obj <<rotate
			{
				std::rotate(objectPool.begin(), objectPool.end() - 1, objectPool.end());
			}
			if (ys::InputManager::getKeyDown((UINT)Key::D)) //넥스트 Obj
			{
				std::rotate(objectPool.begin(), objectPool.begin() + 1, objectPool.end());
			}
			if (ys::InputManager::getKeyDown((UINT)Key::S))
			{
				isPrintAll = isPrintAll ? false : true;
			}

		}
	}

	void InputShape::render(HWND hWnd, HDC hDC)
	{
		if (!objectPool.empty())
		{
			if (isPrintAll)
			{
				for (int i = 0; i < objectPool.size(); ++i)
				{
					auto hPen = CreatePen(PS_SOLID, objectPool[i].penWidth, objectPool[i].colors.first);
					auto hBrush = CreateSolidBrush(objectPool[i].colors.second);
					auto oldPen = (HPEN)SelectObject(hDC, hPen);
					auto oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					renderObject(hDC, objectPool[i]);

					SelectObject(hDC, oldPen);
					SelectObject(hDC, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);
				}
			}
			else
			{
				auto hPen = CreatePen(PS_SOLID, objectPool.back().penWidth, objectPool.back().colors.first);
				auto hBrush = CreateSolidBrush(objectPool.back().colors.second);
				auto oldPen = (HPEN)SelectObject(hDC, hPen);
				auto oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

				renderObject(hDC, objectPool.back());

				SelectObject(hDC, oldPen);
				SelectObject(hDC, oldBrush);
				DeleteObject(hPen);
				DeleteObject(hBrush);
			}
		}
		else if(!isTextBoxClicked)
			MessageBox(hWnd, L"ObjectPool is Empty.", L"Error", NULL);

		Rectangle(hDC, textBox.left, textBox.top, textBox.right, textBox.bottom);
		if (nextCharIndex == 0)
			SetCaretPos(textBox.left + 10, textBox.top + 10);
		else
		{
			SIZE size;  GetTextExtentPoint32(hDC, stringBuff.substr(0, nextCharIndex).c_str(), stringBuff.substr(0, nextCharIndex).size(), &size);
			SetCaretPos(textBox.left + 10 + size.cx, textBox.top + 10);
		}
		TextOut(hDC, textBox.left + 10, textBox.top + 10, stringBuff.c_str(), stringBuff.size());
		TextOut(hDC, 100, 200, std::to_wstring(objectPool.size()).c_str(), std::to_wstring(objectPool.size()).size());
	}

	void InputShape::Add(WPARAM buff)
	{
		if (!isTextBoxClicked)
			return;

		std::wstring ch;

		if (isUpper)
			ch = std::towupper(buff);
		else
			ch = std::towlower(buff);

		if (isInsert || nextCharIndex == stringBuff.size())
		{
			if (stringBuff.size() < 100)
			{
				stringBuff.insert(nextCharIndex, ch);
				++nextCharIndex;
			}
		}
		else //!isInsert && 마지막문자가 아닐때
		{
			stringBuff.erase(stringBuff.begin() + nextCharIndex);
			stringBuff.insert(nextCharIndex, ch);
			++nextCharIndex;
		}
	}

	void InputShape::renderObject(HDC hDC, InputShape::Object object)
	{
		auto& rect = object.position;
		auto width = (rect.right - rect.left) * 0.01 * object.size;
		auto height = (rect.bottom - rect.top) * 0.01 * object.size;
		switch (object.shape)
		{
		case ys::InputShape::Shape::kDot:
		{
			Ellipse(hDC, rect.left, rect.top, rect.left + object.penWidth, rect.top + object.penWidth);
			break;
		}
		case ys::InputShape::Shape::kLine:
		{
			MoveToEx(hDC, rect.left, rect.top, NULL);
			LineTo(hDC, rect.left + width, rect.top + height);
			break;
		}
		case ys::InputShape::Shape::kTriangle:
		{
			POINT vertex[3] = { {static_cast<LONG>(rect.left + width / 2.0), rect.top},
					{rect.left, rect.bottom},
					{rect.right, rect.bottom}
			};
			Polygon(hDC, vertex, 3);
			break;
		}
		case ys::InputShape::Shape::kRect:
		{
			Rectangle(hDC, rect.left, rect.top, rect.left + width, rect.top + height);
			break;
		}
		case ys::InputShape::Shape::kPentagon:
		{
			POINT vertex[5] = { {static_cast<LONG>(rect.left + width / 2.0), rect.top},
						{rect.left, static_cast<LONG>(rect.top + height * 4.2 / 10.0)},
						{static_cast<LONG>(rect.left + width * 2 / 10.0), rect.top + height},
						{static_cast<LONG>(rect.left + width * 8 / 10.0), rect.top + height},
						{rect.left + width, static_cast<LONG>(rect.top + height * 4.2 / 10.0)},
			};
			Polygon(hDC, vertex, 5);
			break;
		}
		case ys::InputShape::Shape::kCircle:
		{
			auto diameter = width > height ? height : width;
			Ellipse(hDC, rect.left, rect.top, rect.left + diameter, rect.top + diameter);
			break;
		}
		default:
			break;
		}
	}

	bool InputShape::isVal(InputShape::Object& input)
	{
		if (static_cast<int>(input.shape) < 0 || static_cast<int>(input.shape) >= static_cast<int>(InputShape::Shape::kCount))
			return false;

		if (input.penWidth <= 0 || input.penWidth > kPenWidthMax)
			return false;

		RECT tmpRect = { 0, 0, screenWidth, screenHeight };
		if (!PtInRect(&tmpRect, POINT(input.position.left, input.position.top)) || 
			!PtInRect(&tmpRect, POINT(input.position.right, input.position.bottom)))
			return false;

		return true;
	}

}