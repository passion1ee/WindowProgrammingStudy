#pragma once
#include <windows.h>
#include <list>
#include <vector>

namespace ys
{
	class SnakeGame
	{
		struct Object
		{
			int id;
			int size;
			POINT position;
			POINT prevPosition;
			COLORREF color;
			BYTE moveState;
			bool isPlus;
			BYTE rectMoveCnt;

			Object() = default;
			Object(int id, int size, const POINT& position, COLORREF color_, const BYTE& moveState, bool isPlus)
				: id(id), size(size), position(position), prevPosition(position), color(color_), moveState(moveState), isPlus(isPlus), rectMoveCnt(0) { }


			bool operator==(const Object& other) const
			{
				return id == other.id && size == other.size &&
					position.x == other.position.x && position.y == other.position.y &&
					prevPosition.x == other.prevPosition.x && prevPosition.y == other.prevPosition.y && 
					color == other.color && moveState == other.moveState && isPlus == other.isPlus && rectMoveCnt == other.rectMoveCnt;
			}
		};

	public:
		void setScreen(RECT screenSize);

		void Init(HWND hWnd_, RECT screenSize);
		void Run();
		void Render();

	private:
		void Update();//�÷��̾� �ڵ��̵��ڵ� �����ڸ�
		void renderFrame();
		void renderObject(Object& object, bool isCircle);
		void move(std::list<Object>& body);
		void collide();
		void catchSnake();

	private:
		HWND hWnd;
		HDC hDC;
		HDC hBackDC;
		HBITMAP hBitmap;

		POINT screen;
		double xGrid;
		double yGrid;

		float frameCheck;
		float moveCoolTime;
		float setMoveTime;
		float speed;

		Object player;
		std::vector<Object> feeds;// 1:kHorizon 2:kVertical 3: kRect 4: kStop
		std::vector<Object> boxs;// 4: kStop
		std::vector<std::list<Object>> snakeList;//������(�� ����� Prevüũ�� ���� list��(�� vector����? �������� ����Ʈ�� ������(������'s snake, player's snake) ���� �� ����))
		// if(index == 0) 
		// objList[index].Pos == Player.prevPos;
		// else
		// objList[index].Pos == objList[index - 1].prevPos;
	public:
		SnakeGame();
	};
}

/*
-																																ȭ�鿡 40x40�� ���尡 �׷�����.
-	ȭ���� ��������� �ڳʿ� ���ΰ� ���� �ְ�, ���� ��ɾ ���� �� �Ǵ� �࿡ ���� ������׷� ������ ĭ�� ���� �ڵ� ��
	���Ѵ�. ��, �����ڸ��� �����ϸ� ���� ������ �̵� �� �ݴ� �������� �̵��Ѵ�.
-	������ ������ ��ġ���� ���̵��� �����ִ�. 20���� ���̰� �����ִ�.
*	���̴� ������ �������� �����Ǿ� �ִ�.
-	���ΰ� ���� ���̸� ������ �� �ڸ��� ������ �������� ���������� ��Ÿ����. ����������
*	�̵� ��� 1: �¿�� �̵�, ������ �����ڸ��� �����ϸ� ������ �ٲ㼭 �̵��Ѵ�.
*	�̵� ��� 2: ���Ϸ� �̵�, ������ �����ڸ��� �����ϸ� ������ �ٲ㼭 �̵��Ѵ�.
*	�̵� ��� 2: �׸� �׸��� ����.
*	�̵� ��� 4: ���ڸ��� �״�� �ִ´�.
-	���������� �ε�ġ��, �� ���� �پ ���� �̵� ��� �� �� ���� ������� �̵��Ѵ�. 
-	���ΰ� ���� �������� �ε�ġ��, �������� ���ΰ� ���� �ڿ� ������ �پ� ���ΰ��� �̵��ϴ� ��� ���� �̵��Ѵ�.

	
-	���콺 ���: ���� ���콺 ��ư
*	���� ���콺 ��ư�� �� ���忡 ������: ���콺�� Ŭ���� �������� ���ΰ����� �̵� ������ �ٲٰ� �ٽ� ������׷� �̵��Ѵ�.
*	���� ���콺 ��ư���� ���ΰ� ���� ���θ� Ŭ���ϸ�: ���� ũ�Ⱑ ��� �۾�����, �ٽ� ���� ũ��� �ٲ��.
*	���� ���콺 ��ư���� �������� Ŭ���ϸ�: �������� ���ΰ� ������ �и��Ǿ� ������ ĭ�� �������� �̵��Ѵ�. �и��� �������� �� �������� �̵� ��� 4�� �� �� �� ������� �̵�).
-	�߰��� ������ Ŭ���ϸ� Ŭ���� ���� �ڿ� �ִ� �����鵵 �� �и��� �� �̵��Ѵ� (�� �������� �̵� ��� 4�� �� �� �� ������� �̵�).

-	���콺 ���: ������ ���콺 ��ư
*	������ ���콺 ��ư�� �� ���忡 Ŭ���ϸ�: �� �ڸ��� ��ֹ��� �����, ���� ��ֹ��� ������ ������ �ٲ۴�.
*	��ֹ��� �ִ� 20������ ���� �� �ִ�.

-	Ű���� ��ɾ�:
*	s: ���� ����
*	��/��/��/��: �¿���� Ű���带 �Է��ϸ� ���ΰ� ���� ������ �¿���Ϸ� �ٲ�� �̵��Ѵ�. �����ڸ��� �����ϸ� ������� �̵��Ѵ�.
*	+/-: ��+���� �Է��ϸ� ���ΰ����� �ӵ��� ���� ��������, ��-���� �Է��ϸ� �ӵ��� ���� ��������.
*	j: ���ΰ����� �� �������� �� �ڸ����� (�Ǵ� �̵��ϸ鼭) �̵����⿡ ������������ �����ϵ��� �Ѵ�.
*	t: ���ΰ� ���� �� ���� �������� �ǰ�, ù��° �������� ���ΰ� ���� �ȴ�. ��� �Է��Ͽ� ���ΰ� ���� �ٲ� �� �ִ�.
*	a: ���� ���� �ӵ��� ������� �̵��� �ϴµ� �پ����� �ʾҴ� ��� ���������� ���ΰ��� �ڿ� �پ ��ֹ��� �״�� ����ϸ� ������
	�׷� �̵��Ѵ�. �����ߴ� ��ġ�� �����ϸ� ���� �ӵ��� �ǰ�, �پ����� �ʾҴ� ���������� �ڽŵ��� ���� ��ġ�� ����.
*	q: ���α׷��� �����Ѵ�
*/