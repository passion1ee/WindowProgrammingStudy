#include "Person.h"

void Person::Update(ys::TrffLightSignal signal, RECT screen)
{
	//� �� Rect�� �ִ��� �˻� �� ���� -> �»�ܺ��� �ð�������� 1 2 3 4 1
	
	switch (signal)
	{
	case ys::TrffLightSignal::RED://�ǳʱ�
		//Move()�� 
		// (������ ����)(�Һ�) = (velocity)(�긦 �ٲ�߰ڳ�) * (���θ� �ǳ� �ð�(1 / getRealFPS()))(�Һ�)
		// velocity�� ���� �� �̵�
		break;
	case ys::TrffLightSignal::GREENtoYELLOW://��������
		//�ش� �� Rect�� ���� ��ȣ�� �̵��ϵ��� direction ����
		//���⼭ width�� �������� height�� �������� ���ϱ�
		break;
	case ys::TrffLightSignal::GREEN://�ǳ�����
		break;
	default:
		break;
	}
}
