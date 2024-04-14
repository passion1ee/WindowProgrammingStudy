#include "Person.h"

void Person::Update(ys::TrffLightSignal signal, RECT screen)
{
	//어떤 블럭 Rect에 있는지 검사 후 저장 -> 좌상단부터 시계방향으로 1 2 3 4 1
	
	switch (signal)
	{
	case ys::TrffLightSignal::RED://건너기
		//Move()는 
		// (도로의 길이)(불변) = (velocity)(얘를 바꿔야겠네) * (도로를 건널 시간(1 / getRealFPS()))(불변)
		// velocity값 조정 후 이동
		break;
	case ys::TrffLightSignal::GREENtoYELLOW://방향정해
		//해당 블럭 Rect의 다음 번호로 이동하도록 direction 조정
		//여기서 width값 도로인지 height값 도로인지 정하기
		break;
	case ys::TrffLightSignal::GREEN://건너지마
		break;
	default:
		break;
	}
}
