#pragma once
#include <windows.h>
#include <vector>
#include "../../WinProgramming/MyEngine_source/ysInputManager.h"

namespace ys
{

	class WordPuzzle
	{
	public:
		struct Object
		{
			int id;
			wchar_t alphabet;
			POINT position;

			Object() = delete;
			Object(wchar_t alphabet_, POINT _position, int _id)
				: alphabet(alphabet_), position(_position), id(_id) {}

			bool operator==(const Object& other)
			{
				if (id != other.id)
					return false;
				if (alphabet != other.alphabet)
					return false;
				if (position.x != other.position.x || position.y != other.position.y)
					return false;
				return true;
			}
		};

	public:
		static void setScreen(int width, int height);

		static void Init();
		static void Run(HWND hWnd);
		static void render(HDC hDC);

	private:
		static void Update(HWND hWnd);
		static void renderFrame(HDC hDC);
		static void renderObject(HDC hDC, Object object);

		static bool isWord();
		static void victorySound();
	private:
		static int screenWidth;
		static int screenHeight;

		static BYTE mapSize;
		static double xGrid;
		static double yGrid;

		static POINT player;
		static std::vector<Object> alphabets;
		static std::vector<std::vector<std::wstring>> plain;
		static std::wstring goalWord;
	};
}

