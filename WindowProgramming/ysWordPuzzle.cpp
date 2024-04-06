#include "ysWordPuzzle.h"
#include "stdfax.h"

namespace ys
{
	int WordPuzzle::screenWidth;
	int WordPuzzle::screenHeight;

	BYTE WordPuzzle::mapSize;
	double WordPuzzle::xGrid;
	double WordPuzzle::yGrid;

	POINT WordPuzzle::player;
	std::vector<WordPuzzle::Object> WordPuzzle::alphabets;
	std::vector<std::vector<std::wstring>> WordPuzzle::plain;
	std::wstring WordPuzzle::goalWord;

	void WordPuzzle::setScreen(int width, int height)
	{
		screenWidth = width;
		screenHeight = height;
		xGrid = screenWidth / static_cast<double>(mapSize);
		yGrid = screenHeight / static_cast<double>(mapSize);
	}

	void WordPuzzle::Init()
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
				do	{
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

	void WordPuzzle::Run(HWND hWnd)
	{
		InputManager::BeforeUpdate();
		Update(hWnd);
		InputManager::AfterUpdate();
	}

	void WordPuzzle::render(HDC hDC)
	{
		renderFrame(hDC);
		for (const auto& alphabet : alphabets)
			renderObject(hDC, alphabet);
		Ellipse(hDC, player.x * xGrid, player.y * yGrid, (player.x + 1) * xGrid, (player.y + 1) * yGrid);
	}

	void WordPuzzle::Update(HWND hWnd)
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

		if (isWord())
		{
			victorySound();
			MessageBox(hWnd, L"VICTORY!", L"", NULL);
			Init();
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}

	void WordPuzzle::renderFrame(HDC hDC)
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
	void WordPuzzle::renderObject(HDC hDC, Object object)
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
	bool WordPuzzle::isWord()
	{
		auto wordStart = (mapSize - (goalWord.size())) / 2;
		std::wstring goalCheck;
		for(int i = 0; i < goalWord.size(); ++i)
			for (auto iter = alphabets.begin(); iter != alphabets.end(); ++iter)
				if (iter->position.y == 1 && iter->position.x == wordStart + i)
					goalCheck += iter->alphabet;

		if (goalCheck == goalWord)
			return true;
		else
			return false;
	}
	void WordPuzzle::victorySound()
	{
		Beep(130, 100);
		Beep(262, 100);
		Beep(330, 100);
		Beep(392, 100);
		Beep(523, 100);
		Beep(660, 100);
		Beep(784, 300);
		Beep(660, 300);
		Beep(146, 100);
		Beep(262, 100);
		Beep(311, 100);
		Beep(415, 100);
		Beep(523, 100);
		Beep(622, 100);
		Beep(831, 300);
		Beep(622, 300);
		Beep(155, 100);
		Beep(294, 100);
		Beep(349, 100);
		Beep(466, 100);
		Beep(588, 100);
		Beep(699, 100);
		Beep(933, 300);
		Beep(933, 100);
		Beep(933, 100);
		Beep(933, 100);
		Beep(1047, 400);
	}
}