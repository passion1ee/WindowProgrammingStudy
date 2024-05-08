#include "ColorTetris.h"
#include "resource.h"
#include <map>

namespace ys
{
	void ColorTetris::Init(HWND hWnd_, RECT& screenSize)
	{
		hWnd = hWnd_;
		hDc = GetDC(hWnd_);
		hBackDc = CreateCompatibleDC(hDc);

		setScreen(screenSize);

		score = 0;

		isRun = false;
		isSameColor = false;
		isKeep = false;
		blockDownTime = 1.0f;
		curTime = 0.0f;
		blockMaxIndex = static_cast<BYTE>(Block::shapeL) + 1;
		colorMaxIndex = 8;

		activeObj = nullptr;
		nextObj = nullptr;
		keepObj = nullptr;

		Object tmp(RECT({ 0, 0, kColSize, kRowSize }));
		world = tmp;

		colors.push_back(RGB(255, 0, 0));
		colors.push_back(RGB(0, 255, 0));
		colors.push_back(RGB(0, 0, 255));
		colors.push_back(RGB(255, 0, 255));
		colors.push_back(RGB(255, 255, 0));
		colors.push_back(RGB(0, 255, 255));
		colors.push_back(RGB(150, 75, 0));
		colors.push_back(RGB(139, 0, 255));

		InputManager::Init();
		Timer::Init();
	}
	void ColorTetris::Run()
	{
		InputManager::BeforeUpdate();
		Timer::Update();
		if (!isRun) return;

		if (Timer::getDeltaTime() > 0.0f)
		{
			Update();
			Render();
			if (!isRun)
				PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
		InputManager::AfterUpdate();
	}

	void ColorTetris::command(const WORD& command)
	{
		switch (command)
		{
		case ID_START:
			isRun = true;
			break;
		case ID_END:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;

		case ID_SPEED_FAST:
			blockDownTime = 0.5f;
			break;
		case ID_SPEED_MEDIUM:
			blockDownTime = 1.0f;
			break;
		case ID_SPEED_SLOW:
			blockDownTime = 1.5f;
			break;

		case ID_BLOCK_4:
			blockMaxIndex = 4;
			break;
		case ID_BLOCK_5:
			blockMaxIndex = 5;
			break;
		case ID_BLOCK_6:
			blockMaxIndex = 6;
			break;
		case ID_BLOCK_7:
			blockMaxIndex = 7;
			break;
		case ID_BLOCK_8:
			blockMaxIndex = 8;
			break;

		case ID_COLOR_6:
			colorMaxIndex = 6;
			break;
		case ID_COLOR_7:
			colorMaxIndex = 7;
			break;
		case ID_COLOR_8:
			colorMaxIndex = 8;
			break;
		default:
			break;
		}
	}

	void ColorTetris::Update()
	{
		if(InputManager::getKeyDown((UINT)Key::H)) isSameColor = !isSameColor;

		if (InputManager::getKeyDown(VK_SPACE) && !isKeep)
		{
			isKeep = true;
			if (activeObj->childObjs.size() == 2)
				activeObj->size = {0, 0, 2, 2};
			else
				activeObj->size = {0, 0, 3, 3};

			for (int y = 0; y < activeObj->childObjs.size(); ++y)
				for (int x = 0; x < activeObj->childObjs[y].size(); ++x)
					activeObj->childObjs[y][x].position = { x, y };
			if(keepObj == nullptr)
			{
				keepObj = activeObj;
				activeObj = nullptr;
			}
			else
			{
				auto tmp = activeObj;
				activeObj = keepObj;
				auto max = uid(randomEngine) % (kColSize - 1);
				for (int i = 0; i < max; ++i)
					activeObj->Move(false, true, world);
				keepObj = tmp;
			}
		}

		if (activeObj == nullptr)
		{
			if(nextObj != nullptr)
			{
				auto max = uid(randomEngine) % (kColSize - 1);
				for (int i = 0; i < max; ++i)
					nextObj->Move(false, true, world);
			}
			activeObj = nextObj;
			nextObj = new Object();
			nextObj->SetBlock(static_cast<Block>(uid(randomEngine) % blockMaxIndex), colors, colorMaxIndex, isSameColor);
		}

		if (InputManager::getKeyDown(VK_UP))		activeObj->Rotate(world);
		if (InputManager::getKeyDown(VK_LEFT))		activeObj->Move(false, false, world);
		if (InputManager::getKeyDown(VK_RIGHT))		activeObj->Move(false, true, world);
		if (InputManager::getKeyDown(VK_DOWN))		curTime += blockDownTime;
													
		curTime += Timer::getDeltaTime() * (1.0 + (score / 1000) * 0.05);
		if(curTime >= blockDownTime) 
		{
			if (!activeObj->Move(true, true, world))
			{
				isKeep = false;
				for (auto& chiledObj : activeObj->childObjs)
					for (auto& obj : chiledObj)
						if (obj.isActive)
							world.childObjs[obj.position.y][obj.position.x] = obj;
				delete activeObj;
				activeObj = nullptr;
			}

			if(curTime >= 1.5 * blockDownTime)
				curTime = 0;
			else
				curTime -= blockDownTime;
		}

		int weight{};
		bool isActiveLine = true;
		for (auto& childObjs : world.childObjs)
		{
			LONG setY;
			isActiveLine = true;
			for (auto& child : childObjs)
			{
				setY = child.position.y;
				if (!child.isActive) isActiveLine = false;
			}

			if (isActiveLine)
			{
				weight++;
				score += 500 * weight;
				for (int y = setY - 1; y >= 0; --y)
					for (int x = 0; x < kColSize; ++x)
					{
						world.childObjs[y][x].Move(true, true);
						world.childObjs[y + 1][x] = world.childObjs[y][x];
					}
				for (int x = 0; x < kColSize; ++x)
					world.childObjs[0][x].isActive = false;
			}
		}

		std::map<std::string, POINT> search;
		for (auto& childObjs : world.childObjs)
			for (auto& child : childObjs)
			{
				if (!child.isActive) continue;
				search.clear();
				auto tmp = searchSameColor(search, child.position);
				if (tmp.size() >= 3)
				{
					score += 100 * tmp.size();
					for (const auto& [key, value] : tmp)
					{
						for (int y = value.y - 1; y >= 0; --y)
						{
							world.childObjs[y][value.x].Move(true, true);
							world.childObjs[y + 1][value.x] = world.childObjs[y][value.x];
							search.erase(key);
						}
						world.childObjs[0][value.x].isActive = false;
					}
				}
			}
	}

	void ColorTetris::Render()
	{
		PatBlt(hBackDc, 0, 0, width, height, WHITENESS);
		renderFrame();

		std::wstring line = L"SCORE : " + std::to_wstring(score);
		SIZE size;  GetTextExtentPoint32(hBackDc, line.c_str(), line.size(), &size);
		TextOut(hBackDc, (width * 9 / 10) - size.cx, height / 10 - 40, line.c_str(), line.size());

		if (activeObj != nullptr)
		{
			for (auto& childObjs : activeObj->childObjs)
				for (auto& child : childObjs)
				{
					if (child.isActive && world.childObjs[child.position.y][child.position.x].isActive)
					{
						std::wstring line = L"GAME OVER";
						SIZE size;  GetTextExtentPoint32(hBackDc, line.c_str(), line.size(), &size);
						TextOut(hBackDc, width / 2 - size.cx, height / 10, line.c_str(), line.size());
						TextOut(hBackDc, width / 2 - size.cx, height * 2 / 10, line.c_str(), line.size());
						TextOut(hBackDc, width / 2 - size.cx, height * 3 / 10, line.c_str(), line.size());
						Beep(800, 200);
						Beep(300, 600);
						isRun = false;
					}
				}
		}

		if(activeObj != nullptr)
		{
			for (auto& chiledObj : activeObj->childObjs)
				for (auto& obj : chiledObj)
					if (obj.isActive)
					{
						auto brush = CreateSolidBrush(obj.color);
						auto oldBrush = SelectObject(hBackDc, brush);
						Rectangle(hBackDc, obj.position.x * xGrid, obj.position.y * yGrid
							, (obj.position.x + 1) * xGrid, (obj.position.y + 1) * yGrid);
						SelectObject(hBackDc, oldBrush);
						DeleteObject(brush);
					}
		}


		line = L"NEXT BLOCK";
		size;  GetTextExtentPoint32(hBackDc, line.c_str(), line.size(), &size);
		TextOut(hBackDc, (width * 9 / 10) - size.cx, height / 10, line.c_str(), line.size());
		for (auto& chiledObj : nextObj->childObjs)
			for (auto& obj : chiledObj)
				if (obj.isActive)
				{
					auto brush = CreateSolidBrush(obj.color);
					auto oldBrush = SelectObject(hBackDc, brush);
					Rectangle(hBackDc, (width * 7 / 10) + obj.position.x * xGrid, (height / 5) + obj.position.y * yGrid
						, (width * 7 / 10) + (obj.position.x + 1) * xGrid, (height / 5) + (obj.position.y + 1) * yGrid);
					SelectObject(hBackDc, oldBrush);
					DeleteObject(brush);
				}


		line = L"KEEP BLOCK";
		size;  GetTextExtentPoint32(hBackDc, line.c_str(), line.size(), &size);
		TextOut(hBackDc, (width * 9 / 10) - size.cx, height * 6 / 10, line.c_str(), line.size());
		if (keepObj != nullptr)
		{
			for (auto& chiledObj : keepObj->childObjs)
				for (auto& obj : chiledObj)
					if (obj.isActive)
					{
						auto brush = CreateSolidBrush(obj.color);
						auto oldBrush = SelectObject(hBackDc, brush);
						Rectangle(hBackDc, (width * 7 / 10) + obj.position.x * xGrid, (height * 7 / 10) + obj.position.y * yGrid
							, (width * 7 / 10) + (obj.position.x + 1) * xGrid, (height * 7 / 10) + (obj.position.y + 1) * yGrid);
						SelectObject(hBackDc, oldBrush);
						DeleteObject(brush);
					}
		}

		for (auto& chiledObj : world.childObjs)
			for (auto& obj : chiledObj)
				if(obj.isActive)
				{
					auto brush = CreateSolidBrush(obj.color);
					auto oldBrush = SelectObject(hBackDc, brush);
					Rectangle(hBackDc, obj.position.x * xGrid, obj.position.y * yGrid
						, (obj.position.x + 1) * xGrid, (obj.position.y + 1) * yGrid);
					SelectObject(hBackDc, oldBrush);
					DeleteObject(brush);
				}

		BitBlt(hDc, 0, 0, width, height, hBackDc, 0, 0, SRCCOPY);
	}

	void ColorTetris::renderFrame()
	{
		for (int i = 1; i < kRowSize; ++i)
		{
			MoveToEx(hBackDc, 0, i * yGrid, NULL);
			LineTo(hBackDc, width * 2 / 3, i * yGrid);
		}

		for (int i = 1; i < kColSize; ++i)
		{
			MoveToEx(hBackDc, i * xGrid, 0, NULL);
			LineTo(hBackDc, i * xGrid, height);
		}
		Rectangle(hBackDc, width * 2 / 3, 0, width, height);
	}

	void ColorTetris::setScreen(RECT& screenSize)
	{
		::AdjustWindowRect(&screenSize, WS_OVERLAPPEDWINDOW, FALSE);
		screen = screenSize;
		width = screen.right + screen.left; height = screen.bottom + screen.top;
		SetWindowPos(hWnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);

		hBitmap = CreateCompatibleBitmap(hDc, width, height);
		auto oldBitmap = SelectObject(hBackDc, hBitmap);
		DeleteObject(oldBitmap);
		xGrid = (width * 2 / 3) / static_cast<double>(kColSize);
		yGrid = height / static_cast<double>(kRowSize);
	}

	std::map<std::string, POINT> ColorTetris::searchSameColor(std::map<std::string, POINT>& search, const POINT& child)
	{
		if(child.x - 1 >= 0 && child.x - 1 < kColSize - 1)
			if (world.childObjs[child.y][child.x - 1].color == world.childObjs[child.y][child.x].color)
			{
				auto it = search.insert(std::make_pair(
					std::to_string(child.x - 1) + ", " + std::to_string(child.y),
					POINT(child.x - 1, child.y)));
				
				if(it.second) searchSameColor(search, it.first->second);
			}
		if(child.x + 1 > 0 && child.x + 1 < kColSize)
			if (world.childObjs[child.y][child.x + 1].color == world.childObjs[child.y][child.x].color)
			{
				auto it = search.insert(std::make_pair(
					std::to_string(child.x + 1) + ", " + std::to_string(child.y),
					POINT(child.x + 1, child.y)));
				if (it.second) searchSameColor(search, it.first->second);
			}

		if(child.y - 1 >= 0 && child.y - 1 < kRowSize - 1)
			if (world.childObjs[child.y - 1][child.x].color == world.childObjs[child.y][child.x].color)
			{
				auto it = search.insert(std::make_pair(
					std::to_string(child.x) + ", " + std::to_string(child.y - 1),
					POINT(child.x, child.y - 1)));
				if (it.second) searchSameColor(search, it.first->second);
			}

		if(child.y + 1 > 0 && child.y + 1 < kRowSize)
			if (world.childObjs[child.y + 1][child.x].color == world.childObjs[child.y][child.x].color)
			{
				auto it = search.insert(std::make_pair(
					std::to_string(child.x) + ", " + std::to_string(child.y + 1),
					POINT(child.x, child.y + 1)));
				if (it.second) searchSameColor(search, it.first->second);
			}
		return search;
	}

	void ColorTetris::Object::SetBlock(const Block& block, const std::vector<COLORREF>& colors,
		const size_t& maxIndex, const bool& isSameColor)
	{
		childObjs.clear();
		for (int j = 0; j < 3; ++j)
		{
			childObjs.push_back(std::vector<childObj>());
			for (int i = 0; i < 3; ++i)
				childObjs[j].push_back(childObj(POINT(i, j), RGB(0, 0, 0)));
		}
		size = RECT(0, 0, 3, 3);
		switch (block)
		{
		case ys::ColorTetris::Block::Square:
		{
			childObjs.clear();
			for (int j = 0; j < 2; ++j)
			{
				childObjs.push_back(std::vector<childObj>());
				for (int i = 0; i < 2; ++i)
					childObjs[j].push_back(childObj(POINT(i, j), RGB(0, 0, 0)));
			}
			size = RECT(0, 0, 2, 2);
			childObjs[0][0].isActive = true;
			childObjs[0][1].isActive = true;
			childObjs[1][0].isActive = true;
			childObjs[1][1].isActive = true;
			break;
		}
		case ys::ColorTetris::Block::straight:
		{
			childObjs[0][0].isActive = true;
			childObjs[1][0].isActive = true;
			childObjs[2][0].isActive = true;
			break;
		}
		case ys::ColorTetris::Block::shapeU:
		{
			childObjs[0][0].isActive = true;
			childObjs[0][1].isActive = true;
			childObjs[1][0].isActive = true;
			childObjs[2][0].isActive = true;
			childObjs[2][1].isActive = true;
			break;
		}
		case ys::ColorTetris::Block::shapeT:
		{
			childObjs[0][0].isActive = true;
			childObjs[1][0].isActive = true;
			childObjs[1][1].isActive = true;
			childObjs[2][0].isActive = true;
			break;
		}
		case ys::ColorTetris::Block::shapeZ:
		{
			childObjs[0][1].isActive = true;
			childObjs[1][0].isActive = true;
			childObjs[1][1].isActive = true;
			childObjs[2][0].isActive = true;
			break;
		}
		case ys::ColorTetris::Block::shapeS:
		{
			childObjs[0][0].isActive = true;
			childObjs[1][0].isActive = true;
			childObjs[1][1].isActive = true;
			childObjs[2][1].isActive = true;
			break;
		}
		case ys::ColorTetris::Block::shapeJ:
		{
			childObjs[0][1].isActive = true;
			childObjs[1][1].isActive = true;
			childObjs[2][0].isActive = true;
			childObjs[2][1].isActive = true;
			break;
		}
		case ys::ColorTetris::Block::shapeL:
		{
			childObjs[0][0].isActive = true;
			childObjs[1][0].isActive = true;
			childObjs[2][0].isActive = true;
			childObjs[2][1].isActive = true;
			break;
		}
		default:
			break;
		}
		COLORREF tmp;
		if (isSameColor)
			tmp = colors[uid(randomEngine) % maxIndex];

		for(auto& childObj : childObjs)
			for(auto& obj : childObj)
				if(obj.isActive)
				{
					if(isSameColor)
						obj.color = tmp;
					else
						obj.color = colors[uid(randomEngine) % maxIndex];
				}
	}

	bool ColorTetris::Object::Move(const bool& isVertical, const bool& isPlus, const Object& world)
	{
		bool isCollideX = false;
		bool isCollideY = false;
		for (auto& chiledObj : childObjs)
			for (auto& obj : chiledObj)
				if (obj.isActive && !isCollideX && !isCollideY)
				{
					std::pair<bool, bool> bools = obj.MoveCollide(isVertical, isPlus, world);
					isCollideX = bools.first; isCollideY = bools.second;
				}
				else if (isCollideX || isCollideY) break;
		if (isCollideX || isCollideY)
		{
			if (isCollideX) return true;
			if (isCollideY) return false;
		}
		for (auto& chiledObj : childObjs)
			for (auto& obj : chiledObj)
				obj.Move(isVertical, isPlus);
		if (isVertical && isPlus && size.top < kRowSize - 1)
			OffsetRect(&size, 0, 1);
		else if (!isVertical && isPlus && size.right < kColSize)
			OffsetRect(&size, 1, 0);
		else if (!isVertical && !isPlus && size.left > 0)
			OffsetRect(&size, -1, 0);
		return true;
		//面倒贸府 眉农
	}
	
	void ColorTetris::Object::Rotate(const Object& world)
	{
		auto tmp = childObjs;
		for (int i = 0; i < childObjs.size(); ++i)
			for (int j = 0; j < childObjs.front().size(); ++j)
			{
				tmp[i][j] = childObjs[childObjs.front().size() - 1 - j][i];
				auto tmpPos = childObjs[childObjs.front().size() - 1 - j][i].position;
				tmp[i][j].position.y = size.top + (childObjs.front().size() - 1 - j);
				tmp[i][j].position.x = size.left + i;

				if (tmp[i][j].isActive)
					if (tmp[i][j].position.y < 0 || tmp[i][j].position.y >= kRowSize 
						|| world.childObjs[tmp[i][j].position.y][tmp[i][j].position.x].isActive)
						return;
					else if (tmp[i][j].position.x < 0 || tmp[i][j].position.x >= kColSize 
						|| world.childObjs[tmp[i][j].position.y][tmp[i][j].position.x].isActive)
						return;
			}		   

		for (int i = 0; i < childObjs.size(); ++i) {
			for (int j = 0; j < childObjs.front().size(); ++j) {
				childObjs[i][j] = tmp[i][j];
			}
		}
	}

	std::pair<bool, bool> ColorTetris::childObj::MoveCollide(const bool& isVertical, const bool& isPlus, const Object& world)
	{
		if (isVertical && isPlus)
		{
			if (position.y < kRowSize - 1)
			{
				if (world.childObjs[position.y + 1][position.x].isActive)
					return std::make_pair(false, true);
				else
					return std::make_pair(false, false);
			}
			else
				return std::make_pair(false, true);
		}
		else if (!isVertical && isPlus)
		{
			if (position.x < kColSize - 1)
			{
				if (world.childObjs[position.y][position.x + 1].isActive)
					return std::make_pair(true, false);
				else
					return std::make_pair(false, false);
			}
			else
				return std::make_pair(true, false);
		}
		else if (!isVertical && !isPlus)
		{
			if (position.x > 0)
			{
				if (world.childObjs[position.y][position.x - 1].isActive)
					return std::make_pair(true, false);
				else
					return std::make_pair(false, false);
			}
			else
				return std::make_pair(true, false);
		}
		return std::make_pair(false, false);
	}
}

