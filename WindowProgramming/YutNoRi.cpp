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

	std::string Yutnori::castAndnSaveYut(Player& turnPlayer)
	{
		YutSticks curYut;
		std::stringstream output;
		while (true) {
			std::string tmpTrigger;
			std::cout << "���� �����÷��� 0�������� 1, 1�������� 2�� �Է��ϼ���:";
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
				std::cout << "���ʰ� �ƴմϴ�.\n" << "\x1b[1A";
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
			std::cout << "------���� ������: \n" << output.str();
			std::cout << "------�̹� ������: \n" << curYutRender;

			std::vector<int> ablePos;
			int curStonePos;
			std::pair<Quantity, Quantity> prevQuan;
			while (true) {//�� ���� �� �̵� ������ �� �˻� �� ���� ����ְ� �� �̵� Ȯ���ޱ�
				auto tmp = turnPlayer->isSamePos(-1);
				if (tmp.size() == turnPlayer->stoneQuantity() && curYut == YutSticks::kBackDo)
					return false;

				do {//�� ����
					curStonePos = turnPlayer->selectStone();
				} while (curStonePos == -1 && curYut == YutSticks::kBackDo);

				if (curStonePos == -1)
					prevQuan = { Quantity::kOne, Quantity::kEmpty };
				else
					prevQuan = board[curStonePos]->getQuantity();

				ablePos = board.availablePosition(curStonePos, curYut);//�̵� ������ ������ ã��

				/*�̵� ������ ������ ����ֱ�*/
				if (curStonePos == -1)
					board[0]->setState({ Quantity::kOne, Quantity::kEmpty }, turnPlayer->getSymbol(), Color::prevMove);
				else
					board[curStonePos]->setColor(Color::prevMove);//�����̱� �� ��ġ�� �����ϱ�

				std::vector<std::pair<Quantity, Quantity>> tmpQuan;
				std::vector<Symbol> tmpShape;
				std::vector<Color> tmpColor;
				for (auto pos : ablePos) {//�̵����� Pos�� State�� tmp�� ����
					tmpQuan.push_back(board[pos]->getQuantity());
					tmpShape.push_back(board[pos]->getShape());
					tmpColor.push_back(board[pos]->getColor());
				}

				for (auto pos : ablePos) {//�̵����� Pos�� �����ϸ� �̸� �����ֱ�
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
				renderer.Render(board);//�̵�������ġ �����ֱ�

				if (curStonePos == -1)
					board[0]->setState({ Quantity::kEmpty, Quantity::kEmpty }, Symbol::dump, Color::dump);//���� ������ ������(���� ����� ����)
				else
					board[curStonePos]->setColor(turnPlayer->getColor());//���� ������ ������(���� ����� ����)

				for (int i = 0; i < ablePos.size(); ++i)//�̵����� Pos�� ���� State�� ������(���� ����� ����)
					board[ablePos[i]]->setState(tmpQuan[i], tmpShape[i], tmpColor[i]);

				bool isSelect{ false };
				while (true) {
					std::string tmpSelect;
					std::cout << "���� �� ���� �����̽ðڽ��ϱ�?: 1. ��		2.�ٽ� �����ϰڽ��ϴ�. \n :  ";
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
						std::cout << "------���� ������: \n" << output.str();
						std::cout << "------�̹� ������: \n" << curYutRender;
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

			bool isGoalIn = turnPlayer->setStonePos(curStonePos, nextStonePos);//�ϴ� Stone�� Pos���� �̵� �Ϸ�, 0�̸� �̵� �� Stone ���� + true��ȯ
			if (isGoalIn) {
				if (turnPlayer->stoneQuantity())
					return true;
				board[curStonePos]->setState({ Quantity::kEmpty, Quantity::kEmpty }, Symbol::dump, Color::dump);
				continue;
			}

			std::vector<int> opponentStonesPos = opponentPlayer->isSamePos(nextStonePos);
			//����� ���� Stones�� pos���� ������ ������ Stone pos�� ������ Ž�� �� Pos�� ����

			if (opponentStonesPos.size() == 0) {//Ž������� ���ٸ� �Ʊ� ���̳� �� ���� ///// �ִٸ� ����� ��
				std::pair<Quantity, Quantity> saveQuan = SumQuantity(prevQuan, board[nextStonePos]->getQuantity());

				board[curStonePos]->setState({ Quantity::kEmpty, Quantity::kEmpty }, Symbol::dump, Color::dump);
				board[nextStonePos]->setState(saveQuan, turnPlayer->getSymbol(), turnPlayer->getColor());

			}
			else {//��뵹�� ������
				for (auto& stonePos : opponentStonesPos)//������ ���� Pos�� �ִ� ���Stones�� pos���� -1(���忡�� ���ֱ�)�� ����
					opponentPlayer->setStonePos(stonePos, -1);

				board[curStonePos]->setState({ Quantity::kEmpty, Quantity::kEmpty }, Symbol::dump, Color::dump);
				board[nextStonePos]->setState(prevQuan, turnPlayer->getSymbol(), turnPlayer->getColor());

				std::cout << "\x1b[2J" << "\x1b[H";
				renderer.Render(board);
				std::cout << "��븦 ��ҽ��ϴ�! \n";

				output << castAndnSaveYut(*turnPlayer);
			}
		}

		return false;
	}

}