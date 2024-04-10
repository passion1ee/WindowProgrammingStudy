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
		: hWnd(nullptr), hDC(nullptr), hBackDC(nullptr), hBitmap(nullptr), screen({ 0, 0 }), xGrid(0), yGrid(0), frameCheck(0), keyCoolTime(0)
	{
		std::uniform_int_distribution<> position(0, kMapSize - 1);
		player = Object(0, 100, { 0, 0 }, RGB(ObjectColor(randomEngine), ObjectColor(randomEngine), ObjectColor(randomEngine)), kHorizon, true);
		for (int i = 0; i < 20;)
		{
			auto x = position(randomEngine); auto y = position(randomEngine);
			if (feeds.end() != std::find(feeds.begin(), feeds.end(), [x, y](Object& comp) {return comp.position.x == x && comp.position.y == y; }) || (x == 0 && y == 0))
				continue;
			feeds.push_back(Object(0, 100, {position(randomEngine), position(randomEngine)},
				RGB(ObjectColor(randomEngine), ObjectColor(randomEngine), ObjectColor(randomEngine)), kStop, true));
			++i;
		}

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

		keyCoolTime = 0;
		Timer::Init();
	}

	void SnakeGame::Run()
	{
		static bool isRun = false;
		InputManager::BeforeUpdate();
		Timer::Update();
		if (InputManager::getKeyUp((UINT)Key::S) || isRun) {
			if (1 / Timer::getRealFPS() <= (frameCheck += Timer::getDeltaTime())) {
				frameCheck = 0.0f;
				Update();
				Render();
			}
			isRun = true;
		}
		InputManager::AfterUpdate();
	}

	void SnakeGame::Render()
	{
		PatBlt(hBackDC, 0, 0, screen.x, screen.y, WHITENESS);
		
		renderFrame();


		BitBlt(hDC, 0, 0, screen.x, screen.y, hBackDC, 0, 0, SRCCOPY);
	}

	void SnakeGame::Update()
	{
		if (InputManager::getKey(VK_LEFT))
		{
			player.moveState = kHorizon;
			player.isPlus = false;
		}
		if (InputManager::getKey(VK_RIGHT))
		{
			player.moveState = kHorizon;
			player.isPlus = true;
		}
		if (InputManager::getKey(VK_UP))
		{
			player.moveState = kVertical;
			player.isPlus = false;
		}
		if (InputManager::getKey(VK_DOWN))
		{
			player.moveState = kVertical;
			player.isPlus = true;
		}

		for (auto& list : snakeList)
		{
			if (list == snakeList[0]) continue;
			move(list.front(), list);
		}
		move(player, snakeList[0]);
		collide();
		catchSnake();

		if (ys::InputManager::getKeyUp((UINT)ys::Key::Q))
		{
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
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

	void SnakeGame::renderObject(Object& object)
	{
	}

	void SnakeGame::move(Object& head, std::list<Object> body)
	{	
		head.prevPosition = head.position;
		switch (head.moveState)
		{
		case kHorizon:
		{
			if(head.isPlus)
			{
				if(head.position.x == 39)
				{
					head.position.y++;
					head.isPlus = false;
				}
				else
					head.position.x++;
			}
			else
			{
				if (head.position.x == 0)
				{
					head.position.y--;
					head.isPlus = true;
				}
				else
					head.position.x--;
			}
			break;
		}
		case kVertical:
		{
			if (head.isPlus)
			{
				if (head.position.y == 39)
				{
					head.position.x++;
					head.isPlus = false;
				}
				else
					head.position.y++;
			}
			else
			{
				if (head.position.y == 0)
				{
					head.position.x--;
					head.isPlus = true;
				}
				else
					head.position.y--;
			}
			break;
		}
		case kRect:
			head.moveState |= kRectWidth;
			break;
		case kStop:
			break;
		default:
			if (head.moveState & (kRect | kRectWidth))
			{
				head.isPlus ? head.position.x++ : head.position.x--;
				head.rectMoveCnt++;
				if (head.rectMoveCnt >= 3)
				{
					head.isPlus ? true : false;
					head.moveState = (head.moveState & ~kRectWidth) | kRectHeight;
					head.rectMoveCnt = 0;
				}
			}
			else if (head.moveState & (kRect | kRectHeight))
			{
				head.isPlus ? head.position.y++ : head.position.y--;
				head.rectMoveCnt++;
				if (head.rectMoveCnt >= 3)
				{
					head.isPlus ? false : true;
					head.moveState = head.moveState & ~kRectHeight | kRectWidth;
					head.rectMoveCnt = 0;
				}
			}
			break;
		}
		body.begin()->position = head.prevPosition;
		for (auto& iter = ++body.begin(); iter != body.end(); ++iter)
		{
			iter->position = (--iter)->prevPosition; ++iter;
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
				snakeList.push_back(std::list<Object>(1, *iter));
				iter = feeds.erase(iter);
			}
			else
				iter++;
		}

	}

	void SnakeGame::catchSnake()
	{
		for (auto iter = snakeList.begin() + 1; iter != snakeList.end();)//움직이는 뱀의 머리체크용 + player와 충돌체크 뱀
		{
			for (auto inIter = snakeList.begin() + 1; inIter != snakeList.end();)//다른 뱀
			{
				if (*inIter == *iter) continue;

				auto collideIter = std::find(inIter->begin(), inIter->end(), [iter](Object& comp) {
					return comp.position.x == iter->front().position.x && comp.position.y == iter->front().position.y;
				});//뱀의 머리와 위치가 같은(충돌한) 다른 뱀의 몸통부분
				if (inIter->end() != collideIter)
				{
					iter->splice(iter->begin(), *inIter, collideIter, inIter->end());//움직이는 뱀에 충돌한 부분부터 그 뱀의 끝부분까지 움직이는 뱀의 몸통 시작부분에 삽입
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
			auto collideIter = std::find(iter->begin(), iter->end(), [playerVal](Object& comp) {
				return comp.position.x == playerVal.position.x && comp.position.y == playerVal.position.y;
			});//뱀의 머리와 위치가 같은(충돌한) 다른 뱀의 몸통부분
			if (iter->end() != collideIter)
				snakeList.front().splice(snakeList.front().begin(), *iter, collideIter, iter->end());//움직이는 뱀에 충돌한 부분부터 그 뱀의 끝부분까지
																									//움직이는 뱀의 몸통 시작부분에 삽입
			if (iter->empty())
				iter = snakeList.erase(iter);//삭제되었으니 다음 iter를 찾아 그 iter로 다음진행
			else
				iter++;//그냥 다음 iter진행
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


