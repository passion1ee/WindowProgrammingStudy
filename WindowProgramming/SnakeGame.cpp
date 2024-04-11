#include "SnakeGame.h"
#include "stdfax.h"

#include <iostream>

constexpr BYTE kMapSize = 40;

constexpr BYTE kHorizon = 0x1;
constexpr BYTE kVertical = 0x2;
constexpr BYTE kRect = 0x4;
constexpr BYTE kStop = 0x8;

constexpr BYTE kRectWidth = 0x10;
constexpr BYTE kRectHeight = 0x20;

namespace ys
{
	SnakeGame::SnakeGame()
		: hWnd(nullptr), hDC(nullptr), hBackDC(nullptr), hBitmap(nullptr), screen({ 0, 0 }), xGrid(0), yGrid(0), frameCheck(0), moveCoolTime(0), setMoveTime(0), speed(0.00f)
	{

	}

	void SnakeGame::setScreen(RECT screenSize)
	{
		//::AdjustWindowRect(&screenSize, WS_OVERLAPPEDWINDOW, FALSE);
		screen.x = screenSize.right - screenSize.left; screen.y = screenSize.bottom - screenSize.top;

		SetWindowPos(hWnd, nullptr, 0, 0, screen.x, screen.y, 0);

		hBitmap = CreateCompatibleBitmap(hDC, screen.x, screen.y);
		auto oldBitmap = SelectObject(hBackDC, hBitmap);
		DeleteObject(oldBitmap);
		xGrid = screen.x / static_cast<double>(kMapSize);
		yGrid = screen.y / static_cast<double>(kMapSize);
	}

	void SnakeGame::Init(HWND hWnd_, RECT screenSize)
	{
		hWnd = hWnd_;
		hDC = GetDC(hWnd_);
		hBackDC = CreateCompatibleDC(hDC);

		SnakeGame::setScreen(screenSize);
		std::uniform_int_distribution<> position(0, kMapSize - 1);
		player = Object(0, 100, { 0, 0 }, RGB(ObjectColor(randomEngine), ObjectColor(randomEngine), ObjectColor(randomEngine)), kHorizon, true);
		for (int i = 0; i < 30;)
		{
			auto x = position(randomEngine); auto y = position(randomEngine);
			if (feeds.end() == std::find_if(feeds.begin(), feeds.end(), [x, y](Object& comp) {return comp.position.x == x && comp.position.y == y; }) && (x != 0 && y != 0))
			{
				feeds.push_back(Object(1, 50, { position(randomEngine), position(randomEngine) },
					RGB(ObjectColor(randomEngine), ObjectColor(randomEngine), ObjectColor(randomEngine)), kStop, true));
				++i;
			}
		}
		snakeList.assign(1, std::list<Object>(1, player));
		moveCoolTime = 0;
		Timer::Init();
	}

	void SnakeGame::Run()
	{
		static bool isRun = false;
		InputManager::BeforeUpdate();
		Timer::Update();
		if (ys::InputManager::getKeyDown(VK_ADD) && speed <= 1 / Timer::getRealFPS() / 2)
		{
			speed += 0.001f;
		}
		if (ys::InputManager::getKeyDown(VK_SUBTRACT) && speed > -1 / Timer::getRealFPS())
			speed -= 0.001f;
		if (InputManager::getKeyUp((UINT)Key::S) || isRun) {
			if (InputManager::getKeyUp((UINT)Key::S)) {
				setMoveTime = 2 / Timer::getRealFPS();
				isRun = isRun ? false : true;
			}
			moveCoolTime += Timer::getDeltaTime();
			if (1 / Timer::getRealFPS() - speed <= (frameCheck += Timer::getDeltaTime())) {
				frameCheck = 0.0f;
				Update();
				Render();
				Timer::Rander(hDC, screen);
			}
		}
		InputManager::AfterUpdate();
	}

	void SnakeGame::Render()
	{
		PatBlt(hBackDC, 0, 0, screen.x, screen.y, WHITENESS);
		
		renderFrame();

		for(auto& feed : feeds)
		{
			auto brush = CreateSolidBrush(feed.color);
			auto oldBrush = SelectObject(hBackDC, brush);
			renderObject(feed, false);
			SelectObject(hBackDC, oldBrush);
			DeleteObject(brush);
		}

		for(auto& box : boxs)
		{
			auto brush = CreateSolidBrush(box.color);
			auto oldBrush = SelectObject(hBackDC, brush);
			renderObject(box, false);
			SelectObject(hBackDC, oldBrush);
			DeleteObject(brush);
		}

		for(auto& list : snakeList)
			for(auto& obj : list)
			{
				auto brush = CreateSolidBrush(obj.color);
				auto oldBrush = SelectObject(hBackDC, brush);
				renderObject(obj, true);
				SelectObject(hBackDC, oldBrush);
				DeleteObject(brush);
			}

		BitBlt(hDC, 0, 0, screen.x, screen.y, hBackDC, 0, 0, SRCCOPY);
	}

	void SnakeGame::Update()
	{
		std::cout << speed << std::endl;
		if (InputManager::getKey(VK_LEFT))
		{
			snakeList.front().front().moveState = kHorizon;
			snakeList.front().front().isPlus = false;
		}
		if (InputManager::getKey(VK_RIGHT))
		{
			snakeList.front().front().moveState = kHorizon;
			snakeList.front().front().isPlus = true;
		}
		if (InputManager::getKey(VK_UP))
		{
			snakeList.front().front().moveState = kVertical;
			snakeList.front().front().isPlus = false;
		}
		if (InputManager::getKey(VK_DOWN))
		{
			snakeList.front().front().moveState = kVertical;
			snakeList.front().front().isPlus = true;
		}

		if (setMoveTime <= moveCoolTime)
		{
			for (auto& list : snakeList)
			{
				if (list == snakeList.front()) continue;
				move(list);
			}
		}

		move(snakeList.front());
		player = snakeList.front().front();

		if(snakeList.size() > 1)
			catchSnake();
		collide();

		if (ys::InputManager::getKeyUp((UINT)ys::Key::Q))
		{
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
		if (setMoveTime <= moveCoolTime) moveCoolTime = 0;
	}

	void SnakeGame::renderFrame()
	{
		for (int i = 0; i < kMapSize; ++i)
		{
			MoveToEx(hBackDC, 0, i * yGrid, NULL);
			LineTo(hBackDC, screen.x, i * yGrid);
		}
		for (int i = 0; i < kMapSize; ++i)
		{
			MoveToEx(hBackDC, i * xGrid, 0, NULL);
			LineTo(hBackDC, i * xGrid, screen.y);
		}
	}

	void SnakeGame::renderObject(Object& object, bool isCircle)
	{
		auto realXGrid = xGrid * 0.01 * object.size;
		auto realYGrid = yGrid * 0.01 * object.size;
		auto realPositionX = object.position.x * xGrid + (xGrid - realXGrid) / 2.0;
		auto realPositionY = object.position.y * yGrid + (yGrid - realYGrid) / 2.0;
		if (isCircle)
			Ellipse(hBackDC, realPositionX, realPositionY, realPositionX + realXGrid, realPositionY + realYGrid);
		else
			Rectangle(hBackDC, realPositionX, realPositionY, realPositionX + realXGrid, realPositionY + realYGrid);
	}

	void SnakeGame::move(std::list<Object>& body)
	{	
		body.front().prevPosition = body.front().position;
		switch (body.front().moveState)
		{
		case kHorizon:
		{
			if(body.front().isPlus)
			{
				if(body.front().position.x >= 39)
				{
					if (body.front().position.y >= 39)
						body.front().position.y = 0;
					else
						body.front().position.y++;
					body.front().isPlus = false;
				}
				else
					body.front().position.x++;
			}
			else
			{
				if (body.front().position.x <= 0)
				{
					if (body.front().position.y >= 39)
						body.front().position.y = 0;
					else
						body.front().position.y++;
					body.front().isPlus = true;
				}
				else
					body.front().position.x--;
			}
			break;
		}
		case kVertical:
		{
			if (body.front().isPlus)
			{
				if (body.front().position.y >= 39)
				{
					if (body.front().position.x >= 39)
						body.front().position.x = 0;
					else
						body.front().position.x++;
					body.front().isPlus = false;
				}
				else
					body.front().position.y++;
			}
			else
			{
				if (body.front().position.y <= 0)
				{
					if (body.front().position.x >= 39)
						body.front().position.x = 0;
					else
						body.front().position.x++;
					body.front().isPlus = true;
				}
				else
					body.front().position.y--;
			}
			break;
		}
		case kRect:
			body.front().moveState |= kRectWidth;
			break;
		case kStop:
			break;
		default:
			if (body.front().moveState & (kRectWidth))
			{
				if (body.front().isPlus)
				{
					if (body.front().position.x >= 39)
						body.front().position.x = 0;
					else
						body.front().position.x++;
				}
				else
				{
					if (body.front().position.x <= 0)
						body.front().position.x = 39;
					else
						body.front().position.x--;
				}

				body.front().rectMoveCnt++;
				if (body.front().rectMoveCnt % 3 == 0)
					body.front().moveState = (body.front().moveState & ~kRectWidth) | kRectHeight;
				if (body.front().rectMoveCnt >= 6)
				{
					body.front().isPlus = body.front().isPlus ? false : true;
					body.front().rectMoveCnt = 0;
				}
			}
			else if (body.front().moveState & (kRectHeight))
			{
				if (body.front().isPlus)
				{
					if (body.front().position.y >= 39)
						body.front().position.y = 0;
					else
						body.front().position.y++;
				}
				else
				{
					if (body.front().position.y <= 0)
						body.front().position.y = 39;
					else
						body.front().position.y--;
				}

				body.front().rectMoveCnt++;
				if (body.front().rectMoveCnt % 3 == 0)
					body.front().moveState = body.front().moveState & ~kRectHeight | kRectWidth;
				if (body.front().rectMoveCnt >= 6)
				{
					body.front().isPlus = body.front().isPlus ? false : true;
					body.front().rectMoveCnt = 0;
				}
			}
			break;
		}
		if(body.size() > 1 && body.front().moveState != kStop){
			for (auto iter = (++body.begin()); iter != body.end(); ++iter)
			{
				iter->prevPosition = iter->position;
				auto tmp = (--iter)->prevPosition; ++iter;
				iter->position = tmp;
			}
		}
	}

	void SnakeGame::collide()
	{
		for (auto iter = feeds.begin(); iter != feeds.end();)
		{
			if (player.position.x == iter->position.x && player.position.y == iter->position.y)
			{
				switch (uid(randomEngine) % 4)
				{
				case 0:
					iter->moveState = kHorizon;
					break;
				case 1:
					iter->moveState = kVertical;
					break;
				case 2:
					iter->moveState = kRect;
					break;
				case 3:
					iter->moveState = kStop;
					break;
				default:
					break;
				}
				uid(randomEngine) % 2 == 0 ? iter->isPlus = false : iter->isPlus = true;
				iter->size = 100;
				snakeList.push_back(std::list<Object>(1, *iter));
				iter = feeds.erase(iter);
			}
			else
				iter++;
		}

	}

	void SnakeGame::catchSnake()
	{
		if(!(snakeList.begin() + 1)->empty())
		{
			for (auto iter = snakeList.begin() + 1; iter != snakeList.end();)//움직이는 뱀의 머리체크용 + player와 충돌체크 뱀
			{
				for (auto inIter = snakeList.begin() + 1; inIter != snakeList.end();)//다른 뱀
				{
					if (*inIter == *iter)
					{
						inIter++;
						continue;
					}

					auto collideIter = std::find_if(inIter->begin(), inIter->end(), [iter](Object& comp) {
						return comp.position.x == iter->front().position.x && comp.position.y == iter->front().position.y;
						});//뱀의 머리와 위치가 같은(충돌한) 다른 뱀의 몸통부분
					if (inIter->end() != collideIter)
					{
						iter->splice(++(iter->begin()), *inIter, collideIter, inIter->end());//움직이는 뱀에 충돌한 부분부터 그 뱀의 끝부분까지 움직이는 뱀의 몸통 시작부분에 삽입
						switch (uid(randomEngine) % 4)
						{
						case 0:
							iter->front().moveState = kHorizon;
							break;
						case 1:
							iter->front().moveState = kVertical;
							break;
						case 2:
							iter->front().moveState = kRect;
							break;
						case 3:
							iter->front().moveState = kStop;
							break;
						default:
							break;
						}
						uid(randomEngine) % 2 == 0 ? iter->front().isPlus = false : iter->front().isPlus = true;
					}
					if (inIter->empty())
						inIter = snakeList.erase(inIter);//삭제되었으니 다음 iter를 찾아 그 iter로 다음진행
					else
						inIter++;//그냥 다음 iter진행
				}

				auto& playerVal = player;
				auto collideIter = std::find_if(iter->begin(), iter->end(), [playerVal](Object& comp) {
					return comp.position.x == playerVal.position.x && comp.position.y == playerVal.position.y;
					});//뱀의 머리와 위치가 같은(충돌한) 다른 뱀의 몸통부분
				if (iter->end() != collideIter)
					snakeList.front().splice(++(snakeList.front().begin()), *iter, collideIter, iter->end());//움직이는 뱀에 충돌한 부분부터 그 뱀의 끝부분까지
				//움직이는 뱀의 몸통 시작부분에 삽입
				if (iter->empty())
					iter = snakeList.erase(iter);//삭제되었으니 다음 iter를 찾아 그 iter로 다음진행
				else
					iter++;//그냥 다음 iter진행
			}
		}

		//충돌 체크 헤드 - player, snakeList[index][0]
		//충돌 체크 당함 - player -> feeds --------> snakeList.push_back(*iter), feeds.erase(*iter)
		// 
		//충돌 체크 당함 - snakeList[index1(~0)][0] -> snakeList[(index2(~0, ~index1))][index]
		// ------> snakeList[index1].splice(snakeList[index1].begin(), snakeList[(index2(~0, ~index1))], iter,snakeList[(index2(~0, ~index1))].end())
		//충돌 체크 당함 - player -> snakeList[(index(~0))][index] ----------> snakeList[0].splice(snakeList[0].begin(), snakeList[(index(~0))],
		//																		iter, snakeList[(index(~0))].end())
		//
		//snakeList[index].empty()면 snakeList.erase(index);

	}
}


