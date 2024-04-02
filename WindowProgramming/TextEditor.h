#pragma once

#include <vector>
#include <Windows.h>
#include <string>
namespace ys
{
	class TextEditor
	{
	public:
		static void Init();
		static void Run();
		static void Update();
		static void Add(WPARAM buff);
		static void Render(HDC hDC);

	private:
		static std::vector<std::wstring> pdownPrevNote;
		static std::vector<std::wstring> pupPrevNote;
		static void enter(const size_t);


	private:
		static std::vector<std::wstring> note;
		static short curLine;
		static size_t nextCharIndex;
		static bool isInsert;
		static bool isUpper;
	};
}