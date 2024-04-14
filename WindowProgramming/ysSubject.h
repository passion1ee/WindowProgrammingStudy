#pragma once
#include "stdfax.h"
#include <Windows.h>

namespace ys
{
	class Observer;
	
	class Subject
	{
	public:
		virtual ~Subject() {}
		virtual void Attach(std::shared_ptr<ys::Observer> observer) = 0;
		virtual void Detach(std::shared_ptr<ys::Observer> observer) = 0;
		virtual void Notify(RECT screen) = 0;
	};
}

