#pragma once
namespace ys
{
	class Observer
	{
	public:
		virtual ~Observer() {}
		virtual void Update(BYTE signal) = 0;
	};
}