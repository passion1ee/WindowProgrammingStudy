#include "TextEditor.h"
#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#include <tchar.h>
#include <cwctype>

namespace ys
{
	constexpr BYTE kLineMaxSize = 80;
	constexpr BYTE kMaxLine = 35;
	constexpr BYTE kLineHeight = 20;
	std::vector<std::wstring> TextEditor::note;
	std::vector<std::wstring> TextEditor::pdownPrevNote;
	std::vector<std::wstring> TextEditor::pupPrevNote;
	short TextEditor::curLine;
	size_t TextEditor::nextCharIndex;
	bool TextEditor::isInsert;
	bool TextEditor::isUpper;

	void TextEditor::Init()
	{
		note.push_back(L"");
		curLine = 0;
		nextCharIndex = 0;
		isInsert = false;
		isUpper = false;
	}

	void TextEditor::Run()
	{
		InputManager::BeforeUpdate();
		Update();
		InputManager::AfterUpdate();
	}

	void TextEditor::Update()
	{
		
		if (ys::InputManager::getKeyDown(VK_LEFT) /*|| ys::InputManager::getKey(VK_LEFT)*/)
		{
			if(nextCharIndex > 0)
				--nextCharIndex;
			else
			{
				if (curLine > 0)
				{
					--curLine;
					nextCharIndex = note.at(curLine).size();
				}
			}
		}
		if (ys::InputManager::getKeyDown(VK_RIGHT) /*|| ys::InputManager::getKey(VK_RIGHT)*/)
		{
			if (nextCharIndex < note.at(curLine).size())
				++nextCharIndex;
			else
			{
				if (curLine < note.size() - 1)
				{
					++curLine;
					nextCharIndex = 0;
				}
			}
		}
		if (ys::InputManager::getKeyDown(VK_UP) /*|| ys::InputManager::getKey(VK_UP)*/)
		{
			if (curLine > 0)
			{
				--curLine;
				if(nextCharIndex > note.at(curLine).size())
					nextCharIndex = note.at(curLine).size();
			}
		}
		if (ys::InputManager::getKeyDown(VK_DOWN) /*|| ys::InputManager::getKey(VK_DOWN)*/)
		{
			if (curLine < note.size() - 1)
			{
				++curLine;
				if(nextCharIndex > note.at(curLine).size())
					nextCharIndex = note.at(curLine).size();
			}
		}


		if (ys::InputManager::getKeyDown(VK_RETURN))
		{
			if (curLine < kMaxLine)
				enter(nextCharIndex);
			else
			{
				curLine = 0;
				nextCharIndex = 0;
			}
		}
		if (ys::InputManager::getKeyDown(VK_TAB))
		{
			if (note.at(curLine).size() < kLineMaxSize - 4)
			{
				note.at(curLine).insert(nextCharIndex, L"     ");
				nextCharIndex += 5;
			}
		}


		if (ys::InputManager::getKeyDown(VK_BACK))
		{
			if (note.at(curLine).empty() || nextCharIndex == 0)
			{
				if (curLine != 0 || nextCharIndex != 0)
				{
					size_t tmp = note.at(curLine - 1).size();
					nextCharIndex = tmp;
					note.at(curLine - 1).append(note.at(curLine).c_str());
					note.erase(note.begin() + curLine--);
					if (note.at(curLine).size() > kLineMaxSize)
					{
						enter(kLineMaxSize);
						curLine--;
						nextCharIndex = tmp;
					}
				}
			}
			else if (nextCharIndex > 0)
			{
				note.at(curLine).erase(note.at(curLine).begin() + --nextCharIndex);
			}
		}
		if (ys::InputManager::getKeyDown(VK_DELETE))
		{
			if (nextCharIndex == note.at(curLine).size())//마지막문자 일때
			{
				if (curLine != note.size() - 1)// 마지막줄이 아닐때
				{
					size_t tmp = note.at(curLine).size();
					note.at(curLine).append(note.at(curLine + 1).c_str());
					note.erase(note.begin() + curLine + 1);
					if (note.at(curLine).size() > kLineMaxSize)
					{
						enter(kLineMaxSize);
						curLine--;
						nextCharIndex = tmp;
					}
				}
			}
			else//마지막문자가 아닐때
			{
				note.at(curLine).erase(note.at(curLine).begin() + nextCharIndex);
			}
		}
		if (ys::InputManager::getKeyDown(VK_ESCAPE))
		{
			note.clear();
			note.push_back(L"");
			curLine = 0;
			nextCharIndex = 0;
		}


		if (ys::InputManager::getKeyDown(VK_HOME))
		{
			nextCharIndex = 0;
		}
		if (ys::InputManager::getKeyDown(VK_END))
		{
			nextCharIndex = note.at(curLine).size();
			if (nextCharIndex >= kLineMaxSize)
			{
				nextCharIndex = 0;
				++curLine;
				if(curLine >= note.size())
					note.insert(note.begin() + curLine, L"");
			}
		}
		if (ys::InputManager::getKeyDown(VK_F1))
		{
			isUpper = isUpper ? false : true;
		}
		if (ys::InputManager::getKeyDown(VK_INSERT))
		{
			isInsert = isInsert ? false : true;
		}


		if (ys::InputManager::getKeyDown(VK_F2))
		{
			bool isPrevPrint{ false };
			for (int i = 0; i < note.size(); ++i)
			{
				auto tmp = note.at(i).find(L"____");
				if (tmp != std::string::npos)
				{
					note.at(i).erase(tmp, 4);
					isPrevPrint = true;
				}
			}

			if (isPrevPrint)
			{
				nextCharIndex = nextCharIndex > 4 ? nextCharIndex - 4 : 0;
			}
			else
			{
				for (int index = 0; index < note.size(); ++index)
				{
					note.at(index).insert(0, L"____");
				}
				nextCharIndex += 4;
			}
		}
		if (ys::InputManager::getKeyDown(VK_F3))
		{
			nextCharIndex = 0;
			if(note.size() <= kMaxLine)
			{
				++curLine;
				note.insert(note.begin(), L"");
			}
			else
			{
				note.pop_back();
				note.insert(note.begin(), L"");
			}
		}
		if (ys::InputManager::getKeyDown(VK_F4))
		{
			std::reverse(note.begin(), note.end());
			std::reverse(pdownPrevNote.begin(), pdownPrevNote.end());
			std::reverse(pupPrevNote.begin(), pupPrevNote.end());
			curLine = note.size() - 1 - curLine;
		}
		if (ys::InputManager::getKeyDown(VK_NEXT))
		{
			if (pupPrevNote.empty())
			{
				if (pdownPrevNote.empty())
				{
					pdownPrevNote = note;
					for (auto& line : note)
					{
						std::wstring word;
						std::wstring result;
						bool inWord{ false };

						for (auto& ch : line)
						{
							if (iswspace(ch))
							{
								if (inWord)
								{
									result += L'(' + word + L')';
									word.clear();
									inWord = false;
								}
							}
							else
							{
								word += towupper(ch);
								inWord = true;
							}
						}

						// 마지막 단어 처리
						if (!word.empty())
							result += L'(' + word + L')';

						line = result;
					}
				}
				else
				{
					note = pdownPrevNote;
					pdownPrevNote.clear();
				}
			}
			
		}
		if (ys::InputManager::getKeyDown(VK_PRIOR))
		{
			if (pdownPrevNote.empty())
			{
				if (pupPrevNote.empty())
				{
					pupPrevNote = note;
					for (auto& line : note)
					{
						line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
					}
				}
				else
				{
					note = pupPrevNote;
					pupPrevNote.clear();
				}
			}
		}
		if (ys::InputManager::getKeyDown(VK_ADD))
		{
			if (pdownPrevNote.empty() && pupPrevNote.empty())
			{
				for (auto& line : note)
				{
					for (auto& ch : line)
					{
						if (iswalpha(ch))
						{
							if (ch == L'z') ch = L'a';
							else if (ch == L'Z') ch = L'A';
							else ++ch;
						}
						else if (iswdigit(ch))
						{
							if (ch == L'9') ch = L'0';
							else ++ch;
						}
					}
				}
			}
		}
		if (ys::InputManager::getKeyDown(VK_SUBTRACT))
		{
			if (pdownPrevNote.empty() && pupPrevNote.empty())
			{
				for (auto& line : note)
				{
					for (auto& ch : line)
					{
						if (iswalpha(ch))
						{
							if (ch == L'a') ch = L'z';
							else if (ch == L'A') ch = L'Z';
							else --ch;
						}
						else if (iswdigit(ch))
						{
							if (ch == L'0') ch = L'9';
							else --ch;
						}
					}
				}
			}
		}

	}



	void TextEditor::Add(HDC hDC, WPARAM buff)
	{
		if(pdownPrevNote.empty() && pupPrevNote.empty())
		{
			std::wstring ch;

			if (isUpper)
				ch = std::towupper(buff);
			else
				ch = std::towlower(buff);


			if (isInsert || nextCharIndex == note.at(curLine).size())
			{
				if (note.at(curLine).size() < kLineMaxSize)
				{
					note.at(curLine).insert(nextCharIndex, ch);
					++nextCharIndex;
				}
				else
				{
					enter(nextCharIndex);
				}
			}
			else //!isInsert && 마지막문자가 아닐때
			{
				note.at(curLine).erase(note.at(curLine).begin() + nextCharIndex);
				note.at(curLine).insert(nextCharIndex, ch);
				++nextCharIndex;
			}
		}
	}



	void TextEditor::Render(HDC hDC)
	{
		for (int i = 0; i < note.size(); ++i)
			TextOut(hDC, 0, kLineHeight * i, note.at(i).c_str(), note.at(i).size());

		const auto& caretPrevString = note.at(curLine).substr(0, nextCharIndex);

		if (note.at(curLine).empty())
			SetCaretPos(0, kLineHeight * curLine);
		else
		{
			if (nextCharIndex == 0)
				SetCaretPos(0, kLineHeight * curLine);
			else
			{
				SIZE size;  GetTextExtentPoint32(hDC, note.at(curLine).substr(0, nextCharIndex).c_str(), note.at(curLine).substr(0, nextCharIndex).size(), &size);
				SetCaretPos(size.cx, kLineHeight * curLine);
			}
		}
		std::wstring tmp = L"line: " + std::to_wstring(curLine) + L"    Col: " + std::to_wstring(nextCharIndex);
		TextOut(hDC, 1300, 500, tmp.c_str(), tmp.size());
	}


	void TextEditor::enter(const size_t prevIndex)
	{
		nextCharIndex = 0;
		++curLine;
		note.insert(note.begin() + curLine, L"");
		if (prevIndex < note.at(curLine - 1).size())//문자열 중간인가
		{
			note.at(curLine).insert(note.at(curLine).begin(), note.at(curLine - 1).begin() + prevIndex, note.at(curLine - 1).end());
			note.at(curLine - 1).erase(note.at(curLine - 1).begin() + prevIndex, note.at(curLine - 1).end());
		}
	}
}