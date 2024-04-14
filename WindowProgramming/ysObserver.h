#pragma once
#include "stdfax.h"
#include <Windows.h>

namespace ys
{
	

	class Observer
	{
	public:
		virtual ~Observer() {}
		virtual bool Update(ys::TrffLightSignal signal, RECT screen) = 0;
	};
}