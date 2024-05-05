#pragma once
#include "..\\..\\WinProgramming\\MyEngine_source\\ysMath.h"
#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#include "..\\..\\WinProgramming\\MyEngine_source\\ysTimer.h"
#include <random>

namespace ys
{
	static std::random_device ranDevice;
	static std::mt19937 randomEngine(ranDevice());
	static std::uniform_int_distribution<> uid;
}