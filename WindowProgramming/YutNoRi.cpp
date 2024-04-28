#include "Yutnori.h"
#include <iostream>
#include <cmath>
#include <sstream>
using namespace std;

constexpr short kYutSize = 269;
namespace ys
{
	/*Yutnori::Yutnori(Renderer& renderer_) : renderer(renderer_) {
		std::vector<Symbol> symbols;
		for (int i = 0; i < static_cast<int>(Symbol::quantity); ++i)
			symbols.push_back(static_cast<Symbol>(i));

		std::vector<Color> colors;
		for (int i = 0; i < static_cast<int>(Color::quantity); ++i)
			colors.push_back(static_cast<Color>(i));

		Symbol setSymbol;
		Color setColor;

		for (int playerNumber = 0; playerNumber < 2; ++playerNumber) {
			std::cout << "\x1b[H";
			while (true) {
				std::string tmpSymbol;
				std::cout << "\x1b[2J" << "\x1b[" << std::to_string(symbols.size() + 2) << "A";
				std::cout << isTwoTurn << "�� ������ Symbol�� �����ϼ���." << std::endl;
				for (int i = 0; i < symbols.size(); ++i)
					std::cout << i << ". " << renderer.symbolToString(symbols[i]) << "\n";
				std::cin >> tmpSymbol;

				bool isVal{ true };
				for (auto arr : tmpSymbol) {
					if (!isdigit(arr)) {
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						isVal = false;
					}
				}
				if (!isVal)
					continue;
				auto NumSymbol = stoi(tmpSymbol);
				if (!(0 > NumSymbol || NumSymbol > symbols.size())) {
					setSymbol = symbols[NumSymbol];
					symbols.erase(symbols.begin() + NumSymbol);
					break;
				}
			}

			std::cout << "\x1b[H";
			while (true) {
				std::string tmpColor;
				std::cout << "\x1b[2J" << "\x1b[" << std::to_string(colors.size() + 2) << "A";
				std::cout << isTwoTurn << "�� ������ Color�� �����ϼ���." << std::endl;
				for (int i = 0; i < colors.size(); ++i)
					std::cout << i << ". " << renderer.setColor(colors[i], "����") << "\n";
				std::cin >> tmpColor;

				bool isVal{ true };
				for (auto arr : tmpColor) {
					if (!isdigit(arr)) {
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						isVal = false;
					}
				}
				if (!isVal)
					continue;
				auto NumColor = stoi(tmpColor);
				if (!(0 > NumColor || NumColor > colors.size())) {
					setColor = colors[NumColor];
					colors.erase(colors.begin() + NumColor);
					break;
				}
			}

			if (isTwoTurn)
				players.second = Player(setSymbol, setColor);
			else
				players.first = Player(setSymbol, setColor);
			isTwoTurn = isTwoTurn ? false : true;
		}
	}*/

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

		Rectangle(hBackDc, width / 10, height / 10, width * 9 / 10, height * 9 / 10);
		float menuX{ width * 8 / 10 };
		float menuY{ height * 8 / 10 };

		std::wstring line = std::to_wstring(static_cast<int>(setColor)) + L"�� ������ ���� �����ϼ���!";
		SIZE size;  GetTextExtentPoint32(hBackDc, line.c_str(), line.size(), &size);
		TextOut(hBackDc, width / 2 - size.cx / 2, height / 10, line.c_str(), line.size());

		auto brush = CreateSolidBrush(getColor(setColor));
		auto oldBrush = SelectObject(hBackDc, brush);
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
			if (circlePointCollide(center, radius, mousePosition))
			{
				setSymbol = Symbol::circle;
				isSetPlayer = true;
			}
			if (PtInRect(&box, POINT(static_cast<int>(mousePosition.x), static_cast<int>(mousePosition.y))))
			{
				setSymbol = Symbol::Hash;
				isSetPlayer = true;
			}
			if (isSetPlayer)
			{
				if (isTwoTurn)
					players.second = Player(setSymbol, setColor);
				else
					players.first = Player(setSymbol, setColor);
				colors.erase(colors.begin() + select);
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
			Update();
			Render();
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

	void Yutnori::Update()
	{
	}

	void Yutnori::Render()
	{
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

	void Yutnori::click(const int& x, const int& y)
	{
		mousePosition.x = x;
		mousePosition.y = y;
	}

	bool Yutnori::getWhoTurn() const
	{
		return isTwoTurn;
	}

	Player& Yutnori::switchPlayer() {
		isTwoTurn = isTwoTurn ? false : true;
		std::cout << isTwoTurn << "�� �����Դϴ�.\n";
		if (isTwoTurn)
			return players.second;
		else
			return players.first;
	}

	std::pair<Quantity, Quantity> Yutnori::SumQuantity(const std::pair<Quantity, Quantity>& prev, const std::pair<Quantity, Quantity>& next)
	{
		int firstSum = static_cast<int>(next.first) + static_cast<int>(prev.first);
		int secondSum = static_cast<int>(next.second) + static_cast<int>(prev.second);
		if (firstSum > 2)
			return { Quantity::kTwo, static_cast<Quantity>(secondSum + firstSum - 2) };
		else
			return { static_cast<Quantity>(firstSum), static_cast<Quantity>(secondSum) };
	}

	//std::string Yutnori::castAndnSaveYut(Player& turnPlayer)
	//{
	//	YutSticks curYut;
	//	std::stringstream output;
	//	while (true) {
	//		std::string tmpTrigger;
	//		std::cout << "���� �����÷��� 0�������� 1, 1�������� 2�� �Է��ϼ���:";
	//		std::cin >> tmpTrigger;

	//		bool isVal{ true };
	//		for (auto arr : tmpTrigger) {
	//			if (!isdigit(arr)) {
	//				isVal = false;
	//				break;
	//			}
	//		}
	//		if (!isVal) {
	//			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	//			std::cout << "\x1b[1A" << "\x1b[J";
	//			continue;
	//		}
	//		int trigger = stoi(tmpTrigger);
	//		--trigger;
	//		if (trigger == isTwoTurn) {
	//			curYut = turnPlayer.castYut();
	//			/*int cheet{};
	//			std::cin >> cheet;
	//			switch (cheet) {
	//			case 1:
	//				curYut = YutSticks::kDo;
	//				break;
	//			case 2:
	//				curYut = YutSticks::kGae;
	//				break;
	//			case 3:
	//				curYut = YutSticks::kGeol;
	//				break;
	//			case 4:
	//				curYut = YutSticks::kYut;
	//				break;
	//			case 5:
	//				curYut = YutSticks::kMo;
	//				break;
	//			case -1:
	//				curYut = YutSticks::kBackDo;
	//				break;
	//			}*/
	//			std::cout << renderer.renderYut(curYut);
	//			output << renderer.renderYut(curYut);
	//			rollHistory.push(curYut);
	//			if (curYut != YutSticks::kYut && curYut != YutSticks::kMo)
	//				return output.str();
	//		}
	//		else {
	//			std::cout << "���ʰ� �ƴմϴ�.\n" << "\x1b[1A";
	//		}
	//	}
	//}

	//bool Yutnori::advanceTurn() {
	//	Player* turnPlayer;
	//	Player* opponentPlayer;
	//	if (isTwoTurn) {
	//		turnPlayer = &players.second;
	//		opponentPlayer = &players.first;
	//	}
	//	else {
	//		turnPlayer = &players.first;
	//		opponentPlayer = &players.second;
	//	}

	//	renderer.Render(board);
	//	YutSticks curYut;
	//	std::stringstream output;
	//	output << castAndnSaveYut(*turnPlayer);

	//	while (!rollHistory.empty()) {
	//		std::cout << "\x1b[2J" << "\x1b[H";
	//		renderer.Render(board);
	//		curYut = rollHistory.front();
	//		rollHistory.pop();
	//		std::string curYutRender = output.str().substr(0, kYutSize);
	//		std::string tmp = output.str().substr(kYutSize);
	//		output.str("");
	//		output.clear();
	//		output << tmp;
	//		std::cout << "------���� ������: \n" << output.str();
	//		std::cout << "------�̹� ������: \n" << curYutRender;

	//		std::vector<int> ablePos;
	//		int curStonePos;
	//		std::pair<Quantity, Quantity> prevQuan;
	//		while (true) {//�� ���� �� �̵� ������ �� �˻� �� ���� ����ְ� �� �̵� Ȯ���ޱ�
	//			auto tmp = turnPlayer->isSamePos(-1);
	//			if (tmp.size() == turnPlayer->stoneQuantity() && curYut == YutSticks::kBackDo)
	//				return false;

	//			do {//�� ����
	//				curStonePos = turnPlayer->selectStone();
	//			} while (curStonePos == -1 && curYut == YutSticks::kBackDo);

	//			if (curStonePos == -1)
	//				prevQuan = { Quantity::kOne, Quantity::kEmpty };
	//			else
	//				prevQuan = board[curStonePos]->getQuantity();

	//			ablePos = board.availablePosition(curStonePos, curYut);//�̵� ������ ������ ã��

	//			/*�̵� ������ ������ ����ֱ�*/
	//			if (curStonePos == -1)
	//				board[0]->setState({ Quantity::kOne, Quantity::kEmpty }, turnPlayer->getSymbol(), Color::prevMove);
	//			else
	//				board[curStonePos]->setColor(Color::prevMove);//�����̱� �� ��ġ�� �����ϱ�

	//			std::vector<std::pair<Quantity, Quantity>> tmpQuan;
	//			std::vector<Symbol> tmpShape;
	//			std::vector<Color> tmpColor;
	//			for (auto pos : ablePos) {//�̵����� Pos�� State�� tmp�� ����
	//				tmpQuan.push_back(board[pos]->getQuantity());
	//				tmpShape.push_back(board[pos]->getShape());
	//				tmpColor.push_back(board[pos]->getColor());
	//			}

	//			for (auto pos : ablePos) {//�̵����� Pos�� �����ϸ� �̸� �����ֱ�
	//				if (board[curStonePos]->getShape() == board[pos]->getShape()) {
	//					std::pair<Quantity, Quantity> saveQuan = SumQuantity(prevQuan, board[pos]->getQuantity());

	//					board[pos]->setState(saveQuan, turnPlayer->getSymbol(), Color::predictedMove);
	//				}
	//				else {
	//					std::pair<Quantity, Quantity> prevQuan;
	//					if (curStonePos == -1)
	//						prevQuan = { Quantity::kOne, Quantity::kEmpty };
	//					else
	//						prevQuan = board[curStonePos]->getQuantity();
	//					board[pos]->setState(prevQuan, turnPlayer->getSymbol(), Color::predictedMove);
	//				}
	//			}
	//			renderer.Render(board);//�̵�������ġ �����ֱ�

	//			if (curStonePos == -1)
	//				board[0]->setState({ Quantity::kEmpty, Quantity::kEmpty }, Symbol::dump, Color::dump);//���� ������ ������(���� ����� ����)
	//			else
	//				board[curStonePos]->setColor(turnPlayer->getColor());//���� ������ ������(���� ����� ����)

	//			for (int i = 0; i < ablePos.size(); ++i)//�̵����� Pos�� ���� State�� ������(���� ����� ����)
	//				board[ablePos[i]]->setState(tmpQuan[i], tmpShape[i], tmpColor[i]);

	//			bool isSelect{ false };
	//			while (true) {
	//				std::string tmpSelect;
	//				std::cout << "���� �� ���� �����̽ðڽ��ϱ�?: 1. ��		2.�ٽ� �����ϰڽ��ϴ�. \n :  ";
	//				std::cin >> tmpSelect;
	//				bool isVal{ true };
	//				for (auto arr : tmpSelect) {
	//					if (!isdigit(arr)) {
	//						isVal = false;
	//						break;
	//					}
	//				}
	//				if (!isVal) {
	//					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	//					std::cout << "\x1b[2A" << "\x1b[J";
	//					continue;
	//				}

	//				int select = stoi(tmpSelect);
	//				if (select == 1) {
	//					isSelect = true;
	//					break;
	//				}
	//				else if (select == 2) {
	//					std::cout << "\x1b[2J" << "\x1b[H";
	//					renderer.Render(board);
	//					std::cout << "------���� ������: \n" << output.str();
	//					std::cout << "------�̹� ������: \n" << curYutRender;
	//					break;
	//				}
	//			}
	//			if (isSelect)
	//				break;
	//		}

	//		int nextStonePos;

	//		if (ablePos.size() > 1)
	//			nextStonePos = ablePos.back();
	//		else
	//			nextStonePos = ablePos.front();

	//		bool isGoalIn = turnPlayer->setStonePos(curStonePos, nextStonePos);//�ϴ� Stone�� Pos���� �̵� �Ϸ�, 0�̸� �̵� �� Stone ���� + true��ȯ
	//		if (isGoalIn) {
	//			if (turnPlayer->stoneQuantity())
	//				return true;
	//			board[curStonePos]->setState({ Quantity::kEmpty, Quantity::kEmpty }, Symbol::dump, Color::dump);
	//			continue;
	//		}

	//		std::vector<int> opponentStonesPos = opponentPlayer->isSamePos(nextStonePos);
	//		//����� ���� Stones�� pos���� ������ ������ Stone pos�� ������ Ž�� �� Pos�� ����

	//		if (opponentStonesPos.size() == 0) {//Ž������� ���ٸ� �Ʊ� ���̳� �� ���� ///// �ִٸ� ����� ��
	//			std::pair<Quantity, Quantity> saveQuan = SumQuantity(prevQuan, board[nextStonePos]->getQuantity());

	//			board[curStonePos]->setState({ Quantity::kEmpty, Quantity::kEmpty }, Symbol::dump, Color::dump);
	//			board[nextStonePos]->setState(saveQuan, turnPlayer->getSymbol(), turnPlayer->getColor());

	//		}
	//		else {//��뵹�� ������
	//			for (auto& stonePos : opponentStonesPos)//������ ���� Pos�� �ִ� ���Stones�� pos���� -1(���忡�� ���ֱ�)�� ����
	//				opponentPlayer->setStonePos(stonePos, -1);

	//			board[curStonePos]->setState({ Quantity::kEmpty, Quantity::kEmpty }, Symbol::dump, Color::dump);
	//			board[nextStonePos]->setState(prevQuan, turnPlayer->getSymbol(), turnPlayer->getColor());

	//			std::cout << "\x1b[2J" << "\x1b[H";
	//			renderer.Render(board);
	//			std::cout << "��븦 ��ҽ��ϴ�! \n";

	//			output << castAndnSaveYut(*turnPlayer);
	//		}
	//	}

	//	return false;
	//}

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

}