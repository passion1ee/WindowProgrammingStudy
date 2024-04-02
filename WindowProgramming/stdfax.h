#pragma once
#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#include <random>

namespace ys
{
	static std::random_device ranDevice;
	static std::mt19937 randomEngine(ranDevice());
	static std::uniform_int_distribution<> ObjectColor(0, 255);
}