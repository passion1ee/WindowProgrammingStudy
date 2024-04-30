#include "Yutnori.h"
#include <iostream>
#include <cmath>
#include <sstream>
using namespace std;

namespace ys
{
	void Yutnori::Init(HWND hWnd_, RECT screenSize)
	{
		hWnd = hWnd_;
		hDc = GetDC(hWnd_);
		hBackDc = CreateCompatibleDC(hDc);

		setScreen(screenSize);

		for (int i = 0; i < static_cast<int>(Color::quantity); ++i)
			colors.push_back(static_cast<Color>(i));

		InputManager::Init();
		Timer::Init();
	}

	void Yutnori::Set()
	{
		static int select{};

		auto brush = CreateSolidBrush(RGB(88, 64, 48));
		auto oldBrush = SelectObject(hBackDc, brush);
		Rectangle(hBackDc, 0, 0, width, height);
		SelectObject(hBackDc, oldBrush);
		DeleteObject(brush);

		brush = CreateSolidBrush(RGB(244, 225, 154));
		oldBrush = SelectObject(hBackDc, brush);
		Rectangle(hBackDc, width / 10, height / 10, width * 9 / 10, height * 9 / 10);
		SelectObject(hBackDc, oldBrush);
		DeleteObject(brush);

		float menuX{ width * 8 / 10 };
		float menuY{ height * 8 / 10 };

		std::wstring line = to_wstring(isTwoTurn ? 1 : 0) + L"번 유저의 돌을 선택하세요!";
		SIZE size;  GetTextExtentPoint32(hBackDc, line.c_str(), line.size(), &size);
		TextOut(hBackDc, width / 2 - size.cx / 2, height / 10, line.c_str(), line.size());

		brush = CreateSolidBrush(getColor(setColor));
		oldBrush = SelectObject(hBackDc, brush);
		if(setSymbol != Symbol::circle)
			Ellipse(hBackDc, width / 10 + menuX / 5, height / 10 + menuY / 3, width / 10 + menuX * 2 / 5, height / 10 + menuY * 2 / 3);
		Vector2 center{ (width / 5 + menuX * 3 / 5) / 2, (height / 5 + menuY) / 2 };
		float radius{ menuX / 10 };

		RECT box{ width / 10 + menuX * 3 / 5, height / 10 + menuY / 3, width / 10 + menuX * 4 / 5, height / 10 + menuY * 2 / 3 };
		if (setSymbol != Symbol::Hash)
			Rectangle(hBackDc, box.left, box.top, box.right, box.bottom);
		SelectObject(hBackDc, oldBrush);
		DeleteObject(brush);
		if (InputManager::getKeyDown(VK_LEFT))
		{
			if (select <= 0)
				select = colors.size() - 1;
			else
				select--;
			setColor = colors[select];
		}
		if (InputManager::getKeyDown(VK_RIGHT))
		{
			if (select >= colors.size() - 1)
				select = 0;
			else
				select++;
			setColor = colors[select];
		}
		if (InputManager::getKeyDown(VK_LBUTTON) && setColor != Color::dump)
		{
			bool isSetPlayer = false;
			if (circlePointCollide(center, radius, mousePosition) && setSymbol != Symbol::circle)
			{
				setSymbol = Symbol::circle;
				isSetPlayer = true;
			}
			if (PtInRect(&box, POINT(static_cast<int>(mousePosition.x), static_cast<int>(mousePosition.y))) && setSymbol != Symbol::Hash)
			{
				setSymbol = Symbol::Hash;
				isSetPlayer = true;
			}
			if (isSetPlayer)
			{
				if (isTwoTurn)
					players.second = Player(setSymbol, setColor, 480);
				else
					players.first = Player(setSymbol, setColor, 240);
				colors.erase(colors.begin() + select);
				if (select >= colors.size() - 1)
					select = 0;
				else
					select++;
				setColor = colors[select];
				isTwoTurn = isTwoTurn ? false : true;
			}
		}

		if (colors.size() < 3)
			isSet = false;
	}

	void Yutnori::Run()
	{
		InputManager::BeforeUpdate();
		Timer::Update();

		if (Timer::getDeltaTime() > 0.0f && !isSet) {
			Player* turnPlayer;
			Player* opponentPlayer;
			if (isTwoTurn) {
				turnPlayer = &players.second;
				opponentPlayer = &players.first;
			}
			else {
				turnPlayer = &players.first;
				opponentPlayer = &players.second;
			}
			static std::vector<int> ablePos;//이동가능 위치들
			static int prevStoneId;//이동 전(현재) 위치
			static int prevQuan;//선택한 돌의 돌 개수

			Render();//업데이트 전 or 나중업데이트 후
			Update(*turnPlayer, *opponentPlayer, ablePos, prevStoneId, prevQuan);//안에서 Render가 한번 이루어짐;;ㅠㅠㅠㅠ
			LateUpdate(*turnPlayer, *opponentPlayer, ablePos, prevStoneId, prevQuan);

			if (InputManager::getKeyDown((UINT)Key::S))
			{
				isSet = true;
				isTwoTurn = false;

				while (!rollHistory.empty()) rollHistory.pop();
				board.clear();
				mousePosition = { -1, -1 };

				colors.clear();
				for (int i = 0; i < static_cast<int>(Color::quantity); ++i)
					colors.push_back(static_cast<Color>(i));
				setSymbol = Symbol::dump;
				setColor = Color::dump;
			}
			if (InputManager::getKeyDown((UINT)Key::Q))
			{
				PostMessage(hWnd, WM_CLOSE, 0, 0);
			}
		}
		else if(isSet)
		{
			PatBlt(hBackDc, 0, 0, screen.right - screen.left, screen.bottom - screen.top, WHITENESS);
			Set();
			BitBlt(hDc, 0, 0, screen.right - screen.left, screen.bottom - screen.top, hBackDc, 0, 0, SRCCOPY);
			InputManager::AfterUpdate();
		}

		InputManager::AfterUpdate();
	}

	void Yutnori::Update(Player& turnPlayer, Player& opponentPlayer,
		std::vector<int>& ablePos, int& prevStoneId, int& prevQuan)
	{
		if (!isRoll)
		{ 
			if (!castAndnSaveYut(turnPlayer))
				return;
			if (!rollHistory.empty())
			{
				if (rollHistory.back() != YutSticks::kYut && rollHistory.back() != YutSticks::kMo)
					isRoll = true;
			}
			else
				return;
		}

		if (!rollHistory.empty() && ablePop)
		{
			curYut = rollHistory.front();
			rollHistory.pop();
			ablePop = false;
		}
		else if (rollHistory.empty() && ablePop)
			switchPlayer();

		if (!ablePop)
		{
			auto tmp = turnPlayer.isSameId(-1);
			if (tmp.size() == turnPlayer.curStoneCount() && curYut == YutSticks::kBackDo)
			{
				curYut = YutSticks::kDo;
				while (!rollHistory.empty()) rollHistory.pop();
				switchPlayer();
				return;
			}

			if (InputManager::getKeyDown(VK_LBUTTON) || isMove)
			{
				static int curStoneId;
				if (!isMove)
				{
					prevStoneId = turnPlayer.selectStone(POINT(static_cast<int>(mousePosition.x), static_cast<int>(mousePosition.y)));
					if ((prevStoneId == -1 && curYut == YutSticks::kBackDo) || prevStoneId == -2) return;
					ableSelect = false;

					if (prevStoneId == -1)
						prevQuan = 1;
					else
						prevQuan = board[prevStoneId]->getQuantity();

					yutCount = 0;
					curStoneId = prevStoneId;
					isMove = true;
				}
				mousePosition = { -1, -1 };


				ablePos = board.availablePosition(prevStoneId, static_cast<int>(curYut));//이동 가능한 포지션
				yutCount = static_cast<int>(curYut);
				int nextStoneId;
				if (ablePos.size() > 1)//다른 루트가 있다면 무조건 다른 루트로
					nextStoneId = ablePos.back();
				else
					nextStoneId = ablePos.front();

					isGoalIn = turnPlayer.setStoneCurId(curStoneId, nextStoneId);//일단 Stone의 Id값만 이동 완료, 0이면 이동 후 Stone 삭제 + true반환
				if (yutCount == static_cast<int>(curYut))
					isMove = false;
			}
		}
	}

	void Yutnori::LateUpdate(Player& turnPlayer, Player& opponentPlayer,
		std::vector<int>& ablePos, int& prevStoneId, int& prevQuan)
	{
		if (isMove || ablePop || ableSelect || !isRoll) return;

		int nextStoneId;
		if (ablePos.size() > 1)//다른 루트가 있다면 무조건 다른 루트로
			nextStoneId = ablePos.back();
		else
			nextStoneId = ablePos.front();

		if (isGoalIn) {
			if (turnPlayer.curStoneCount() == 0)
			{
				isGoal = true;
				return;
			}
			board[prevStoneId]->setState(0, Symbol::dump, Color::dump);
		}

		std::vector<int> opponentStonesPos = opponentPlayer.isSameId(nextStoneId);
		//상대의 남은 Stones의 pos값과 움직인 유저의 Stone pos가 같은지 탐색 후 Pos값 저장

		if (opponentStonesPos.size() == 0) {//탐색결과가 없다면 아군 돌이나 빈 공간 ///// 있다면 상대의 돌
			int saveQuan = prevQuan + board[nextStoneId]->getQuantity();

			board[prevStoneId]->setState(0, Symbol::dump, Color::dump);
			board[nextStoneId]->setState(saveQuan, turnPlayer.getSymbol(), turnPlayer.getColor());

		}
		else {//상대돌이 있을때
			for (auto& stonePos : opponentStonesPos)//움직인 후의 Pos에 있는 상대Stones의 pos값을 -1(보드에서 없애기)로 설정
				opponentPlayer.setStoneCurId(stonePos, -1);

			board[prevStoneId]->setState(0, Symbol::dump, Color::dump);
			board[nextStoneId]->setState(prevQuan, turnPlayer.getSymbol(), turnPlayer.getColor());

			isRoll = false;
		}
		ablePop = true;
		ableSelect = true;
	}

	void Yutnori::Render()
	{
		PatBlt(hBackDc, 0, 0, screen.right - screen.left, screen.bottom - screen.top, WHITENESS);

		auto brush = CreateSolidBrush(RGB(206, 162, 75));
		auto oldBrush = SelectObject(hBackDc, brush);
		Rectangle(hBackDc, 0, 0, width, height);
		SelectObject(hBackDc, oldBrush);
		DeleteObject(brush);

		if (isGoal)
		{
			brush = CreateSolidBrush(RGB(244, 225, 154));
			oldBrush = SelectObject(hBackDc, brush);
			Rectangle(hBackDc, width / 10, height / 10, width * 9 / 10, height * 9 / 10);
			SelectObject(hBackDc, oldBrush);
			DeleteObject(brush);

			std::wstring line = to_wstring(isTwoTurn ? 1 : 0) + L"번 유저의 승리!";
			SIZE size;  GetTextExtentPoint32(hBackDc, line.c_str(), line.size(), &size);
			TextOut(hBackDc, width / 2 - size.cx / 2, height / 2, line.c_str(), line.size());
		}
		else
		{
			brush = CreateSolidBrush(RGB(88, 64, 48));
			oldBrush = SelectObject(hBackDc, brush);
			Rectangle(hBackDc, 0, 0, 640, 640);
			SelectObject(hBackDc, oldBrush);
			DeleteObject(brush);

			board.render(hBackDc);

			renderYutQueue();

			players.first.render(hBackDc);
			players.second.render(hBackDc);

		}

		BitBlt(hDc, 0, 0, screen.right - screen.left, screen.bottom - screen.top, hBackDc, 0, 0, SRCCOPY);
		if(isGoal)
		{
			victorySound();
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
	}

	void Yutnori::setScreen(RECT screenSize)
	{
		screen = screenSize;
		width = screen.right - screen.left;
		height = screen.bottom - screen.top;
		SetWindowPos(hWnd, nullptr, 0, 0, screen.right - screen.left, screen.bottom - screen.top, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

		hBitmap = CreateCompatibleBitmap(hDc, screen.right - screen.left, screen.bottom - screen.top);
		auto oldBitmap = SelectObject(hBackDc, hBitmap);
		DeleteObject(oldBitmap);
	}

	void Yutnori::renderYutQueue()//윷 상황판 출력
	{
		queue<YutSticks> tmp = rollHistory;

		Player* turnPlayer = isTwoTurn ? &players.second : &players.first;
		auto brush = CreateSolidBrush(RGB(243, 221, 135));
		auto oldBrush = SelectObject(hBackDc, brush);
		if(!isRoll)
		{
			auto pen = CreatePen(PS_SOLID, 10, getColor(turnPlayer->getColor()));
			auto oldPen = SelectObject(hBackDc, pen);
			Rectangle(hBackDc, 680, 0, 890, 640);
			SelectObject(hBackDc, oldPen);
			DeleteObject(pen);
		}
		else
			Rectangle(hBackDc, 680, 0, 890, 640);
		SelectObject(hBackDc, oldBrush);
		DeleteObject(brush);

		
		if (rollHistory.empty() && ablePop) return;
		renderYutSet(curYut, POINT(680, 20));
		for (int i = 1; i < 4 && !tmp.empty(); ++i)
		{
			renderYutSet(tmp.front(), POINT(680, i * 150 + 20));
			tmp.pop();
		}
		if (!tmp.empty())
		{
			std::wstring line = L"출력되지 못한 윷이 더 있습니다..!";
			SIZE size;  GetTextExtentPoint32(hBackDc, line.c_str(), line.size(), &size);
			TextOut(hBackDc, 680 + (105 - size.cx / 2), 600, line.c_str(), line.size());
		}
		BitBlt(hDc, 650, 0, 900, screen.bottom - screen.top, hBackDc, 0, 0, SRCCOPY);
		//680 890
		//10~5까지 출력
		/*	Rectangle(hBackDc, 0, 0, 130, 130);//10
		for(int i = 0; i < 4; i++)//9 8 7 6
			Rectangle(hBackDc, i * 90 + 150, 30, i * 90 + 220, 100);
		Rectangle(hBackDc, 510, 0, 640, 130);//5*/
	}

	void Yutnori::click(const int& x, const int& y)
	{
		mousePosition.x = x;
		mousePosition.y = y;
	}

	Player& Yutnori::switchPlayer() {
		isRoll = false;
		isTwoTurn = isTwoTurn ? false : true;
		std::cout << isTwoTurn << "의 차례입니다.\n";
		if (isTwoTurn)
			return players.second;
		else
			return players.first;
	}

	bool Yutnori::castAndnSaveYut(Player& turnPlayer)
	{
		YutSticks curYut;
		if (InputManager::getKeyDown((UINT)Key::Num1) && isTwoTurn == false)
		{
			curYut = turnPlayer.castYut();
			Beep(2000, 100);
			rollHistory.push(curYut);
			return true;
		}
		if (InputManager::getKeyDown((UINT)Key::Num2) && isTwoTurn == true)
		{
			curYut = turnPlayer.castYut();
			Beep(2000, 100);
			rollHistory.push(curYut);
			return true;
		}
		return false;
	}


	COLORREF Yutnori::getColor(const Color& color)
	{
		switch (color)
		{
		case Color::kRed:
			return(RGB(255, 0, 0));
		case Color::kBlue:
			return(RGB(0, 0, 255));
		case Color::kGreen:
			return(RGB(0, 255, 0));
		case Color::kYellow:
			return(RGB(255, 255, 0));
		case Color::dump:
			return(RGB(255, 255, 255));
		case Color::prevMove:
			return(RGB(255, 0, 144));
		case Color::predictedMove:
			return(RGB(0, 183, 235));
		default:
			break;
		}
	}

	void Yutnori::renderYutSet(const YutSticks& yut, const POINT& position)
	{
		switch (yut) {
		case YutSticks::kDo:
		{
			auto setPosition = position;
			renderYut(setPosition, false);
			setPosition.x += 50;
			renderYut(setPosition, false);
			setPosition.x += 50;
			renderYut(setPosition, false);
			setPosition.x += 50;
			renderYut(setPosition, true);
			return;
		}
		case YutSticks::kGae:
		{
			auto setPosition = position;
			renderYut(setPosition, false);
			setPosition.x += 50;
			renderYut(setPosition, false);
			setPosition.x += 50;
			renderYut(setPosition, true);
			setPosition.x += 50;
			renderYut(setPosition, true);
			return;
		}
		case YutSticks::kGeol:
		{
			auto setPosition = position;
			renderYut(setPosition, false);
			setPosition.x += 50;
			renderYut(setPosition, true);
			setPosition.x += 50;
			renderYut(setPosition, true);
			setPosition.x += 50;
			renderYut(setPosition, true);
			return;
		}
		case YutSticks::kYut:
		{
			auto setPosition = position;
			renderYut(setPosition, true);
			setPosition.x += 50;
			renderYut(setPosition, true);
			setPosition.x += 50;
			renderYut(setPosition, true);
			setPosition.x += 50;
			renderYut(setPosition, true);
			return;
		}
		case YutSticks::kMo:
		{
			auto setPosition = position;
			renderYut(setPosition, false);
			setPosition.x += 50;
			renderYut(setPosition, false);
			setPosition.x += 50;
			renderYut(setPosition, false);
			setPosition.x += 50;
			renderYut(setPosition, false);
			return;
		}
		case YutSticks::kBackDo:
		{
			auto setPosition = position;
			renderYut(setPosition, false);
			setPosition.x += 50;
			renderYut(setPosition, false);
			setPosition.x += 50;
			renderYut(setPosition, false);
			setPosition.x += 50;
			Rectangle(hBackDc, setPosition.x + 10, position.y, setPosition.x + 50, position.y + 130);
			MoveToEx(hBackDc, setPosition.x + 15, position.y + 125, NULL);
			LineTo(hBackDc, setPosition.x + 45, position.y + 5);
			return;
		}
		default:
			return;
		}
	}

	void Yutnori::renderYut(const POINT& position, bool isFront)
	{
		HBRUSH brush;
		HGDIOBJ oldBrush;
		auto pen = CreatePen(PS_SOLID, 3, RGB(0,0,0));
		auto oldPen = SelectObject(hBackDc, pen);
		if(isFront)
		{
			brush = CreateSolidBrush(RGB(226, 204, 181));
			oldBrush = SelectObject(hBackDc, brush);
			Rectangle(hBackDc, position.x + 10, position.y, position.x + 50, position.y + 130);
		}
		else
		{
			brush = CreateSolidBrush(RGB(178, 135, 93));
			oldBrush = SelectObject(hBackDc, brush);
			Rectangle(hBackDc, position.x + 10, position.y, position.x + 50, position.y + 130);
			for (int i = 0; i < 4; ++i)
			{
				MoveToEx(hBackDc, position.x + 20, position.y + 30 * i + 10, NULL);
				LineTo(hBackDc, position.x + 40, position.y + 30 * i + 30);
				MoveToEx(hBackDc, position.x + 40, position.y + 30 * i + 10, NULL);
				LineTo(hBackDc, position.x + 20, position.y + 30 * i + 30);
			}
		}
		SelectObject(hBackDc, oldPen);
		DeleteObject(pen);
		SelectObject(hBackDc, oldBrush);
		DeleteObject(brush);
	}

	void Yutnori::victorySound()
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