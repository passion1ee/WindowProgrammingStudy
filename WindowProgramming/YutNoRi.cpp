#include "Yutnori.h"
#include <iostream>
#include <sstream>

constexpr short kYutSize = 269;
namespace ys
{
	Yutnori::Yutnori(Renderer& renderer_) : renderer(renderer_) {
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
				std::cout << isTwoTurn << "번 유저의 Symbol을 선택하세요." << std::endl;
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
				std::cout << isTwoTurn << "번 유저의 Color를 선택하세요." << std::endl;
				for (int i = 0; i < colors.size(); ++i)
					std::cout << i << ". " << renderer.setColor(colors[i], "■■■") << "\n";
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
	}

	void Yutnori::Init()
	{

	}

	void Yutnori::Run()
	{
	}

	void Yutnori::Update()
	{
	}

	void Yutnori::Render()
	{
	}

	bool Yutnori::getWhoTurn() const
	{
		return isTwoTurn;
	}

	Player& Yutnori::switchPlayer() {
		isTwoTurn = isTwoTurn ? false : true;
		std::cout << isTwoTurn << "의 차례입니다.\n";
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

	std::string Yutnori::castAndnSaveYut(Player& turnPlayer)
	{
		YutSticks curYut;
		std::stringstream output;
		while (true) {
			std::string tmpTrigger;
			std::cout << "윷을 던지시려면 0번유저는 1, 1번유저는 2를 입력하세요:";
			std::cin >> tmpTrigger;

			bool isVal{ true };
			for (auto arr : tmpTrigger) {
				if (!isdigit(arr)) {
					isVal = false;
					break;
				}
			}
			if (!isVal) {
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "\x1b[1A" << "\x1b[J";
				continue;
			}
			int trigger = stoi(tmpTrigger);
			--trigger;
			if (trigger == isTwoTurn) {
				curYut = turnPlayer.castYut();
				/*int cheet{};
				std::cin >> cheet;
				switch (cheet) {
				case 1:
					curYut = YutSticks::kDo;
					break;
				case 2:
					curYut = YutSticks::kGae;
					break;
				case 3:
					curYut = YutSticks::kGeol;
					break;
				case 4:
					curYut = YutSticks::kYut;
					break;
				case 5:
					curYut = YutSticks::kMo;
					break;
				case -1:
					curYut = YutSticks::kBackDo;
					break;
				}*/
				std::cout << renderer.renderYut(curYut);
				output << renderer.renderYut(curYut);
				rollHistory.push(curYut);
				if (curYut != YutSticks::kYut && curYut != YutSticks::kMo)
					return output.str();
			}
			else {
				std::cout << "차례가 아닙니다.\n" << "\x1b[1A";
			}
		}
	}

	bool Yutnori::advanceTurn() {
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

		renderer.Render(board);
		YutSticks curYut;
		std::stringstream output;
		output << castAndnSaveYut(*turnPlayer);

		while (!rollHistory.empty()) {
			std::cout << "\x1b[2J" << "\x1b[H";
			renderer.Render(board);
			curYut = rollHistory.front();
			rollHistory.pop();
			std::string curYutRender = output.str().substr(0, kYutSize);
			std::string tmp = output.str().substr(kYutSize);
			output.str("");
			output.clear();
			output << tmp;
			std::cout << "------남은 윷가락: \n" << output.str();
			std::cout << "------이번 윷가락: \n" << curYutRender;

			std::vector<int> ablePos;
			int curStonePos;
			std::pair<Quantity, Quantity> prevQuan;
			while (true) {//돌 선택 후 이동 가능한 곳 검사 후 판을 찍어주고 돌 이동 확정받기
				auto tmp = turnPlayer->isSamePos(-1);
				if (tmp.size() == turnPlayer->stoneQuantity() && curYut == YutSticks::kBackDo)
					return false;

				do {//돌 고르기
					curStonePos = turnPlayer->selectStone();
				} while (curStonePos == -1 && curYut == YutSticks::kBackDo);

				if (curStonePos == -1)
					prevQuan = { Quantity::kOne, Quantity::kEmpty };
				else
					prevQuan = board[curStonePos]->getQuantity();

				ablePos = board.availablePosition(curStonePos, curYut);//이동 가능한 포지션 찾기

				/*이동 가능한 포지션 찍어주기*/
				if (curStonePos == -1)
					board[0]->setState({ Quantity::kOne, Quantity::kEmpty }, turnPlayer->getSymbol(), Color::prevMove);
				else
					board[curStonePos]->setColor(Color::prevMove);//움직이기 전 위치를 강조하기

				std::vector<std::pair<Quantity, Quantity>> tmpQuan;
				std::vector<Symbol> tmpShape;
				std::vector<Color> tmpColor;
				for (auto pos : ablePos) {//이동가능 Pos의 State를 tmp에 저장
					tmpQuan.push_back(board[pos]->getQuantity());
					tmpShape.push_back(board[pos]->getShape());
					tmpColor.push_back(board[pos]->getColor());
				}

				for (auto pos : ablePos) {//이동가능 Pos에 강조하며 미리 보여주기
					if (board[curStonePos]->getShape() == board[pos]->getShape()) {
						std::pair<Quantity, Quantity> saveQuan = SumQuantity(prevQuan, board[pos]->getQuantity());

						board[pos]->setState(saveQuan, turnPlayer->getSymbol(), Color::predictedMove);
					}
					else {
						std::pair<Quantity, Quantity> prevQuan;
						if (curStonePos == -1)
							prevQuan = { Quantity::kOne, Quantity::kEmpty };
						else
							prevQuan = board[curStonePos]->getQuantity();
						board[pos]->setState(prevQuan, turnPlayer->getSymbol(), Color::predictedMove);
					}
				}
				renderer.Render(board);//이동가능위치 보여주기

				if (curStonePos == -1)
					board[0]->setState({ Quantity::kEmpty, Quantity::kEmpty }, Symbol::dump, Color::dump);//원래 색으로 돌리기(이후 출력을 위해)
				else
					board[curStonePos]->setColor(turnPlayer->getColor());//원래 색으로 돌리기(이후 출력을 위해)

				for (int i = 0; i < ablePos.size(); ++i)//이동가능 Pos의 기존 State로 돌리기(이후 출력을 위해)
					board[ablePos[i]]->setState(tmpQuan[i], tmpShape[i], tmpColor[i]);

				bool isSelect{ false };
				while (true) {
					std::string tmpSelect;
					std::cout << "정말 이 돌을 움직이시겠습니까?: 1. 네		2.다시 선택하겠습니다. \n :  ";
					std::cin >> tmpSelect;
					bool isVal{ true };
					for (auto arr : tmpSelect) {
						if (!isdigit(arr)) {
							isVal = false;
							break;
						}
					}
					if (!isVal) {
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						std::cout << "\x1b[2A" << "\x1b[J";
						continue;
					}

					int select = stoi(tmpSelect);
					if (select == 1) {
						isSelect = true;
						break;
					}
					else if (select == 2) {
						std::cout << "\x1b[2J" << "\x1b[H";
						renderer.Render(board);
						std::cout << "------남은 윷가락: \n" << output.str();
						std::cout << "------이번 윷가락: \n" << curYutRender;
						break;
					}
				}
				if (isSelect)
					break;
			}

			int nextStonePos;

			if (ablePos.size() > 1)
				nextStonePos = ablePos.back();
			else
				nextStonePos = ablePos.front();

			bool isGoalIn = turnPlayer->setStonePos(curStonePos, nextStonePos);//일단 Stone의 Pos값만 이동 완료, 0이면 이동 후 Stone 삭제 + true반환
			if (isGoalIn) {
				if (turnPlayer->stoneQuantity())
					return true;
				board[curStonePos]->setState({ Quantity::kEmpty, Quantity::kEmpty }, Symbol::dump, Color::dump);
				continue;
			}

			std::vector<int> opponentStonesPos = opponentPlayer->isSamePos(nextStonePos);
			//상대의 남은 Stones의 pos값과 움직인 유저의 Stone pos가 같은지 탐색 후 Pos값 저장

			if (opponentStonesPos.size() == 0) {//탐색결과가 없다면 아군 돌이나 빈 공간 ///// 있다면 상대의 돌
				std::pair<Quantity, Quantity> saveQuan = SumQuantity(prevQuan, board[nextStonePos]->getQuantity());

				board[curStonePos]->setState({ Quantity::kEmpty, Quantity::kEmpty }, Symbol::dump, Color::dump);
				board[nextStonePos]->setState(saveQuan, turnPlayer->getSymbol(), turnPlayer->getColor());

			}
			else {//상대돌이 있을때
				for (auto& stonePos : opponentStonesPos)//움직인 후의 Pos에 있는 상대Stones의 pos값을 -1(보드에서 없애기)로 설정
					opponentPlayer->setStonePos(stonePos, -1);

				board[curStonePos]->setState({ Quantity::kEmpty, Quantity::kEmpty }, Symbol::dump, Color::dump);
				board[nextStonePos]->setState(prevQuan, turnPlayer->getSymbol(), turnPlayer->getColor());

				std::cout << "\x1b[2J" << "\x1b[H";
				renderer.Render(board);
				std::cout << "상대를 잡았습니다! \n";

				output << castAndnSaveYut(*turnPlayer);
			}
		}

		return false;
	}

}