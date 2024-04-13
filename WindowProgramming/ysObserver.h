#pragma once
namespace ys
{
	class Observer
	{
	public:
		virtual ~Observer() {}
		virtual void Update(int signal) = 0;
	};
}