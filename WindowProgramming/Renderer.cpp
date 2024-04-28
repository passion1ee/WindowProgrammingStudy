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
	ss << "    忙式式式式式忖     忙式式式式式忖     忙式式式式式忖         忙式式式式式忖     忙式式式式式忖     忙式式式式式忖 \n" <<
		"    弛 " << first[10] << " 弛 ____弛 " << first[9] << " 弛 ____弛 " << first[8] << " 弛 ________弛 " << first[7] << " 弛 ____弛 " << first[6] << " 弛 ____弛 " << first[5] << " 弛 \n" <<
		"    弛 " << second[10] << " 弛     弛 " << second[9] << " 弛     弛 " << second[8] << " 弛         弛 " << second[7] << " 弛     弛 " << second[6] << " 弛     弛 " << second[5] << " 弛 \n" <<
		" 10 戌式式式式式戎     戌式式式式式戎     戌式式式式式戎         戌式式式式式戎     戌式式式式式戎   5 戌式式式式式戎 \n" <<
		"        |    Ｋ                                                     Ｉ    |   \n" <<
		"        |      Ｋ    25                                    20     Ｉ      |   \n" <<
		"    忙式式式式式忖     忙式式式式式忖                                 忙式式式式式忖     忙式式式式式忖 \n" <<
		"    弛 " << first[11] << " 弛     弛 " << first[25] << " 弛                                 弛 " << first[20] << " 弛     弛 " << first[4] << " 弛 \n" <<
		"    弛 " << second[11] << " 弛     弛 " << second[25] << " 弛                                 弛 " << second[20] << " 弛     弛 " << second[4] << " 弛 \n" <<
		"    戌式式式式式戎     戌式式式式式戎                                 戌式式式式式戎     戌式式式式式戎 \n" <<
		"        |                Ｋ 弛                        弛 Ｉ                 |   \n" <<
		"        |               式式式戎                        戌式式式                |   \n" <<
		"    忙式式式式式忖                 忙式式式式式忖         忙式式式式式忖                 忙式式式式式忖 \n" <<
		"    弛 " << first[12] << " 弛                 弛 " << first[26] << " 弛         弛 " << first[21] << " 弛                 弛 " << first[3] << " 弛 \n" <<
		"    弛 " << second[12] << " 弛                 弛 " << second[26] << " 弛         弛 " << second[21] << " 弛                 弛 " << second[3] << " 弛 \n" <<
		"    戌式式式式式戎                 戌式式式式式戎  27 22  戌式式式式式戎                 戌式式式式式戎 \n" <<
		"        |                          Ｋ忙式式式式式忖 Ｉ                           |   \n" <<
		"        |                           弛 " << first[22] << " 弛                             |   \n" <<
		"        |                           弛 " << second[22] << " 弛                             |   \n" <<
		"        |                          Ｉ戌式式式式式戎 Ｋ                           |   \n" <<
		"    忙式式式式式忖                 忙式式式式式忖         忙式式式式式忖                 忙式式式式式忖 \n" <<
		"    弛 " << first[13] << " 弛                 弛 " << first[23] << " 弛         弛 " << first[28] << " 弛                 弛 " << first[2] << " 弛 \n" <<
		"    弛 " << second[13] << " 弛                 弛 " << second[23] << " 弛         弛 " << second[28] << " 弛                 弛 " << second[2] << " 弛 \n" <<
		"    戌式式式式式戎                 戌式式式式式戎         戌式式式式式戎              ^  戌式式式式式戎 \n" <<
		"        |               弛 Ｉ                          Ｋ 弛       / | \\    |   \n" <<
		"        |         24    戌式式式                        式式式戎    29   |      |   \n" <<
		"    忙式式式式式忖     忙式式式式式忖                                 忙式式式式式忖  |  忙式式式式式忖 \n" <<
		"    弛 " << first[14] << " 弛     弛 " << first[24] << " 弛                                 弛 " << first[29] << " 弛  |  弛 " << first[1] << " 弛 \n" <<
		"    弛 " << second[14] << " 弛     弛 " << second[24] << " 弛                                 弛 " << second[29] << " 弛  |  弛 " << second[1] << " 弛 \n" <<
		"    戌式式式式式戎     戌式式式式式戎                                 戌式式式式式戎     戌式式式式式戎 \n" <<
		"        |     Ｉ                                                  Ｋ      |   \n" <<
		"        |   Ｉ                                                      Ｋ    |   \n" <<
		"    忙式式式式式忖     忙式式式式式忖     忙式式式式式忖         忙式式式式式忖     忙式式式式式忖     忙式式式式式忖 \n" <<
		"    弛 " << first[15] << " 弛 ____弛 " << first[16] << " 弛 ____弛 " << first[17] << " 弛 ________弛 " << first[18] << " 弛 ____弛 " << first[19] << " 弛 ____弛 " << first[0] << " 弛 \n" <<
		"    弛 " << second[15] << " 弛     弛 " << second[16] << " 弛     弛 " << second[17] << " 弛         弛 " << second[18] << " 弛     弛 " << second[19] << " 弛     弛 " << second[0] << " 弛 \n" <<
		" 15 戌式式式式式戎     戌式式式式式戎     戌式式式式式戎         戌式式式式式戎     戌式式式式式戎   0 戌式式式式式戎 \n";
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
		stream << "忙式式式忖  忙式式式忖  忙式式式忖  忙式式式忖\n";
		stream << "弛 x 弛  弛 x 弛  弛 x 弛  弛   弛\n";
		stream << "弛   弛  弛   弛  弛   弛  弛   弛\n";
		stream << "弛 x 弛  弛 x 弛  弛 x 弛  弛   弛\n";
		stream << "弛   弛  弛   弛  弛   弛  弛   弛\n";
		stream << "弛 x 弛  弛 x 弛  弛 x 弛  弛   弛\n";
		stream << "戌式式式戎  戌式式式戎  戌式式式戎  戌式式式戎\n";
		return stream.str();
	case YutSticks::kGae:
		stream << "忙式式式忖  忙式式式忖  忙式式式忖  忙式式式忖\n";
		stream << "弛 x 弛  弛 x 弛  弛   弛  弛   弛\n";
		stream << "弛   弛  弛   弛  弛   弛  弛   弛\n";
		stream << "弛 x 弛  弛 x 弛  弛   弛  弛   弛\n";
		stream << "弛   弛  弛   弛  弛   弛  弛   弛\n";
		stream << "弛 x 弛  弛 x 弛  弛   弛  弛   弛\n";
		stream << "戌式式式戎  戌式式式戎  戌式式式戎  戌式式式戎\n";
		return stream.str();
	case YutSticks::kGeol:
		stream << "忙式式式忖  忙式式式忖  忙式式式忖  忙式式式忖\n";
		stream << "弛 x 弛  弛   弛  弛   弛  弛   弛\n";
		stream << "弛   弛  弛   弛  弛   弛  弛   弛\n";
		stream << "弛 x 弛  弛   弛  弛   弛  弛   弛\n";
		stream << "弛   弛  弛   弛  弛   弛  弛   弛\n";
		stream << "弛 x 弛  弛   弛  弛   弛  弛   弛\n";
		stream << "戌式式式戎  戌式式式戎  戌式式式戎  戌式式式戎\n";
		return stream.str();
	case YutSticks::kYut:
		stream << "忙式式式忖  忙式式式忖  忙式式式忖  忙式式式忖\n";
		stream << "弛   弛  弛   弛  弛   弛  弛   弛\n";
		stream << "弛   弛  弛   弛  弛   弛  弛   弛\n";
		stream << "弛   弛  弛   弛  弛   弛  弛   弛\n";
		stream << "弛   弛  弛   弛  弛   弛  弛   弛\n";
		stream << "弛   弛  弛   弛  弛   弛  弛   弛\n";
		stream << "戌式式式戎  戌式式式戎  戌式式式戎  戌式式式戎\n";
		return stream.str();
	case YutSticks::kMo:
		stream << "忙式式式忖  忙式式式忖  忙式式式忖  忙式式式忖\n";
		stream << "弛 x 弛  弛 x 弛  弛 x 弛  弛 x 弛\n";
		stream << "弛   弛  弛   弛  弛   弛  弛   弛\n";
		stream << "弛 x 弛  弛 x 弛  弛 x 弛  弛 x 弛\n";
		stream << "弛   弛  弛   弛  弛   弛  弛   弛\n";
		stream << "弛 x 弛  弛 x 弛  弛 x 弛  弛 x 弛\n";
		stream << "戌式式式戎  戌式式式戎  戌式式式戎  戌式式式戎\n";
		return stream.str();
	case YutSticks::kBackDo:
		stream << "忙式式式忖  忙式式式忖  忙式式式忖  忙式式式忖\n";
		stream << "弛 x 弛  弛 x 弛  弛 x 弛  弛   弛\n";
		stream << "弛   弛  弛   弛  弛   弛  弛  /弛\n";
		stream << "弛 x 弛  弛 x 弛  弛 x 弛  弛 / 弛\n";
		stream << "弛   弛  弛   弛  弛   弛  弛/  弛\n";
		stream << "弛 x 弛  弛 x 弛  弛 x 弛  弛   弛\n";
		stream << "戌式式式戎  戌式式式戎  戌式式式戎  戌式式式戎\n";
		return stream.str();
	default:
		return " ";
	}
}