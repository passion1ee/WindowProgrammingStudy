#include "TextEditor.h"
#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#include <tchar.h>
#include <cwctype>

namespace ys
{
	constexpr BYTE kLineMaxSize = 80;
	constexpr BYTE kMaxLine = 10;
	constexpr BYTE kLineHeight = 20;
	std::vector<std::wstring> TextEditor::note;
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
			enter(nextCharIndex);
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

	}



	void TextEditor::Add(HDC hDC, WPARAM buff)
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

/*
이스케이프키 (esc): 화면이 다 지워지고 캐럿은 맨 윗줄 앞에 있다..
- 탭키 (tab): 5개의 스페이스가 삽입되고 캐럿도 5개 스페이스 뒤로 이동한다.
- 화살표 키: 캐럿이 현재 위치에서 문자 기준으로 좌/우/상/하로 이동한다. 캐럿을 이동한 후 문자를 입력하면 그 위치에 입력된다. 문자
덮어쓰기가 디폴트이다. 한 줄은 최대 30자 까지만 입력된다.
- F1 키: 입력하는 문자가 대문자로 출력된다. 다시 누르면 소문자로 출력된다.
- Del 키: 현재 캐럿이 놓인 단어가 (스페이스로 구분) 삭제되며 뒤의 문자들이 앞으로 나온다. 캐럿의 위치는 바뀌지 않는다. 마지막 문자
였고 그 단어가 삭제되었다면 캐럿은 새로운 마지막 문자의 맨 뒤로 이동한다.
- Home 키: 캐럿이 그 줄의 맨 앞에 온다. 홈 키로 캐럿을 이동 후 문자를 입력하면 문자가 입력된다. 문자 덮어쓰기가 디폴트이다.
- End 키: 캐럿이 놓인 그 줄의 맨 뒤로 캐럿이 움직인다. 움직인 위치에 새로운 문자를 넣을 수 있다. 80자가 다 찼다면 캐럿은 다음 줄로
이동하고 문자가 출력된다 (덮어쓰기 혹은 삽입은 선택 가능)
- Insert 키: 토글키로 insert키를 누르면 현재 캐럿 위치에서 문자가 추가되고, 다시 누르면 문자를 덮어쓴다.
*/

/*
A + 0 = A = 0 + A
(A + 0) = A
이때 행렬이 같기 위한 조건은 크기가 같고, 모든 원소가 같아야한다.
A의 크기가 m * n라고 할때, 행렬의 덧셈을 하기 위해선 두 행렬의 크기가 같아야한다.
따라서 0행렬의 크기도 m*n이다. 이때 더해서 나오는 행렬은 같은 크기이므로
(A + 0)의 크기는 m*n이고 A의 크기도 m*n이므로 양변의 행렬은 크기가 같음을 알 수 있다.
(A + 0).ij = A.ij
([a].ij + [0]ij) = (a.11 + 0.11) + (a.1
= [a].ij
A.ij = [a].ij
양변의 모든 원소는 같다.
따라서 양변은 크기와 모든 원소가 같으므로 등식이 성립한다.

AI = A = IA
A의 크기를 m*n이라 할때 I는 행렬의 곱의 정의에 의해 n*n이여야 성립한다. 따라서 I의 크기는 n*n이다.
(A.mn)(I.nn) = (A.mn) = (I.nn)(A.mn)
이때 IA는 행렬의 곱의 정의에 의해 성립하려면 n = m 을 만족해야한다. 따라서 n = m같다
식을 다시 적어보면 (A.nn)(I.nn) = (A.nn) = (I.nn)(A.nn)으로 모두 정방행렬이다.
(AI)nn = (A)nn = (IA)nn이므로 모두 nn의 크기를 가진 행렬으로 크기가 같다.

(A.ik)(I.kj) = [a.ik][I.kj] = (a.i1)(I.1j) + (a.i2)(I.2j) + ... + (a.ik)(I.kj)
이때 I는 단위행렬이므로 주대각선 상의 원소는 1이고 나머지는 0이다. 이때 j = k일때 주대각선의 원소이다.
나머지는 0이므로 모두 0이 나오고 (a.11 I.11) + (a.12 I.22) + ... (a.nn I.nn)이고 이 I의 원소는 1이므로
								= a.11 + a.12 + ... + a.nn = [a.nn] = (A)nn이 나온다.
정리하면 (A.nn)(I.nn) = (A.nn)가 된다는 것을 보였다.
따라서 AI = A이다.

(I.nn)(A.nn) = [I.ik][a.kj] = (I.i1)(a.1j) + (I.i2)(a.2j) + ... + (I.ik)(a.kj)
이때 I는 단위행렬이므로 주대각선 상의 원소는 1이고 나머지는 0이다. 이때 i = k일때 주대각선의 원소이다.
나머지는 0이므로 모두 0이 나오고 (I.11 a.11) + (I.12 a.22) + ... (I.nn a.nn)이고 이 I의 원소는 1이므로
								= a.11 + a.12 + ... + a.nn = [a.nn] = (A)nn이 나온다.
정리하면 (I.nn)(A.nn) = (A.nn)가 된다는 것을 보였다.
따라서 A = IA이다.
앞의 정의에 의해서 AI = A = IA임을 알 수 있다.

*/