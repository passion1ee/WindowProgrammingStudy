#pragma once
#include "ysObserver.h"

namespace ys
{
	class Subject
	{
	public:
		virtual ~Subject() {}
		virtual void Attach(Observer* observer) = 0; // 관찰자 추가
		virtual void Detach(Observer* observer) = 0; // 관찰자 제거
		virtual void Notify() = 0; // 모든 관찰자에게 변경 사실 알림
	};
}

