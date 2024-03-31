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
			if (nextCharIndex == note.at(curLine).size())//���������� �϶�
			{
				if (curLine != note.size() - 1)// ���������� �ƴҶ�
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
			else//���������ڰ� �ƴҶ�
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
		else //!isInsert && ���������ڰ� �ƴҶ�
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
		if (prevIndex < note.at(curLine - 1).size())//���ڿ� �߰��ΰ�
		{
			note.at(curLine).insert(note.at(curLine).begin(), note.at(curLine - 1).begin() + prevIndex, note.at(curLine - 1).end());
			note.at(curLine - 1).erase(note.at(curLine - 1).begin() + prevIndex, note.at(curLine - 1).end());
		}
	}
}

/*
�̽�������Ű (esc): ȭ���� �� �������� ĳ���� �� ���� �տ� �ִ�..
- ��Ű (tab): 5���� �����̽��� ���Եǰ� ĳ���� 5�� �����̽� �ڷ� �̵��Ѵ�.
- ȭ��ǥ Ű: ĳ���� ���� ��ġ���� ���� �������� ��/��/��/�Ϸ� �̵��Ѵ�. ĳ���� �̵��� �� ���ڸ� �Է��ϸ� �� ��ġ�� �Էµȴ�. ����
����Ⱑ ����Ʈ�̴�. �� ���� �ִ� 30�� ������ �Էµȴ�.
- F1 Ű: �Է��ϴ� ���ڰ� �빮�ڷ� ��µȴ�. �ٽ� ������ �ҹ��ڷ� ��µȴ�.
- Del Ű: ���� ĳ���� ���� �ܾ (�����̽��� ����) �����Ǹ� ���� ���ڵ��� ������ ���´�. ĳ���� ��ġ�� �ٲ��� �ʴ´�. ������ ����
���� �� �ܾ �����Ǿ��ٸ� ĳ���� ���ο� ������ ������ �� �ڷ� �̵��Ѵ�.
- Home Ű: ĳ���� �� ���� �� �տ� �´�. Ȩ Ű�� ĳ���� �̵� �� ���ڸ� �Է��ϸ� ���ڰ� �Էµȴ�. ���� ����Ⱑ ����Ʈ�̴�.
- End Ű: ĳ���� ���� �� ���� �� �ڷ� ĳ���� �����δ�. ������ ��ġ�� ���ο� ���ڸ� ���� �� �ִ�. 80�ڰ� �� á�ٸ� ĳ���� ���� �ٷ�
�̵��ϰ� ���ڰ� ��µȴ� (����� Ȥ�� ������ ���� ����)
- Insert Ű: ���Ű�� insertŰ�� ������ ���� ĳ�� ��ġ���� ���ڰ� �߰��ǰ�, �ٽ� ������ ���ڸ� �����.
*/

/*
A + 0 = A = 0 + A
(A + 0) = A
�̶� ����� ���� ���� ������ ũ�Ⱑ ����, ��� ���Ұ� ���ƾ��Ѵ�.
A�� ũ�Ⱑ m * n��� �Ҷ�, ����� ������ �ϱ� ���ؼ� �� ����� ũ�Ⱑ ���ƾ��Ѵ�.
���� 0����� ũ�⵵ m*n�̴�. �̶� ���ؼ� ������ ����� ���� ũ���̹Ƿ�
(A + 0)�� ũ��� m*n�̰� A�� ũ�⵵ m*n�̹Ƿ� �纯�� ����� ũ�Ⱑ ������ �� �� �ִ�.
(A + 0).ij = A.ij
([a].ij + [0]ij) = (a.11 + 0.11) + (a.1
= [a].ij
A.ij = [a].ij
�纯�� ��� ���Ҵ� ����.
���� �纯�� ũ��� ��� ���Ұ� �����Ƿ� ����� �����Ѵ�.

AI = A = IA
A�� ũ�⸦ m*n�̶� �Ҷ� I�� ����� ���� ���ǿ� ���� n*n�̿��� �����Ѵ�. ���� I�� ũ��� n*n�̴�.
(A.mn)(I.nn) = (A.mn) = (I.nn)(A.mn)
�̶� IA�� ����� ���� ���ǿ� ���� �����Ϸ��� n = m �� �����ؾ��Ѵ�. ���� n = m����
���� �ٽ� ����� (A.nn)(I.nn) = (A.nn) = (I.nn)(A.nn)���� ��� ��������̴�.
(AI)nn = (A)nn = (IA)nn�̹Ƿ� ��� nn�� ũ�⸦ ���� ������� ũ�Ⱑ ����.

(A.ik)(I.kj) = [a.ik][I.kj] = (a.i1)(I.1j) + (a.i2)(I.2j) + ... + (a.ik)(I.kj)
�̶� I�� ��������̹Ƿ� �ִ밢�� ���� ���Ҵ� 1�̰� �������� 0�̴�. �̶� j = k�϶� �ִ밢���� �����̴�.
�������� 0�̹Ƿ� ��� 0�� ������ (a.11 I.11) + (a.12 I.22) + ... (a.nn I.nn)�̰� �� I�� ���Ҵ� 1�̹Ƿ�
								= a.11 + a.12 + ... + a.nn = [a.nn] = (A)nn�� ���´�.
�����ϸ� (A.nn)(I.nn) = (A.nn)�� �ȴٴ� ���� ������.
���� AI = A�̴�.

(I.nn)(A.nn) = [I.ik][a.kj] = (I.i1)(a.1j) + (I.i2)(a.2j) + ... + (I.ik)(a.kj)
�̶� I�� ��������̹Ƿ� �ִ밢�� ���� ���Ҵ� 1�̰� �������� 0�̴�. �̶� i = k�϶� �ִ밢���� �����̴�.
�������� 0�̹Ƿ� ��� 0�� ������ (I.11 a.11) + (I.12 a.22) + ... (I.nn a.nn)�̰� �� I�� ���Ҵ� 1�̹Ƿ�
								= a.11 + a.12 + ... + a.nn = [a.nn] = (A)nn�� ���´�.
�����ϸ� (I.nn)(A.nn) = (A.nn)�� �ȴٴ� ���� ������.
���� A = IA�̴�.
���� ���ǿ� ���ؼ� AI = A = IA���� �� �� �ִ�.

*/