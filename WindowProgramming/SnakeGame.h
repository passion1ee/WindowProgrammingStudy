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

	public:
		void LButton(int x, int y);
		void RButton(int x, int y);

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

		float moveCoolTime;
		float setMoveTime;
		float speed;

		//gameTrigger
		float frameCheck;
		bool isRun;

		//Message
		bool isJump;
		bool isScaleIn;
		bool isScaleOut;
		bool isBoxErase;
		bool isFastSeach;

		Object player;
		std::vector<Object> feeds;// 1:kHorizon 2:kVertical 3: kRect 4: kStop
		std::vector<Object> boxs;// 4: kStop
		std::vector<std::list<Object>> snakeList;//꼬리원(앞 노드의 Prev체크를 위해 list로(왜 vector까지? 꼬리원의 리스트가 여러개(꼬리원's snake, player's snake) 있을 수 있음))
		std::list<Object> fastSnake;
		// if(index == 0) 
		// objList[index].Pos == Player.prevPos;
		// else
		// objList[index].Pos == objList[index - 1].prevPos;
	public:
		SnakeGame();
	};
}
