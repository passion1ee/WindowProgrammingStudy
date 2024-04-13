#pragma once
#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#include "..\\..\\WinProgramming\\MyEngine_source\\ysTimer.h"
#include <random>

namespace ys
{
	struct fVector
	{
		float x;
		float y;

		fVector(float x, float y)
			: x(x), y(y)
		{
		}

		fVector() = default;
	};

	static std::random_device ranDevice;
	static std::mt19937 randomEngine(ranDevice());
	static std::uniform_int_distribution<> ObjectColor(0, 255);
	static std::uniform_int_distribution<> uid;
}