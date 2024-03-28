#include "TextEditor.h"
#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#include <tchar.h>

namespace ys
{
	constexpr BYTE kLineMaxSize = 80;
	std::vector<std::wstring> TextEditor::prevText;
	std::vector<std::wstring> TextEditor::curText;
	std::vector<std::wstring> TextEditor::text;
	short TextEditor::curLine;
	bool TextEditor::maxLine;
	void TextEditor::Init()
	{
		text.push_back(L"");
		curLine = 0;
		maxLine = false;
	}

	void TextEditor::Run(HWND hWnd)
	{
		InputManager::BeforeUpdate();
		Update(hWnd);
		InputManager::AfterUpdate();
	}

	void TextEditor::Update(HWND hWnd)
	{
		HDC hDC = GetDC(hWnd);
		
		if (ys::InputManager::getKeyDown(VK_RETURN))
		{
			//curLine < size 실패시 늘리고 바로 그곳으로 이동
			//
			//@@이번 문제에서만@@ 만약 현재 10번째 원소이면 엔터키 입력 후 1번 원소로 돌아가는 원형으로 제작
			++curLine;
			if (curLine < text.size())
			{
				SIZE size;  GetTextExtentPoint32(hDC, text.at(curLine).c_str(), text.at(curLine).size(), &size);
				SetCaretPos(0, 20 * curLine);
			}
			else
			{
				if (text.size() >= 10)//-----------------
				{
					maxLine = true;
					prevText = text;
					curLine = 0;
					SIZE size;  GetTextExtentPoint32(hDC, text.at(curLine).c_str(), text.at(curLine).size(), &size);
					SetCaretPos(0, 20 * curLine);
				}//--------------------------------------
				else
				{
					text.push_back(L"");
					SetCaretPos(0, 20 * curLine);
				}
			}
		}

		if (ys::InputManager::getKeyDown(VK_BACK))
		{
			//길이가 0인 wstring원소가 Back을 받으면 그 원소 삭제
			if (text.at(curLine).empty())
			{
				if (curLine != 0)
				{
					text.erase(text.begin() + curLine);
					--curLine;
					maxLine = false;
					SIZE size;  GetTextExtentPoint32(hDC, text.at(curLine).c_str(), text.at(curLine).size(), &size);
					SetCaretPos(size.cx, 20 * curLine);
				}
			}
			else
			{
				text.at(curLine) = text.at(curLine).erase(text.at(curLine).size() - 1);
				SIZE size;  GetTextExtentPoint32(hDC, text.at(curLine).c_str(), text.at(curLine).size(), &size);
				SetCaretPos(size.cx, 20 * curLine);
			}
		}

		if (ys::InputManager::getKeyDown(VK_ESCAPE))
		{
			text.clear();
			prevText.clear();
			Init();
		}

		ReleaseDC(hWnd, hDC);
	}

	void TextEditor::Add(HDC hDC, std::wstring buff)
	{
		POINT caretPosition; GetCaretPos(&caretPosition);
		SIZE piece; GetTextExtentPoint32(hDC, buff.c_str(), buff.size(), &piece);

		if (maxLine)
		{
			text.at(curLine).erase((text.at(curLine).rbegin() + prevText.at(curLine).size()).base(), text.at(curLine).end());
			
			if (!prevText.at(curLine).empty() && prevText.at(curLine).size() + text.at(curLine).size() >= kLineMaxSize)
				prevText.at(curLine).erase(prevText.at(curLine).size() - 1);

			if (prevText.at(curLine).size() + text.at(curLine).size() < kLineMaxSize)
			{
				text.at(curLine) += buff;
				text.at(curLine) += prevText.at(curLine);
				POINT caretPosition; GetCaretPos(&caretPosition);
				SIZE frontPiece; GetTextExtentPoint32(hDC, buff.c_str(), buff.size(), &frontPiece);
				SetCaretPos(caretPosition.x + frontPiece.cx, 20 * curLine);
			}
		}
		else
		{
			if (text.at(curLine).size() < kLineMaxSize)
			{
				text.at(curLine) += buff;
				SetCaretPos(caretPosition.x + piece.cx, 20 * curLine);
			}
		}
	}

	void TextEditor::Render(HDC hDC)
	{
		for (int i = 0; i < text.size(); ++i)
		{
			TextOut(hDC, 0, 20 * i, text.at(i).c_str(), text.at(i).size());
		}
	}
}