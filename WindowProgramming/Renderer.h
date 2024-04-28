#ifndef RENDERER
#define RENDERER

#include <iostream>
#include "Player.h"
#include "Board.h"

class Renderer {
private:
	std::string stateRender(const Quantity&, const Symbol&);
public:
	std::string symbolToString(const Symbol&);
	std::string setColor(const Color&, const std::string&);

public:
	std::string renderYut(const YutSticks&);
	void Render(const Board&);
	//void availablePositionRender(const Board&, const int&, const std::vector<int>&);
};


#endif // !RENDERER

