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
		void Update();//플레이어 자동이동코드 가장자리
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
		std::vector<std::list<Object>> snakeList;//꼬리원(앞 노드의 Prev체크를 위해 list로(왜 vector까지? 꼬리원의 리스트가 여러개(꼬리원's snake, player's snake) 있을 수 있음))
		// if(index == 0) 
		// objList[index].Pos == Player.prevPos;
		// else
		// objList[index].Pos == objList[index - 1].prevPos;
	public:
		SnakeGame();
	};
}

/*
-																																화면에 40x40의 보드가 그려진다.
-	화면의 좌측상단쪽 코너에 주인공 원이 있고, 시작 명령어에 따라 열 또는 행에 맞춰 지그재그로 보드의 칸에 맞춰 자동 이
	동한다. 즉, 가장자리에 도달하면 다음 행으로 이동 후 반대 방향으로 이동한다.
-	보드의 랜덤한 위치에서 먹이들이 놓여있다. 20개의 먹이가 놓여있다.
*	먹이는 랜덤한 색상으로 설정되어 있다.
-	주인공 원이 먹이를 먹으면 그 자리에 먹이의 색상으로 꼬리원들이 나타난다. 꼬리원들은
*	이동 방법 1: 좌우로 이동, 보드의 가장자리에 도착하면 방향을 바꿔서 이동한다.
*	이동 방법 2: 상하로 이동, 보드의 가장자리에 도착하면 방향을 바꿔서 이동한다.
*	이동 방법 2: 네모를 그리며 돈다.
*	이동 방법 4: 제자리에 그대로 있는다.
-	꼬리원끼리 부딪치면, 두 개가 붙어서 위의 이동 방법 중 한 개의 방법으로 이동한다. 
-	주인공 원과 꼬리원이 부딪치면, 꼬리원은 주인공 원의 뒤에 꼬리로 붙어 주인공이 이동하는 대로 따라 이동한다.

	
-	마우스 명령: 왼쪽 마우스 버튼
*	왼쪽 마우스 버튼을 빈 보드에 누르면: 마우스가 클릭된 방향으로 주인공원이 이동 방향을 바꾸고 다시 지그재그로 이동한다.
*	왼쪽 마우스 버튼으로 주인공 원의 내부를 클릭하면: 원의 크기가 잠시 작아지고, 다시 원래 크기로 바뀐다.
*	왼쪽 마우스 버튼으로 꼬리원을 클릭하면: 꼬리원은 주인공 원에서 분리되어 보드의 칸에 남겨지며 이동한다. 분리된 꼬리원은 앞 페이지의 이동 방법 4개 중 한 개 방법으로 이동).
-	중간의 꼬리를 클릭하면 클릭된 꼬리 뒤에 있는 꼬리들도 다 분리된 후 이동한다 (앞 페이지의 이동 방법 4개 중 한 개 방법으로 이동).

-	마우스 명령: 오른쪽 마우스 버튼
*	오른쪽 마우스 버튼을 빈 보드에 클릭하면: 그 자리에 장애물이 생기고, 원은 장애물을 만나면 방향을 바꾼다.
*	장애물은 최대 20개까지 만들 수 있다.

-	키보드 명령어:
*	s: 게임 시작
*	↑/←/→/↓: 좌우상하 키보드를 입력하면 주인공 원의 방향이 좌우상하로 바뀌고 이동한다. 가장자리에 도달하면 지그재그 이동한다.
*	+/-: ‘+’를 입력하면 주인공원의 속도가 점점 빨라지고, ‘-’를 입력하면 속도가 점점 느려진다.
*	j: 주인공원과 그 꼬리들은 그 자리에서 (또는 이동하면서) 이동방향에 수직방향으로 점프하도록 한다.
*	t: 주인공 원이 맨 뒤의 꼬리원이 되고, 첫번째 꼬리원이 주인공 원이 된다. 계속 입력하여 주인공 원을 바꿀 수 있다.
*	a: 아주 빠른 속도로 지그재그 이동을 하는데 붙어있지 않았던 모든 꼬리원들이 주인공원 뒤에 붙어서 장애물도 그대로 통과하며 지그재
	그로 이동한다. 시작했던 위치에 도착하면 원래 속도가 되고, 붙어있지 않았던 꼬리원들은 자신들의 원래 위치로 간다.
*	q: 프로그램이 종료한다
*/