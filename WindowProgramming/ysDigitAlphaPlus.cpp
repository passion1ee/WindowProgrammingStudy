#include "ysDigitAlphaPlus.h"
#include "stdfax.h"

constexpr BYTE kDigitMod = 1;
constexpr BYTE kAlphaMod = 2;
constexpr BYTE kDualMod = 3;

namespace ys
{
	int DigitAlphaPlus::screenWidth;
	int DigitAlphaPlus::screenHeight;

	BYTE DigitAlphaPlus::mapSize;
	double DigitAlphaPlus::xGrid;
	double DigitAlphaPlus::yGrid;

	BYTE DigitAlphaPlus::mod;

	POINT DigitAlphaPlus::player;
	std::vector<DigitAlphaPlus::Object> DigitAlphaPlus::alphabets;
	

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
		std::vector<std::vector<std::wstring>> plain;
		mapSize = 20;

		int id{};
		alphabets.reserve(26 + 10);
		plain.assign(mapSize, std::vector<std::wstring>(mapSize, L""));

		std::uniform_int_distribution<> position(0, mapSize - 1);
		if (mod & kDigitMod) {
			for (auto i = L'0'; i <= L'9';)
			{
				auto x = position(randomEngine); auto y = position(randomEngine);
				if (plain[y][x] != L"" || y == 0 || y == 1)
					continue;

				plain[y][x] = i;
				alphabets.push_back(Object(i - L'0', POINT(x, y), false, id++));
				++i;
			}
		}
		if(mod & kAlphaMod){
			for (auto i = L'A'; i <= L'Z';)
			{
				auto x = position(randomEngine); auto y = position(randomEngine);
				if (plain[y][x] != L"" || y == 0 || y == 1)
					continue;

				plain[y][x] = i;
				alphabets.push_back(Object(i - L'A' + 1, POINT(x, y), true, id++));
				++i;
			}
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
		auto hBrush = CreateSolidBrush(RGB(255, 255, 0));
		auto oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, player.x * xGrid, player.y * yGrid, (player.x + 1) * xGrid, (player.y + 1) * yGrid);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);
		SetBkColor(hDC, RGB(255, 255, 0));
		TextOut(hDC, player.x * xGrid + xGrid / 2, player.y * yGrid + yGrid / 2, std::to_wstring(mod).c_str(), std::to_wstring(mod).size());
		SetBkColor(hDC, RGB(255, 255, 255)); 
	}

	void DigitAlphaPlus::select(WPARAM w)
	{
		bool isDigit{ true };
		std::wstring input; input = w;
		for (auto& ch : input)
			if (!iswdigit(ch))
				isDigit = false;
		if (isDigit && std::stoi(input) <= 3)
		{
			mod = std::stoi(input);
			Init();
		}
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
					otherIter->count += iter->count;
					alphabets.erase(iter);
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
					otherIter->count += iter->count;
					alphabets.erase(iter);
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
					otherIter->count += iter->count;
					alphabets.erase(iter);
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
					otherIter->count += iter->count;
					alphabets.erase(iter);
					Beep(1760, 40);
					Beep(2218, 80);
				}
			}
		}

		if (InputManager::getKeyDown((UINT)Key::P))
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
		if(object.isAlpha)
			TextOut(hDC, object.position.x * xGrid + xGrid / 2, object.position.y * yGrid + yGrid / 2, std::wstring(1, L'A' + object.count % 27 - 1).c_str(), std::wstring(1, L'A' + object.count % 27 - 1).size());
		else
			TextOut(hDC, object.position.x * xGrid + xGrid / 2, object.position.y * yGrid + yGrid / 2, std::to_wstring(object.count).c_str(), std::to_wstring(object.count).size());
	}
}