#include "ysDigitAlphaPlus.h"
#include "stdfax.h"

#include <iostream>

constexpr BYTE kDigitMod = 1;
constexpr BYTE kAlphaMod = 2;
constexpr BYTE kDualMod = 3;

namespace ys
{
	HWND DigitAlphaPlus::hWnd;
	HDC DigitAlphaPlus::hDC;
	HDC DigitAlphaPlus::hBackDC;
	HBITMAP DigitAlphaPlus::hBitmap;

	POINT DigitAlphaPlus::screen;
	BYTE DigitAlphaPlus::mapSize;
	double DigitAlphaPlus::xGrid;
	double DigitAlphaPlus::yGrid;

	BYTE DigitAlphaPlus::mod;
	float DigitAlphaPlus::coolTime;

	POINT DigitAlphaPlus::player;
	std::vector<DigitAlphaPlus::Object> DigitAlphaPlus::alphabets;
	

	void DigitAlphaPlus::setScreen(RECT screenSize)
	{
		//::AdjustWindowRect(&screenSize, WS_OVERLAPPEDWINDOW, FALSE);
		screen.x = screenSize.right - screenSize.left; screen.y = screenSize.bottom - screenSize.top;
		
		SetWindowPos(hWnd, nullptr, 0, 0, screen.x, screen.y, 0);

		hBitmap = CreateCompatibleBitmap(hDC, screen.x, screen.y);
		auto oldBitmap = SelectObject(hBackDC, hBitmap);
		DeleteObject(oldBitmap);
		xGrid = screen.x / static_cast<double>(mapSize);
		yGrid = screen.y / static_cast<double>(mapSize);
	}

	void DigitAlphaPlus::Init(HWND hWnd_, RECT screenSize)
	{
		hWnd = hWnd_;
		hDC = GetDC(hWnd_);
		hBackDC = CreateCompatibleDC(hDC);

		alphabets.clear();
		std::vector<std::vector<std::wstring>> plain;
		mapSize = 20;

		ys::DigitAlphaPlus::setScreen(screenSize);

		int id{};
		alphabets.reserve(26 + 10);
		plain.assign(mapSize, std::vector<std::wstring>(mapSize, L""));

		coolTime = 0;
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
		Timer::Init();
	}

	void DigitAlphaPlus::Run()
	{
		Timer::Update();
		InputManager::BeforeUpdate();
		Update();
		InputManager::AfterUpdate();
		render();
	}

	void DigitAlphaPlus::render()
	{
		PatBlt(hBackDC, 0, 0, screen.x, screen.y, WHITENESS);
		renderFrame();
		for (const auto& alphabet : alphabets)
			renderObject(alphabet);
		auto hBrush = CreateSolidBrush(RGB(255, 255, 0));
		auto oldBrush = (HBRUSH)SelectObject(hBackDC, hBrush);
		Ellipse(hBackDC, player.x * xGrid, player.y * yGrid, (player.x + 1) * xGrid, (player.y + 1) * yGrid);
		SelectObject(hBackDC, oldBrush);
		DeleteObject(hBrush);
		SetBkColor(hBackDC, RGB(255, 255, 0));
		TextOut(hBackDC, player.x * xGrid + xGrid / 2, player.y * yGrid + yGrid / 2, std::to_wstring(mod).c_str(), std::to_wstring(mod).size());
		SetBkColor(hBackDC, RGB(255, 255, 255));

		BitBlt(hDC, 0, 0, screen.x, screen.y, hBackDC, 0, 0, SRCCOPY);
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
			Init(hWnd, RECT(0, 0, screen.x, screen.y));
		}
	}

	void DigitAlphaPlus::Update()
	{
		bool isCoolTime{ true };
		bool isMove{ false };
		
		if (coolTime >= 0)
			coolTime -= Timer::getDeltaTime();
		else
		{
			coolTime = 0.0f;
			isCoolTime = false;
		}

		if (InputManager::getKey(VK_LEFT) && !isCoolTime)
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
			isMove = true;
		}
		if (InputManager::getKey(VK_RIGHT) && !isCoolTime)
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
			isMove = true;
		}
		if (InputManager::getKey(VK_UP) && !isCoolTime)
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
			isMove = true;
		}
		if (InputManager::getKey(VK_DOWN) && !isCoolTime)
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
			isMove = true;
		}
		if (isMove && !isCoolTime)
		{
			std::cout << 1 / Timer::getDeltaTime() << std::endl;
			coolTime = 0.15f;
		}
		if (InputManager::getKeyDown((UINT)Key::P))
		{
			Init(hWnd, RECT(0, 0, screen.x, screen.y));
		}
		if (InputManager::getKeyUp((UINT)Key::Q))
		{
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}

	}

	void DigitAlphaPlus::renderFrame()
	{
		for (int i = 0; i < mapSize; ++i)
		{
			MoveToEx(hBackDC, 0, i * yGrid, NULL);
			LineTo(hBackDC, screen.x, i * yGrid);
		}
		for (int i = 0; i < mapSize; ++i)
		{
			MoveToEx(hBackDC, i * xGrid, 0, NULL);
			LineTo(hBackDC, i * xGrid, screen.y);
		}
	}
	void DigitAlphaPlus::renderObject(Object object)
	{
		if(object.isAlpha)
		{
			if(object.count % 26 == 0)
				TextOut(hBackDC, object.position.x * xGrid + xGrid / 2, object.position.y * yGrid + yGrid / 2,
					std::wstring(1, L'Z').c_str(), std::wstring(1, L'Z').size());
			else
				TextOut(hBackDC, object.position.x * xGrid + xGrid / 2, object.position.y * yGrid + yGrid / 2,
					std::wstring(1, L'A' + object.count % 26 - 1).c_str(), std::wstring(1, L'A' + object.count % 26 - 1).size());
		}
		else
			TextOut(hBackDC, object.position.x * xGrid + xGrid / 2, object.position.y * yGrid + yGrid / 2, 
				std::to_wstring(object.count).c_str(), std::to_wstring(object.count).size());
	}
}