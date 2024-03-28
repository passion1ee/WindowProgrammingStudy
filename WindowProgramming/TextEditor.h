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
		static void Run(HWND hWnd);
		static void Update(HWND hWnd);
		static void Add(HDC hDC, std::wstring buff);
		static void Render(HDC hDC);

	private:
		static std::vector<std::wstring> prevText;
		static std::vector<std::wstring> curText;
		static std::vector<std::wstring> text;
		static short curLine;
		static bool maxLine;
	};
}

