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
		: hWnd(nullptr), hDC(nullptr), hBackDC(nullptr), hBitmap(nullptr), screen({ 0, 0 }), xGrid(0), yGrid(0), frameCheck(0), moveCoolTime(0), setMoveTime(0), speed(0.00f),
		isRun(false), isJump(false), isScaleIn(false), isScaleOut(false), isBoxErase(false), isFastSeach(false) {}

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
		for (int i = 0; i < 20;)
		{
			auto x = position(randomEngine); auto y = position(randomEngine);
			if (feeds.end() == std::find_if(feeds.begin(), feeds.end(), [x, y](Object& comp) {return comp.position.x == x && comp.position.y == y; }) && (x != 0 && y != 0))
			{
				feeds.push_back(Object(1, 50, { x, y },	RGB(ObjectColor(randomEngine), ObjectColor(randomEngine), ObjectColor(randomEngine)), kStop, true));
				++i;
			}
		}
		snakeList.assign(1, std::list<Object>(1, player));
		moveCoolTime = 0;
		Timer::Init();
	}

	void SnakeGame::Run()
	{
		if (InputManager::getKeyUp((UINT)Key::S) || isRun)
		{
			Timer::Update();
			if (InputManager::getKeyUp((UINT)Key::S))
			{
				setMoveTime = 2 / Timer::getRealFPS();
				isRun = isRun ? false : true;
			}
			moveCoolTime += Timer::getDeltaTime();
			if (1 / Timer::getRealFPS() - speed <= (frameCheck += Timer::getDeltaTime()))
			{
				InputManager::BeforeUpdate();
				frameCheck = 0.0f;
				Update();
				Render();
				Timer::Render(hDC, screen);
				InputManager::AfterUpdate();
			}
		}
	}

	void SnakeGame::Render()
	{
		PatBlt(hBackDC, 0, 0, screen.x, screen.y, WHITENESS);

		renderFrame();

		for (auto& feed : feeds)
		{
			auto brush = CreateSolidBrush(feed.color);
			auto oldBrush = SelectObject(hBackDC, brush);
			renderObject(feed, false);
			SelectObject(hBackDC, oldBrush);
			DeleteObject(brush);
		}

		for (auto& box : boxs)
		{
			auto brush = CreateSolidBrush(box.color);
			auto oldBrush = SelectObject(hBackDC, brush);
			renderObject(box, false);
			SelectObject(hBackDC, oldBrush);
			DeleteObject(brush);
		}

		if (isFastSeach)
		{
			for (auto& obj : fastSnake)
			{
				auto brush = CreateSolidBrush(obj.color);
				auto oldBrush = SelectObject(hBackDC, brush);
				renderObject(obj, true);
				SelectObject(hBackDC, oldBrush);
				DeleteObject(brush);
			}
		}
		else
		{
			for (auto& list : snakeList)
				for (auto& obj : list)
				{
					auto brush = CreateSolidBrush(obj.color);
					auto oldBrush = SelectObject(hBackDC, brush);
					renderObject(obj, true);
					SelectObject(hBackDC, oldBrush);
					DeleteObject(brush);
				}
		}

		BitBlt(hDC, 0, 0, screen.x, screen.y, hBackDC, 0, 0, SRCCOPY);
	}

	void SnakeGame::Update()
	{
		if (ys::InputManager::getKey(VK_ADD) && speed <= 1 / Timer::getRealFPS() / 2) speed += 0.001f;
		if (ys::InputManager::getKey(VK_SUBTRACT) && speed > -1 / Timer::getRealFPS()) speed -= 0.001f;

		if (InputManager::getKeyDown((UINT)ys::Key::A)) 
		{
			for (auto& list : snakeList)
				for (auto& circle : list)
					fastSnake.push_back(circle);
			speed = 1 / Timer::getRealFPS();
			isFastSeach = true;
		}

		if (InputManager::getKeyDown((UINT)ys::Key::E)) isBoxErase = true;
		if (InputManager::getKeyUp((UINT)ys::Key::E)) isBoxErase = false;

		std::cout << speed << std::endl;
		std::cout << isBoxErase << std::endl;

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

		if (InputManager::getKey((UINT)ys::Key::T) && snakeList.front().size() > 1)
		{
			auto firstColor = snakeList.front().front().color;
			auto endTmp = --snakeList.front().end();
			for (auto circle = snakeList.front().begin(); circle != endTmp;)
			{
				auto tmp = (++circle)->color; --circle;
				circle->color = tmp;
				++circle;
			}
			endTmp->color = firstColor;
		}

		if (isFastSeach)
		{
			move(fastSnake);
			if (fastSnake.front().position.x == 0 && fastSnake.front().position.y == 0)
			{
				for (auto& circle : snakeList.front())
					circle.position = { -1, -1 };
				snakeList.front().front().position = { 0, 0 };
				fastSnake.clear();
				speed = 0.00f;
				isFastSeach = false;
			}
		}
		else
		{
			if (setMoveTime <= moveCoolTime)
				for (auto& list : snakeList)
				{
					if (list == snakeList.front()) continue;
					move(list);
				}

			if (isScaleIn)
			{
				if (snakeList.front().front().size > 10 && !isScaleOut)
					snakeList.front().front().size -= 10;
				else
					if(snakeList.front().front().size == 100)
						isScaleIn = false;
					else
					{
						isScaleOut = true;
						snakeList.front().front().size += 10;
					}
			}
			if (isJump)
			{
				isJump = false;
				snakeList.front().front().prevPosition = snakeList.front().front().position;
				if (snakeList.front().front().moveState == kHorizon)
				{
					if (snakeList.front().front().position.y >= 39)
						snakeList.front().front().position.y = 0;
					else
						snakeList.front().front().position.y++;
				}
				else
				{
					if (snakeList.front().front().position.x <= 0)
						snakeList.front().front().position.x = 39;
					else
						snakeList.front().front().position.x--;
				}
			}
			if (ys::InputManager::getKey((UINT)ys::Key::J))
			{
				isJump = true;
				snakeList.front().front().prevPosition = snakeList.front().front().position;
				if (snakeList.front().front().moveState == kHorizon)
				{
					if (snakeList.front().front().position.y <= 0)
						snakeList.front().front().position.y = 39;
					else
						snakeList.front().front().position.y--;
				}
				else
				{
					if (snakeList.front().front().position.x >= 39)
						snakeList.front().front().position.x = 0;
					else
						snakeList.front().front().position.x++;
				}
			}

			move(snakeList.front());
			player = snakeList.front().front();

			if(snakeList.size() > 1)
				catchSnake();
			collide();
		}


		if (ys::InputManager::getKey((UINT)ys::Key::Q))
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

		auto& player = snakeList.front().front();
		for (auto& box : boxs)
		{
			if (box.position.x == player.position.x /*&& box.position.x + 1 >= player.position.x*/ && box.position.y == player.position.y)
				player.isPlus = player.isPlus ? false : true;
		}
	}

	void SnakeGame::catchSnake()
	{
		if((snakeList.begin() + 1)->empty()) return;
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
			{
				Beep(1760, 10);
				Beep(2218, 30);
				snakeList.front().splice(++(snakeList.front().begin()), *iter, collideIter, iter->end());//움직이는 뱀에 충돌한 부분부터 그 뱀의 끝부분까지
			}
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

	void SnakeGame::LButton(int x, int y)
	{
		if (!isRun) return;
		x /= xGrid;	y /= yGrid;
		auto& player = snakeList.front().front();
		for (auto circle = snakeList.front().begin(); circle != snakeList.front().end(); ++circle)
		{
			if (circle->position.x == x && circle->position.y == y && *circle == player)
			{
				isScaleIn = true;
				isScaleOut = false;
				return;
			}
			if (circle->position.x == x && circle->position.y == y)
			{
				//TODO: 클릭된 주인공 원의 꼬리 뒤에 있는 꼬리들도 다 분리된 후 보드의 칸에 남겨지며 이동 방법 4개 중 한 개 방법으로 이동한다. 
				// 분리된 꼬리원은 앞 페이지 이동 방법 4개 중 한 개 방법으로 이동).
				std::list<Object> tmp;
				tmp.splice(tmp.begin(), snakeList.front(), circle, snakeList.front().end());
				snakeList.push_back(tmp);
				return;
			}
		}
		if (player.position.x > x && player.position.y - 10 < y && player.position.y + 10 > y)
		{
			snakeList.front().front().moveState = kHorizon;
			snakeList.front().front().isPlus = false;
		}
		if (player.position.x < x && player.position.y - 10 < y && player.position.y + 10 > y)
		{
			snakeList.front().front().moveState = kHorizon;
			snakeList.front().front().isPlus = true;
		}
		if (player.position.y > y && player.position.x - 10 < x && player.position.x + 10 > x)
		{
			snakeList.front().front().moveState = kVertical;
			snakeList.front().front().isPlus = false;
		}
		if (player.position.y < y && player.position.x - 10 < x && player.position.x + 10 > x)
		{
			snakeList.front().front().moveState = kVertical;
			snakeList.front().front().isPlus = true;
		}
	}

	void SnakeGame::RButton(int x, int y)
	{
		if (!isRun) return;
		x /= xGrid;	y /= yGrid;
		if(isBoxErase)
		{
			auto tmp = std::find_if(boxs.begin(), boxs.end(), [x, y](Object& comp) {return comp.position.x == x && comp.position.y == y; });
			if (boxs.end() != tmp)
				boxs.erase(tmp);
		}
		else if(boxs.size() < 20)
		{
			bool isCollide{ false };

			if (feeds.end() != std::find_if(feeds.begin(), feeds.end(), [x, y](Object& comp) {return comp.position.x == x && comp.position.y == y; }))
				isCollide = true;
			if (boxs.end() != std::find_if(boxs.begin(), boxs.end(), [x, y](Object& comp) {return comp.position.x == x && comp.position.y == y; }))
				isCollide = true;
			for (auto& list : snakeList)
				if (list.end() != std::find_if(list.begin(), list.end(), [x, y](Object& comp) {return comp.position.x == x && comp.position.y == y; }))
					isCollide = true;
			if (!isCollide)
			{
				boxs.push_back(Object(1, 100, { x, y }, RGB(255, 0, 0), kStop, true));
			}
		}
	}
}


