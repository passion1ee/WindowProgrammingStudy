#pragma once
namespace ys
{
	enum class TrffLightSignal
	{
		RED, REDtoYELLOW, GREENtoYELLOW, GREEN
	};

	class Observer
	{
	public:
		virtual ~Observer() {}
		virtual void Update(TrffLightSignal signal, RECT screen) = 0;
	};
}