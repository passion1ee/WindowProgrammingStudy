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
 돌 이동하여 원하는 위치에 원하는 모양과 원하는 색상으로 도착하기
- 좌측 상단과 우측 하단에 각각 시작 위치와 목표 위치를 표시한다.
* 목표 위치에는 색상변경칸 중의 한 가지 색상으로, 변경될 수 있는 크기로 4종류의 모양 중 한 개의 돌이 그려져 있다.
- 돌을 이동하며 색상, 크기, 모양을 변경하여 목표 지점까지 이동한다.
- 칸의 중간 중간에 다섯 종류의 다른 색의 칸들이 있다. (최소 20개)
1) (255, 0, 0)장애물 칸 (빨간색 칸) - 객체들이 통과할 수 없다.
3) (125, 0, 255)크기 변경 칸 (축소) - 돌의 크기가 축소된다. 최소 크기가 되면 그 후에는 돌의 크기가 커진다.
4) (255, 255, 0)크기 변경 칸 (확대) - 돌의 크기가 확대된다. 돌의 크기는 바둑판의 칸이 최대 크기이다.
5) (0, 0, 255)모양 변경 칸 - 돌의 모양이 변경된다. 4종류의 다른 모양으로 바뀔 수 있다. (세모, 네모, 원, 타원… 모양은 변경 가능하다.)
- 주인공 돌이 시작 위치에 랜덤한 색과 랜덤한 크기, 랜덤한 모양으로 나타난다.
- 플레이는 키보드 명령어를 이용하여 돌을 칸에 맞추어 좌우상하 이동한다.
* 가장자리에 도착하면 반대쪽으로 나타난다.
* 장애물 칸: 지날 수 없다.
* 크기 변경 칸: 돌의 크기가 변한다.
* 모양 변경 칸: 돌의 모양이 변한다.


- 화면에 40X40 칸을 그린다.
2) 색상 변경 칸 - 색상 변경 칸은 랜덤한 색으로 설정한다. 돌이 이 칸을 지나면 색이 칸의 색상과 동일하게 바뀐다. (다른 방법도 가능, 단
색상은 변경되어야 하고, 칸의 색상 중 한 개는 목표 색상이어야 한다)


* 색상 변경 칸: 색상이 랜덤하게 변한다.


- 목표 지점에 도착했을 때, 제시된 형태로 도착하면 이겼다는 메시지가, 다른 형태로 도착하면 졌다는 메시지가 출력된다.
- 키보드 명령
* wasd: 돌 이동 키보드 (좌우상하 이동 명령 키보드)
* r: 새롭게 시작



* q: 프로그램 종료
*/