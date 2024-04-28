#include "Renderer.h"

#include <sstream>

constexpr short kScrWidth = 76;
constexpr short kScrHeight = 35;


std::string Renderer::stateRender(const Quantity& argState, const Symbol& shape) {
	switch (argState) {
	case Quantity::kEmpty:
		return "   ";
	case Quantity::kOne:
		return symbolToString(shape) + "  ";
	case Quantity::kTwo:
		return symbolToString(shape) + " " + symbolToString(shape);
	default:
		return "( )";
	}
}

void Renderer::Render(const Board& board) {
	std::stringstream ss;
	const auto& pBoard = board.getBoard();

	std::vector<std::string> first;
	std::vector<std::string> second;
	for (auto piece : pBoard) {
		first.push_back(setColor(piece->getColor(), stateRender(piece->getQuantity().first, piece->getShape())));
		second.push_back(setColor(piece->getColor(), stateRender(piece->getQuantity().second, piece->getShape())));
	}
	if (first[22] == first[0] && second[22] == second[0]) {
		first[22] = first[27];
		second[22] = second[27];
	}
	ss << "\x1b[2J" << "\x1b[H";
	ss << "    ��������������     ��������������     ��������������         ��������������     ��������������     �������������� \n" <<
		"    �� " << first[10] << " �� ____�� " << first[9] << " �� ____�� " << first[8] << " �� ________�� " << first[7] << " �� ____�� " << first[6] << " �� ____�� " << first[5] << " �� \n" <<
		"    �� " << second[10] << " ��     �� " << second[9] << " ��     �� " << second[8] << " ��         �� " << second[7] << " ��     �� " << second[6] << " ��     �� " << second[5] << " �� \n" <<
		" 10 ��������������     ��������������     ��������������         ��������������     ��������������   5 �������������� \n" <<
		"        |    ��                                                     ��    |   \n" <<
		"        |      ��    25                                    20     ��      |   \n" <<
		"    ��������������     ��������������                                 ��������������     �������������� \n" <<
		"    �� " << first[11] << " ��     �� " << first[25] << " ��                                 �� " << first[20] << " ��     �� " << first[4] << " �� \n" <<
		"    �� " << second[11] << " ��     �� " << second[25] << " ��                                 �� " << second[20] << " ��     �� " << second[4] << " �� \n" <<
		"    ��������������     ��������������                                 ��������������     �������������� \n" <<
		"        |                �� ��                        �� ��                 |   \n" <<
		"        |               ��������                        ��������                |   \n" <<
		"    ��������������                 ��������������         ��������������                 �������������� \n" <<
		"    �� " << first[12] << " ��                 �� " << first[26] << " ��         �� " << first[21] << " ��                 �� " << first[3] << " �� \n" <<
		"    �� " << second[12] << " ��                 �� " << second[26] << " ��         �� " << second[21] << " ��                 �� " << second[3] << " �� \n" <<
		"    ��������������                 ��������������  27 22  ��������������                 �������������� \n" <<
		"        |                          �٦������������� ��                           |   \n" <<
		"        |                           �� " << first[22] << " ��                             |   \n" <<
		"        |                           �� " << second[22] << " ��                             |   \n" <<
		"        |                          �צ������������� ��                           |   \n" <<
		"    ��������������                 ��������������         ��������������                 �������������� \n" <<
		"    �� " << first[13] << " ��                 �� " << first[23] << " ��         �� " << first[28] << " ��                 �� " << first[2] << " �� \n" <<
		"    �� " << second[13] << " ��                 �� " << second[23] << " ��         �� " << second[28] << " ��                 �� " << second[2] << " �� \n" <<
		"    ��������������                 ��������������         ��������������              ^  �������������� \n" <<
		"        |               �� ��                          �� ��       / | \\    |   \n" <<
		"        |         24    ��������                        ��������    29   |      |   \n" <<
		"    ��������������     ��������������                                 ��������������  |  �������������� \n" <<
		"    �� " << first[14] << " ��     �� " << first[24] << " ��                                 �� " << first[29] << " ��  |  �� " << first[1] << " �� \n" <<
		"    �� " << second[14] << " ��     �� " << second[24] << " ��                                 �� " << second[29] << " ��  |  �� " << second[1] << " �� \n" <<
		"    ��������������     ��������������                                 ��������������     �������������� \n" <<
		"        |     ��                                                  ��      |   \n" <<
		"        |   ��                                                      ��    |   \n" <<
		"    ��������������     ��������������     ��������������         ��������������     ��������������     �������������� \n" <<
		"    �� " << first[15] << " �� ____�� " << first[16] << " �� ____�� " << first[17] << " �� ________�� " << first[18] << " �� ____�� " << first[19] << " �� ____�� " << first[0] << " �� \n" <<
		"    �� " << second[15] << " ��     �� " << second[16] << " ��     �� " << second[17] << " ��         �� " << second[18] << " ��     �� " << second[19] << " ��     �� " << second[0] << " �� \n" <<
		" 15 ��������������     ��������������     ��������������         ��������������     ��������������   0 �������������� \n";
	std::cout << ss.str();
}

std::string Renderer::symbolToString(const Symbol& symbol) {
	switch (symbol) {
	case Symbol::Hash:
		return "#";
	case Symbol::circle:
		return "@";
	default:
		return " ";
	}
}

std::string Renderer::setColor(const Color& color, const std::string& baseString) {
	switch (color) {
	case Color::kRed:
		return "\x1b[31;1m" + baseString + "\x1b[0m";
	case Color::kBlue:
		return "\x1b[34;1m" + baseString + "\x1b[0m";
	case Color::kGreen:
		return "\x1b[32;1m" + baseString + "\x1b[0m";
	case Color::kYellow:
		return "\x1b[33;1m" + baseString + "\x1b[0m";
	case Color::prevMove:
		return "\x1b[35;1m" + baseString + "\x1b[0m";
	case Color::predictedMove:
		return "\x1b[36;1m" + baseString + "\x1b[0m";
	default:
		return "\x1b[0m" + baseString + "\x1b[0m";
	}
}

std::string Renderer::renderYut(const YutSticks& yut) {
	std::stringstream stream;
	switch (yut) {
	case YutSticks::kDo:
		stream << "����������  ����������  ����������  ����������\n";
		stream << "�� x ��  �� x ��  �� x ��  ��   ��\n";
		stream << "��   ��  ��   ��  ��   ��  ��   ��\n";
		stream << "�� x ��  �� x ��  �� x ��  ��   ��\n";
		stream << "��   ��  ��   ��  ��   ��  ��   ��\n";
		stream << "�� x ��  �� x ��  �� x ��  ��   ��\n";
		stream << "����������  ����������  ����������  ����������\n";
		return stream.str();
	case YutSticks::kGae:
		stream << "����������  ����������  ����������  ����������\n";
		stream << "�� x ��  �� x ��  ��   ��  ��   ��\n";
		stream << "��   ��  ��   ��  ��   ��  ��   ��\n";
		stream << "�� x ��  �� x ��  ��   ��  ��   ��\n";
		stream << "��   ��  ��   ��  ��   ��  ��   ��\n";
		stream << "�� x ��  �� x ��  ��   ��  ��   ��\n";
		stream << "����������  ����������  ����������  ����������\n";
		return stream.str();
	case YutSticks::kGeol:
		stream << "����������  ����������  ����������  ����������\n";
		stream << "�� x ��  ��   ��  ��   ��  ��   ��\n";
		stream << "��   ��  ��   ��  ��   ��  ��   ��\n";
		stream << "�� x ��  ��   ��  ��   ��  ��   ��\n";
		stream << "��   ��  ��   ��  ��   ��  ��   ��\n";
		stream << "�� x ��  ��   ��  ��   ��  ��   ��\n";
		stream << "����������  ����������  ����������  ����������\n";
		return stream.str();
	case YutSticks::kYut:
		stream << "����������  ����������  ����������  ����������\n";
		stream << "��   ��  ��   ��  ��   ��  ��   ��\n";
		stream << "��   ��  ��   ��  ��   ��  ��   ��\n";
		stream << "��   ��  ��   ��  ��   ��  ��   ��\n";
		stream << "��   ��  ��   ��  ��   ��  ��   ��\n";
		stream << "��   ��  ��   ��  ��   ��  ��   ��\n";
		stream << "����������  ����������  ����������  ����������\n";
		return stream.str();
	case YutSticks::kMo:
		stream << "����������  ����������  ����������  ����������\n";
		stream << "�� x ��  �� x ��  �� x ��  �� x ��\n";
		stream << "��   ��  ��   ��  ��   ��  ��   ��\n";
		stream << "�� x ��  �� x ��  �� x ��  �� x ��\n";
		stream << "��   ��  ��   ��  ��   ��  ��   ��\n";
		stream << "�� x ��  �� x ��  �� x ��  �� x ��\n";
		stream << "����������  ����������  ����������  ����������\n";
		return stream.str();
	case YutSticks::kBackDo:
		stream << "����������  ����������  ����������  ����������\n";
		stream << "�� x ��  �� x ��  �� x ��  ��   ��\n";
		stream << "��   ��  ��   ��  ��   ��  ��  /��\n";
		stream << "�� x ��  �� x ��  �� x ��  �� / ��\n";
		stream << "��   ��  ��   ��  ��   ��  ��/  ��\n";
		stream << "�� x ��  �� x ��  �� x ��  ��   ��\n";
		stream << "����������  ����������  ����������  ����������\n";
		return stream.str();
	default:
		return " ";
	}
}