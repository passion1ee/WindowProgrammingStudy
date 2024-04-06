#include "ysDigitAlphaPlus.h"
#include "stdfax.h"

namespace ys
{
	int DigitAlphaPlus::screenWidth;
	int DigitAlphaPlus::screenHeight;

	BYTE DigitAlphaPlus::mapSize;
	double DigitAlphaPlus::xGrid;
	double DigitAlphaPlus::yGrid;

	POINT DigitAlphaPlus::player;
	std::vector<DigitAlphaPlus::Object> DigitAlphaPlus::alphabets;
	std::vector<std::vector<std::wstring>> DigitAlphaPlus::plain;
	std::wstring DigitAlphaPlus::goalWord;

	void DigitAlphaPlus::setScreen(int width, int height)
	{
		screenWidth = width;
		screenHeight = height;
		xGrid = screenWidth / static_cast<double>(mapSize);
		yGrid = screenHeight / static_cast<double>(mapSize);
	}

	void DigitAlphaPlus::Init()
	{
		alphabets.clear();
		plain.clear();
		mapSize = 20;
		std::vector<std::wstring> allWord = { L"APPLE", L"DEBUG", L"PROGRAMMING", L"WINDOWS", L"HETEROGENEOUS" };
		std::uniform_int_distribution<> selectWord(0, allWord.size() - 1);
		goalWord = allWord[selectWord(randomEngine)];

		int id{};
		int allWordSize{};
		for (auto& word : allWord)
			allWordSize += word.size();
		alphabets.reserve(26 + allWordSize);
		plain.assign(mapSize, std::vector<std::wstring>(mapSize, L""));

		std::uniform_int_distribution<> position(0, mapSize - 1);
		for (auto i = L'A'; i <= L'Z';)
		{
			auto x = position(randomEngine); auto y = position(randomEngine);
			if (plain[y][x] != L"" || y == 0 || y == 1)
				continue;

			plain[y][x] = i;
			alphabets.push_back(Object(i, POINT(x, y), id++));
			++i;
		}

		for (auto& word : allWord)
		{
			if (word == goalWord)
				continue;
			for (auto i = 0; i < word.size();)
			{
				auto x = position(randomEngine); auto y = position(randomEngine);
				if (plain[y][x] != L"" || y == 0 || y == 1)
					continue;

				plain[y][x] = word[i];
				alphabets.push_back(Object(word[i], POINT(x, y), id++));
				++i;
			}
		}

		auto wordStart = (mapSize - (goalWord.size())) / 2;
		std::uniform_int_distribution<> isRandomPos(0, goalWord.size());
		for (auto& piece : goalWord)
		{
			if (isRandomPos(randomEngine) < goalWord.size() / 2)
			{
				int x, y;
				do {
					x = position(randomEngine); y = position(randomEngine);
				} while (plain[y][x] != L"" || y == 0 || y == 1);
				plain[y][x] = piece;
				alphabets.push_back(Object(piece, POINT(x, y), id++));
			}
			else
			{
				plain[1][wordStart] = piece;
				alphabets.push_back(Object(piece, POINT(wordStart, 1), id++));
			}
			wordStart++;
		}

		int x, y;
		do {
			x = position(randomEngine);
		} while (plain[mapSize - 1][x] != L"");
		player.x = x; player.y = mapSize - 1;
	}

	void DigitAlphaPlus::Run(HWND hWnd)
	{
		InputManager::BeforeUpdate();
		Update(hWnd);
		InputManager::AfterUpdate();
	}

	void DigitAlphaPlus::render(HDC hDC)
	{
		renderFrame(hDC);
		for (const auto& alphabet : alphabets)
			renderObject(hDC, alphabet);
		Ellipse(hDC, player.x * xGrid, player.y * yGrid, (player.x + 1) * xGrid, (player.y + 1) * yGrid);
	}

	void DigitAlphaPlus::Update(HWND hWnd)
	{

		if (InputManager::getKeyDown(VK_LEFT))
		{
			if (player.x == 0)
				player.x = mapSize - 1;
			else
				player.x--;
			auto iter = std::find_if(alphabets.begin(), alphabets.end(), [](const Object& other) {
				return player.x == other.position.x && player.y == other.position.y; });
			if (alphabets.end() != iter)//플레이어의 위치와 동일한 블럭이 있는가
			{
				if (iter->position.x == 0)
					iter->position.x = mapSize - 1;
				else
					iter->position.x--;

				auto otherIter = std::find_if(alphabets.begin(), alphabets.end(), [iter](const Object& other) {
					return iter->position.x == other.position.x &&
						iter->position.y == other.position.y &&
						other != *iter; });

				if (alphabets.end() != otherIter)//블럭의 이동 후 위치와 동일한 블럭이 있는가
				{
					if (player.x == mapSize - 1)
						player.x = 0;
					else
						player.x++;
					if (iter->position.x == mapSize - 1)
						iter->position.x = 0;
					else
						iter->position.x++;
				}
				else
				{
					plain[iter->position.y][iter->position.x] = plain[player.y][player.x];
					plain[player.y][player.x] = L"";
					Beep(1760, 40);
					Beep(2218, 80);
				}
			}
		}
		if (InputManager::getKeyDown(VK_RIGHT))
		{
			if (player.x == mapSize - 1)
				player.x = 0;
			else
				player.x++;
			auto iter = std::find_if(alphabets.begin(), alphabets.end(), [](const Object& other) {
				return player.x == other.position.x && player.y == other.position.y; });
			if (alphabets.end() != iter)//플레이어의 위치와 동일한 블럭이 있는가
			{
				if (iter->position.x == mapSize - 1)
					iter->position.x = 0;
				else
					iter->position.x++;
				auto otherIter = std::find_if(alphabets.begin(), alphabets.end(), [iter](const Object& other) {
					return iter->position.x == other.position.x &&
						iter->position.y == other.position.y &&
						other != *iter; });
				if (alphabets.end() != otherIter)//블럭의 이동 후 위치와 동일한 블럭이 있는가
				{
					if (player.x == 0)
						player.x = mapSize - 1;
					else
						player.x--;
					if (iter->position.x == 0)
						iter->position.x = mapSize - 1;
					else
						iter->position.x--;
				}
				else
				{
					plain[iter->position.y][iter->position.x] = plain[player.y][player.x];
					plain[player.y][player.x] = L"";
					Beep(1760, 40);
					Beep(2218, 80);
				}
			}
		}
		if (InputManager::getKeyDown(VK_UP))
		{
			if (player.y == 0)
				player.y = mapSize - 1;
			else
				player.y--;
			auto iter = std::find_if(alphabets.begin(), alphabets.end(), [](const Object& other) {
				return player.x == other.position.x && player.y == other.position.y; });
			if (alphabets.end() != iter)//플레이어의 위치와 동일한 블럭이 있는가
			{
				if (iter->position.y == 0)
					iter->position.y = mapSize - 1;
				else
					iter->position.y--;
				auto otherIter = std::find_if(alphabets.begin(), alphabets.end(), [iter](const Object& other) {
					return iter->position.x == other.position.x &&
						iter->position.y == other.position.y &&
						other != *iter; });
				if (alphabets.end() != otherIter)//블럭의 이동 후 위치와 동일한 블럭이 있는가
				{
					if (player.y == mapSize - 1)
						player.y = 0;
					else
						player.y++;
					if (iter->position.y == mapSize - 1)
						iter->position.y = 0;
					else
						iter->position.y++;
				}
				else
				{
					plain[iter->position.y][iter->position.x] = plain[player.y][player.x];
					plain[player.y][player.x] = L"";
					Beep(1760, 40);
					Beep(2218, 80);
				}
			}
		}
		if (InputManager::getKeyDown(VK_DOWN))
		{
			if (player.y == mapSize - 1)
				player.y = 0;
			else
				player.y++;
			auto iter = std::find_if(alphabets.begin(), alphabets.end(), [](const Object& other) {
				return player.x == other.position.x && player.y == other.position.y; });
			if (alphabets.end() != iter)//플레이어의 위치와 동일한 블럭이 있는가
			{
				if (iter->position.y == mapSize - 1)
					iter->position.y = 0;
				else
					iter->position.y++;
				auto otherIter = std::find_if(alphabets.begin(), alphabets.end(), [iter](const Object& other) {
					return iter->position.x == other.position.x &&
						iter->position.y == other.position.y &&
						other != *iter; });
				if (alphabets.end() != otherIter)//블럭의 이동 후 위치와 동일한 블럭이 있는가
				{
					if (player.y == 0)
						player.y = mapSize - 1;
					else
						player.y--;
					if (iter->position.y == 0)
						iter->position.y = mapSize - 1;
					else
						iter->position.y--;
				}
				else
				{
					plain[iter->position.y][iter->position.x] = plain[player.y][player.x];
					plain[player.y][player.x] = L"";
					Beep(1760, 40);
					Beep(2218, 80);
				}
			}
		}

		if (InputManager::getKeyDown((UINT)Key::S))
		{
			Init();
		}
		if (InputManager::getKeyUp((UINT)Key::Q))
		{
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}

	}

	void DigitAlphaPlus::renderFrame(HDC hDC)
	{
		for (int i = 0; i < mapSize; ++i)
		{
			MoveToEx(hDC, 0, i * yGrid, NULL);
			LineTo(hDC, screenWidth, i * yGrid);
		}
		for (int i = 0; i < mapSize; ++i)
		{
			MoveToEx(hDC, i * xGrid, 0, NULL);
			LineTo(hDC, i * xGrid, screenHeight);
		}
	}
	void DigitAlphaPlus::renderObject(HDC hDC, Object object)
	{
		auto wordStart = (mapSize - (goalWord.size())) / 2;
		if (object.position.y == 1 && (object.position.x >= wordStart && object.position.x <= wordStart + goalWord.size()))
		{
			SetBkColor(hDC, RGB(125, 125, 255));
			TextOut(hDC, object.position.x * xGrid + xGrid / 2, object.position.y * yGrid + yGrid / 2, &object.alphabet, 1);
			SetBkColor(hDC, RGB(255, 255, 255));
		}
		else
			TextOut(hDC, object.position.x * xGrid + xGrid / 2, object.position.y * yGrid + yGrid / 2, &object.alphabet, 1);
	}
}