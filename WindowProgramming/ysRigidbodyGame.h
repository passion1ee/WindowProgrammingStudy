#pragma once
#include <windows.h>
#include <vector>

namespace ys
{
	enum class Shape : BYTE
	{
		kTriangle, kHourglass, kPentagon, kPie, kCount, kRect, kStar, kEmpty
	};

 	struct Object
	{
		int size;
		Shape shape;
		COLORREF color;
		POINT position;//39, 39

		Object() : shape(Shape::kEmpty), color(0), position({ 0, 0 }), size(10) {}
		Object(Shape _shape, COLORREF _color, POINT _position, int _size)
			: shape(_shape), color(_color), position(_position), size(_size) {}

	};

	class RigidbodyGame
	{

	public:
		static void setScreen(int width, int height);

		static void Init();
		static void Run(HWND hWnd);
		static void render(HDC hDC);

	private:
		static void Update(HWND hWnd);
		static void renderFrame(HDC hDC);
		static void renderObject(HDC hDC, Object object);
		static void collide(HWND hWnd);
		static void victorySound();
		static void defeatSound();
	private:
		static int screenWidth;
		static int screenHeight;

		static Object player;
		static POINT playerPrevPosition;
		static Object goal;
		static std::vector<std::vector<Object>> plain;
		static std::vector<COLORREF> colors;
	};
}

/*
 �� �̵��Ͽ� ���ϴ� ��ġ�� ���ϴ� ���� ���ϴ� �������� �����ϱ�
- ���� ��ܰ� ���� �ϴܿ� ���� ���� ��ġ�� ��ǥ ��ġ�� ǥ���Ѵ�.
* ��ǥ ��ġ���� ���󺯰�ĭ ���� �� ���� ��������, ����� �� �ִ� ũ��� 4������ ��� �� �� ���� ���� �׷��� �ִ�.
- ���� �̵��ϸ� ����, ũ��, ����� �����Ͽ� ��ǥ �������� �̵��Ѵ�.
- ĭ�� �߰� �߰��� �ټ� ������ �ٸ� ���� ĭ���� �ִ�. (�ּ� 20��)
1) (255, 0, 0)��ֹ� ĭ (������ ĭ) - ��ü���� ����� �� ����.
3) (125, 0, 255)ũ�� ���� ĭ (���) - ���� ũ�Ⱑ ��ҵȴ�. �ּ� ũ�Ⱑ �Ǹ� �� �Ŀ��� ���� ũ�Ⱑ Ŀ����.
4) (255, 255, 0)ũ�� ���� ĭ (Ȯ��) - ���� ũ�Ⱑ Ȯ��ȴ�. ���� ũ��� �ٵ����� ĭ�� �ִ� ũ���̴�.
5) (0, 0, 255)��� ���� ĭ - ���� ����� ����ȴ�. 4������ �ٸ� ������� �ٲ� �� �ִ�. (����, �׸�, ��, Ÿ���� ����� ���� �����ϴ�.)
- ���ΰ� ���� ���� ��ġ�� ������ ���� ������ ũ��, ������ ������� ��Ÿ����.
- �÷��̴� Ű���� ��ɾ �̿��Ͽ� ���� ĭ�� ���߾� �¿���� �̵��Ѵ�.
* �����ڸ��� �����ϸ� �ݴ������� ��Ÿ����.
* ��ֹ� ĭ: ���� �� ����.
* ũ�� ���� ĭ: ���� ũ�Ⱑ ���Ѵ�.
* ��� ���� ĭ: ���� ����� ���Ѵ�.


- ȭ�鿡 40X40 ĭ�� �׸���.
2) ���� ���� ĭ - ���� ���� ĭ�� ������ ������ �����Ѵ�. ���� �� ĭ�� ������ ���� ĭ�� ����� �����ϰ� �ٲ��. (�ٸ� ����� ����, ��
������ ����Ǿ�� �ϰ�, ĭ�� ���� �� �� ���� ��ǥ �����̾�� �Ѵ�)


* ���� ���� ĭ: ������ �����ϰ� ���Ѵ�.


- ��ǥ ������ �������� ��, ���õ� ���·� �����ϸ� �̰�ٴ� �޽�����, �ٸ� ���·� �����ϸ� ���ٴ� �޽����� ��µȴ�.
- Ű���� ���
* wasd: �� �̵� Ű���� (�¿���� �̵� ��� Ű����)
* r: ���Ӱ� ����



* q: ���α׷� ����
*/