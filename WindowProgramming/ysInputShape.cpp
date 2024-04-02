#include "ysInputShape.h"
#include "stdfax.h"
#include <tchar.h>
#include <cwctype>

namespace ys
{
	int InputShape::screenWidth;
	int InputShape::screenHeight;

	RECT InputShape::textBox;
	bool InputShape::isTextBoxClicked;
	std::wstring InputShape::stringBuff;
	int InputShape::nextCharIndex;
	bool InputShape::isUpper;
	bool InputShape::isInsert;

	std::vector<Object> InputShape::selectObj;
	size_t InputShape::selectedIndex;
	std::vector<Shape> InputShape::shapes;

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
		isTextBoxClicked = false;
		nextCharIndex = 0;
		isInsert = false;
		isUpper = false;

		selectObj.assign(10, Object());
		selectedIndex = 0;
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

			if (ys::InputManager::getKeyDown(VK_RETURN))
			{
				if (isVal())
				{

				}
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
		else
		{//도형모드
			if (ys::InputManager::getKeyDown(VK_LEFT))
			{

			}
			if (ys::InputManager::getKeyDown(VK_RIGHT))
			{

			}
			if (ys::InputManager::getKeyDown(VK_UP))
			{

			}
			if (ys::InputManager::getKeyDown(VK_DOWN))
			{

			}

			if (ys::InputManager::getKeyDown(VK_ADD))
			{//도형 테두리 크기 +변환

			}
			if (ys::InputManager::getKeyDown(VK_SUBTRACT))
			{//도형 테두리 크기 -변환

			}
		}
	}

	void InputShape::render(HDC hDC)
	{
		Rectangle(hDC, textBox.left, textBox.top, textBox.right, textBox.bottom);
		if (nextCharIndex == 0)
			SetCaretPos(textBox.left + 10, textBox.top + 10);
		else
		{
			SIZE size;  GetTextExtentPoint32(hDC, stringBuff.substr(0, nextCharIndex).c_str(), stringBuff.substr(0, nextCharIndex).size(), &size);
			SetCaretPos(textBox.left + 10 + size.cx, textBox.top + 10);
		}
		TextOut(hDC, textBox.left + 10, textBox.top + 10, stringBuff.c_str(), stringBuff.size());
		TextOut(hDC, 100, 200, std::to_wstring(nextCharIndex).c_str(), std::to_wstring(nextCharIndex).size());
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

	void InputShape::renderObject(HDC hDC, Object object)
	{
	}

	bool InputShape::isVal()
	{
		return false;
	}
}