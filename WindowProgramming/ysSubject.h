#pragma once

namespace ys
{
	class Observer;
	
	class Subject
	{
	public:
		virtual ~Subject() {}
		virtual void Attach(Observer* observer) = 0;
		virtual void Detach(Observer* observer) = 0;
		virtual void Notify(RECT screen) = 0;
	};
}

