#include <Windows.h>
#include <tchar.h>
#include <iostream>

//#define PRACTICE__1
//#define PRACTICE__1_1
//#define PRACTICE__1_2

#define PRACTICE__2_week1
//#define PRACTICE__2_1
//#define PRACTICE__2_2

//#define PRACTICE__2_3
//#define PRACTICE__2_4
//#define PRACTICE__2_5
//#define PRACTICE__2_6

#define PRACTICE__2_week2
//#define PRACTICE__2_7
//#define PRACTICE__2_9
//#define PRACTICE__2_10
//////////////////////////////#define PRACTICE__2_11
//////////////////////////////#define PRACTICE__2_12
//////////////////////////////#define PRACTICE__2_13
#define PRACTICE__2_14


#ifdef PRACTICE__1


#define FILE_MENU_EXIT 1

HINSTANCE g_hInst;
HMENU hMenu;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 1";

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WNDCLASSEX NewWndClass(UINT, WNDPROC, HINSTANCE, HICON,
	HCURSOR, HBRUSH, LPCWSTR, LPCWSTR, HICON);
void AddMenu(HWND);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HWND hWnd;
	MSG Message;
	g_hInst = hInstance;

#ifdef PRACTICE__1_1
	WNDCLASSEX WndClass = NewWndClass(CS_HREDRAW | CS_VREDRAW, (WNDPROC)WinProc, hInstance, LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL, lpszClass, LoadIcon(NULL, IDI_APPLICATION));
#endif // PRACTICE__1_1

#ifdef PRACTICE__1_2
	WNDCLASSEX WndClass = NewWndClass(CS_HREDRAW | CS_VREDRAW, (WNDPROC)WinProc, hInstance, LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_HAND), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL, lpszClass, LoadIcon(NULL, IDI_QUESTION));
#endif // PRACTICE__1_2

	RegisterClassEx(&WndClass);

#ifdef PRACTICE__1_1
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		0, 0, 1280, 800, NULL, NULL, hInstance, NULL);
#endif // PRACTICE__1_1

#ifdef PRACTICE__1_2
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
		100, 50, 800, 600, NULL, NULL, hInstance, NULL);
#endif // PRACTICE__1_2

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;

	switch (Msg) {
#ifdef PRACTICE__1_2
	case WM_COMMAND:
		switch (wParam) {
		case FILE_MENU_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			break;
		}
		break;
	case WM_CREATE:
		AddMenu(hWnd);
		break;
#endif // PRACTICE__1_2
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return (DefWindowProc(hWnd, Msg, wParam, lParam));
}

WNDCLASSEX NewWndClass(UINT _style, WNDPROC _lpfnWndProc, HINSTANCE _hInstance, HICON _hIcon,
	HCURSOR _hCursor, HBRUSH _hbrBackground, LPCWSTR _lpszMenuName, LPCWSTR _lpszClassName, HICON _hIconSm)
{
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = _style;
	WndClass.lpfnWndProc = _lpfnWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = _hInstance;
	WndClass.hIcon = _hIcon;
	WndClass.hCursor = _hCursor;
	WndClass.hbrBackground = _hbrBackground;
	WndClass.lpszMenuName = _lpszMenuName;
	WndClass.lpszClassName = _lpszClassName;
	WndClass.hIconSm = _hIconSm;
	return WndClass;
}


#ifdef PRACTICE__1_2
void AddMenu(HWND hWnd) {
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();

	AppendMenuW(hFileMenu, MF_STRING, NULL, L"메뉴창");
	AppendMenuW(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"Exit");

	AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"Menu");

	SetMenu(hWnd, hMenu);
}
#endif // PRACTICE__1_2

/*
윈도우의 타이틀을 “windows program 2”로 설정하고,
시스템 메뉴, 최대, 최소화 버튼, 수평/수직 스크롤 바를 가지고 있고, 크기가 조정 가능한
배경색을 하얀색으로
윈도우를 위치 (100, 50)에, 윈도우의 크기를 800*600 으로 만들어 출력하기.
스몰 아이콘을 물음표로, 커서는 손 모양으로 설정하기
*/
#endif // PRACTICE__1

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

#ifdef PRACTICE__2_week1
 
#ifdef PRACTICE__2_1
#include <random>
#include <string>
#include <vector>

constexpr int Width = 800;
constexpr int Height = 600;
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 1";
std::vector<std::pair<int, int>> coord;
int screenWidth{ Width }, screenHeight{ Height };


LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WNDCLASSEX NewWndClass(UINT, WNDPROC, HINSTANCE, HICON,
	HCURSOR, HBRUSH, LPCWSTR, LPCWSTR, HICON);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HWND hWnd;
	MSG Message;
	g_hInst = hInstance;

	WNDCLASSEX WndClass = NewWndClass(CS_HREDRAW | CS_VREDRAW, (WNDPROC)WinProc, hInstance, LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL, lpszClass, LoadIcon(NULL, IDI_APPLICATION));

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		0, 0, Width, Height, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	std::random_device rd;
	std::mt19937 re(rd());
	std::uniform_int_distribution<> uid;
	PAINTSTRUCT ps;
	HDC hDC;
	
	
	
	std::wstring line;
	
	switch (Msg) {
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;
		lpMMI->ptMinTrackSize.x = Width;
		lpMMI->ptMinTrackSize.y = Height;
		return 0;
	}
	case WM_SIZE:
		screenWidth = LOWORD(lParam);
		screenHeight = HIWORD(lParam);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		coord.clear();
		coord.push_back({ screenWidth / (screenWidth / (screenWidth / 200)) * (screenWidth / (screenWidth / 100)), screenHeight / (screenHeight / (screenHeight / 60)) * (screenHeight / (screenHeight / 30)) });
		for (int i = 1; i < ((screenWidth / 100) * (screenHeight / 30)) / 10;) {
			int xTmp{ (uid(re) % screenWidth) / (screenWidth / (screenWidth / 100)) * (screenWidth / (screenWidth / 100)) }, yTmp{ (uid(re) % screenHeight) / (screenHeight / (screenHeight / 30)) * (screenHeight / (screenHeight / 30)) };
			if (coord.end() == std::find_if(coord.begin(), coord.end(), [xTmp, yTmp](std::pair<int, int> data) {return xTmp == data.first && yTmp == data.second; }))
			{
				coord.push_back({ xTmp, yTmp });
				++i;
			}
		}
		for (int i = 0; i < ((screenWidth / 100) * (screenHeight / 30)) / 10; ++i) {
			line = std::to_wstring(i) + L": (" + std::to_wstring(coord[i].first) + L", " + std::to_wstring(coord[i].second) + L")";
			TextOut(hDC, coord[i].first, coord[i].second, line.c_str(), line.size());
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return (DefWindowProc(hWnd, Msg, wParam, lParam));
}

WNDCLASSEX NewWndClass(UINT _style, WNDPROC _lpfnWndProc, HINSTANCE _hInstance, HICON _hIcon,
	HCURSOR _hCursor, HBRUSH _hbrBackground, LPCWSTR _lpszMenuName, LPCWSTR _lpszClassName, HICON _hIconSm)
{
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = _style;
	WndClass.lpfnWndProc = _lpfnWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = _hInstance;
	WndClass.hIcon = _hIcon;
	WndClass.hCursor = _hCursor;
	WndClass.hbrBackground = _hbrBackground;
	WndClass.lpszMenuName = _lpszMenuName;
	WndClass.lpszClassName = _lpszClassName;
	WndClass.hIconSm = _hIconSm;
	return WndClass;
}
#endif //PRACTICE__2_1

#ifdef PRACTICE__2_2
#include <random>
#include <string>
#include <vector>

constexpr int Width = 400;
constexpr int Height = 300;
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 1";




LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WNDCLASSEX NewWndClass(UINT, WNDPROC, HINSTANCE, HICON,
	HCURSOR, HBRUSH, LPCWSTR, LPCWSTR, HICON);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HWND hWnd;
	MSG Message;
	g_hInst = hInstance;

	WNDCLASSEX WndClass = NewWndClass(CS_HREDRAW | CS_VREDRAW, (WNDPROC)WinProc, hInstance, LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL, lpszClass, LoadIcon(NULL, IDI_APPLICATION));

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		0, 0, Width, Height, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	std::random_device rd;
	std::mt19937 re(rd());
	std::uniform_int_distribution<> uid(5, 10);
	std::uniform_int_distribution<> color(0, 255);
	PAINTSTRUCT ps;
	HDC hDC;
	static int screenWidth{ Width }, screenHeight{ Height };


	std::wstring line;

	switch (Msg) {
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;
		lpMMI->ptMinTrackSize.x = Width;
		lpMMI->ptMinTrackSize.y = Height;
		return 0;
	}
	case WM_SIZE:
		screenWidth = LOWORD(lParam);
		screenHeight = HIWORD(lParam);
		break;
	case WM_PAINT:
	{
		static std::vector<RECT> coord;
		hDC = BeginPaint(hWnd, &ps);
		coord.clear();
		int row = uid(re) % 11;
		int col = uid(re) % 11;
		coord.assign(row * col, RECT());
		for (int i = 0; i < row; ++i)
			for (int j = 0; j < col; ++j)
			{
				coord[(i * col) + j].left = screenWidth / col * j;
				coord[(i * col) + j].top = screenHeight / row * i;
				coord[(i * col) + j].right = screenWidth / col * (j + 1);
				coord[(i * col) + j].bottom = screenHeight / row * (i + 1);
			}

		for (int i = 0; i < coord.size(); ++i) {
			line = L"이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지이윤상의 Windows API 너무 재미있다얼른 배워서 그래픽스도 해봐야지";
			auto setColor = RGB(color(re), color(re), color(re));
			//HBRUSH randomBrush = CreateSolidBrush(setColor);
			//FillRect(hDC, &coord[i], randomBrush);
			SetBkColor(hDC, setColor);
			SetTextColor(hDC, RGB(color(re), color(re), color(re)));
			DrawText(hDC, line.c_str(), line.size(), &coord[i], DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
			//DeleteObject(randomBrush);
		}
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return (DefWindowProc(hWnd, Msg, wParam, lParam));
}

WNDCLASSEX NewWndClass(UINT _style, WNDPROC _lpfnWndProc, HINSTANCE _hInstance, HICON _hIcon,
	HCURSOR _hCursor, HBRUSH _hbrBackground, LPCWSTR _lpszMenuName, LPCWSTR _lpszClassName, HICON _hIconSm)
{
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = _style;
	WndClass.lpfnWndProc = _lpfnWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = _hInstance;
	WndClass.hIcon = _hIcon;
	WndClass.hCursor = _hCursor;
	WndClass.hbrBackground = _hbrBackground;
	WndClass.lpszMenuName = _lpszMenuName;
	WndClass.lpszClassName = _lpszClassName;
	WndClass.hIconSm = _hIconSm;
	return WndClass;
}
#endif // PRACTICE__2_2


#ifdef PRACTICE__2_3

#include <sstream>
#include <random>
#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#pragma comment (lib, "..\\..\\WinProgramming\\x64\\Debug\\MyEngine.lib")

std::random_device rd;
std::mt19937 re(rd());
std::uniform_int_distribution<> randX(0, 600);
std::uniform_int_distribution<> randY(0, 400);
std::uniform_int_distribution<> randNum(0, 9);
std::uniform_int_distribution<> randQuan(5, 20);
std::uniform_int_distribution<> color(0, 255);

static int printX{};
static int printY{};
static int printNum{};
static int printQuan{};

static std::wstringstream mainPrintBuff;

RECT windowRect{ 0, 0, 800 , 600 };

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"WinAPI Practice";

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WNDCLASSEX NewWndClass(UINT, WNDPROC, HINSTANCE, HICON,
	HCURSOR, HBRUSH, LPCWSTR, LPCWSTR, HICON);



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HWND hWnd;
	MSG msg;
	g_hInst = hInstance;

	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	::RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);

	ys::InputManager::Init();

	::ShowWindow(hWnd, nShowCmd);
	::UpdateWindow(hWnd);
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {//event
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}
		//game logic update
		if (ys::InputManager::getKeyDown(VK_RETURN))
		{
			printX = randX(re);
			printY = randY(re);
			printNum = randNum(re);
			printQuan = randQuan(re);

			mainPrintBuff.str(L"");
			mainPrintBuff.clear();
			for (int i = 0; i < printQuan; ++i) {
				for (int j = 0; j < printQuan; ++j)
					mainPrintBuff << std::to_wstring(printNum);
				mainPrintBuff << L"\n";
			}

			InvalidateRect(hWnd, NULL, TRUE);
		}

		if (ys::InputManager::getKeyDown((BYTE)ys::Key::Q))
		{
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	static int x{}, y{};
	static int screeenWidth{ windowRect.right - windowRect.left }, screenHeight{ windowRect.bottom - windowRect.top };
	std::wstring buff;

	switch (Msg) {//InputManager는 singleton으로 해보고 게임 로직 자체는 Component패턴으로 가보자
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;
		lpMMI->ptMinTrackSize.x = windowRect.right - windowRect.left;
		lpMMI->ptMinTrackSize.y = windowRect.bottom - windowRect.top;
		break;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		screeenWidth = rect.right - rect.left;
		screenHeight = rect.bottom - rect.top;
		break;
	}
	case WM_MOUSEMOVE:
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		break;
	}
	case WM_CHAR:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		break;
	}
	case WM_KEYDOWN:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);
		break;
	}
	case WM_KEYUP:
	{
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);
		break;
	}
	case WM_PAINT:
	{
		Beep(199, 100);
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);
		SIZE piece; GetTextExtentPoint32(hDC, std::to_wstring(printNum).c_str(), std::to_wstring(printNum).size(), &piece);
		SetBkColor(hDC, RGB(color(re), color(re), color(re)));
		SetTextColor(hDC, RGB(color(re), color(re), color(re)));
		RECT tmpRect = { printX, printY, printX + piece.cx * printQuan, printY + piece.cy * printQuan };
		DrawText(hDC, mainPrintBuff.str().c_str(), mainPrintBuff.str().size(), &tmpRect, DT_LEFT | DT_TOP);

		::EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return (::DefWindowProc(hWnd, Msg, wParam, lParam));
}

#endif // PRACTICE__2_3

#ifdef PRACTICE__2_4
#include <sstream>
#include <random>
#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#pragma comment (lib, "..\\..\\WinProgramming\\x64\\Debug\\MyEngine.lib")

static std::wstringstream mainStringBuff;
static std::wstringstream mainPrintBuff;
static int PositionX{};
static int PositionY{};
static int printNum{};
static int printTable{};
static bool isPrint{ false };
static bool isPrevPrint{ false };
RECT windowRect{ 0, 0, 800 , 600 };

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"WinAPI Practice";

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WNDCLASSEX NewWndClass(UINT, WNDPROC, HINSTANCE, HICON,
	HCURSOR, HBRUSH, LPCWSTR, LPCWSTR, HICON);

bool isVal(int x, int y, int n, int m);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HWND hWnd;
	MSG msg;
	g_hInst = hInstance;

	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	::RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);

	ys::InputManager::Init();
	CreateCaret(hWnd, NULL, 2, 20);
	SetCaretPos(0, 0);

	::ShowWindow(hWnd, nShowCmd);
	::UpdateWindow(hWnd);
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {//event
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}
		//game logic update
		if (isPrint)
		{
			mainPrintBuff.str(L"");
			mainPrintBuff.clear();
			for (int m = 1; m <= printTable; ++m)
			{
				mainPrintBuff << std::to_wstring(printNum) << L"*" << m << L" = " << std::to_wstring(printNum * m) << L'\n';
			}
			isPrint = false;
			isPrevPrint = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		if (ys::InputManager::getKeyDown(VK_RETURN)) 
		{
			if (isPrevPrint)
			{
				mainStringBuff.str(L"");
				mainStringBuff.clear();
			}
			else
			{
				int x{}, y{}, n{}, m{};
				mainStringBuff >> x >> y >> n >> m;
				mainStringBuff.str(L"");
				mainStringBuff.clear();
				if (isVal(x, y, n, m))
				{
					PositionX = x; PositionY = y; printNum = n; printTable = m;
					isPrint = true;
					SetCaretPos(0, 0);
					InvalidateRect(hWnd, NULL, TRUE);
				}
				else
					SetCaretPos(0, 0);
			}
		}
		if (ys::InputManager::getKeyDown((BYTE)ys::Key::Q)) 
		{
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
	}
	return msg.wParam;
}

bool isVal(int x, int y, int n, int m)
{
	if(x < 0 || x  > 600)
		return false;
	if(y < 0 || y > 400)
		return false;
	if(n < 1 || n > 19)
		return false;
	if(m < 1 || m >30)
		return false;
	return true;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	static int x{}, y{};
	static int screeenWidth{ windowRect.right - windowRect.left }, screenHeight{ windowRect.bottom - windowRect.top };
	std::wstring buff;

	switch (Msg) {//InputManager는 singleton으로 해보고 게임 로직 자체는 Component패턴으로 가보자
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;
		lpMMI->ptMinTrackSize.x = windowRect.right - windowRect.left;
		lpMMI->ptMinTrackSize.y = windowRect.bottom - windowRect.top;
		break;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		screeenWidth = rect.right - rect.left;
		screenHeight = rect.bottom - rect.top;
		break;
	}
	case WM_MOUSEMOVE:
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		break;
	}
	case WM_CHAR:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		if (!isPrint)
		{
			buff = wParam;
			mainStringBuff << buff;
			hDC = GetDC(hWnd);
			POINT caretPosition; GetCaretPos(&caretPosition);
			SIZE piece; GetTextExtentPoint32(hDC, buff.c_str(), buff.size(), &piece);

			SetCaretPos(caretPosition.x + piece.cx, caretPosition.y);

			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	}
	case WM_KEYDOWN:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);
		break;
	}
	case WM_KEYUP:
	{
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);
		break;
	}
	case WM_PAINT:
	{
		Beep(199, 100);
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);
		ShowCaret(hWnd);
		if (isPrevPrint)
		{
			RECT tmpRect = { PositionX, PositionY, PositionX + 200, PositionY + 20 * printTable };
			DrawText(hDC, mainPrintBuff.str().c_str(), mainPrintBuff.str().size(), &tmpRect, DT_LEFT | DT_TOP);
			isPrevPrint = false;
		}
		else
		{
			RECT tmpRect = { 0, 0, PositionX + 200, PositionY + 20};
			DrawText(hDC, mainStringBuff.str().c_str(), mainStringBuff.str().size(), &tmpRect, DT_LEFT | DT_TOP);
		}
		
		::EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return (::DefWindowProc(hWnd, Msg, wParam, lParam));
}

#endif // PRACTICE__2_4

#ifdef PRACTICE__2_5
#include "TextEditor.h"

#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#pragma comment (lib, "..\\..\\WinProgramming\\x64\\Debug\\MyEngine.lib")


RECT windowRect{ 0, 0, 800 , 600 };

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"WinAPI Practice";

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WNDCLASSEX NewWndClass(UINT, WNDPROC, HINSTANCE, HICON,
	HCURSOR, HBRUSH, LPCWSTR, LPCWSTR, HICON);

bool isVal(int x, int y, int n, int m);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HWND hWnd;
	MSG msg;
	g_hInst = hInstance;

	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	::RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);

	ys::InputManager::Init();
	ys::TextEditor::Init();


	::ShowWindow(hWnd, nShowCmd);
	::UpdateWindow(hWnd);
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {//event
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}
		//game logic update
		ys::TextEditor::Run(hWnd);
	}
	return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	static int x{}, y{};
	static int screeenWidth{ windowRect.right - windowRect.left }, screenHeight{ windowRect.bottom - windowRect.top };
	std::wstring buff;

	switch (Msg) {//InputManager는 singleton으로 해보고 게임 로직 자체는 Component패턴으로 가보자
	case WM_CREATE:
	{
		CreateCaret(hWnd, NULL, 2, 20);
		ShowCaret(hWnd);
		break;
	}
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;
		lpMMI->ptMinTrackSize.x = windowRect.right - windowRect.left;
		lpMMI->ptMinTrackSize.y = windowRect.bottom - windowRect.top;
		break;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		screeenWidth = rect.right - rect.left;
		screenHeight = rect.bottom - rect.top;
		break;
	}
	case WM_MOUSEMOVE:
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		break;
	}
	case WM_CHAR:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		if (wParam != VK_BACK && wParam != VK_RETURN)
		{
			buff = wParam;
			hDC = GetDC(hWnd);
			ys::TextEditor::Add(hDC, buff);
			ReleaseDC(hWnd, hDC);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYDOWN:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);
		break;
	}
	case WM_KEYUP:
	{
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);
		break;
	}
	case WM_PAINT:
	{
		Beep(199, 100);
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);

		ys::TextEditor::Render(hDC);

		::EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		::PostQuitMessage(0);
		HideCaret(hWnd);
		DestroyCaret();
		return 0;
	default:
		break;
	}

	return (::DefWindowProc(hWnd, Msg, wParam, lParam));
}
#endif // PRACTICE__2_5

#ifdef PRACTICE__2_6
#include <random>
#include "RandomShape.h"


RECT windowRect{ 0, 0, 800 , 600 };

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"WinAPI Practice";

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WNDCLASSEX NewWndClass(UINT, WNDPROC, HINSTANCE, HICON,
	HCURSOR, HBRUSH, LPCWSTR, LPCWSTR, HICON);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HWND hWnd;
	MSG msg;
	g_hInst = hInstance;

	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	ys::RandomShape::setScreen(windowRect.right, windowRect.bottom);
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	::RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);

	//ys::InputManager::Init();

	::ShowWindow(hWnd, nShowCmd);
	::UpdateWindow(hWnd);
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {//event
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}
		//game logic update
	}
	return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	static int x{}, y{};
	std::wstring line;


	switch (Msg) {//InputManager는 singleton으로 해보고 게임 로직 자체는 Component패턴으로 가보자
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;
		lpMMI->ptMinTrackSize.x = windowRect.right - windowRect.left;
		lpMMI->ptMinTrackSize.y = windowRect.bottom - windowRect.top;
		break;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		ys::RandomShape::setScreen(rect.right - rect.left, rect.bottom - rect.top);
		break;
	}
	case WM_MOUSEMOVE:
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		break;
	}
	//case WM_KEYDOWN:
	//{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
	//	auto keyflags = HIWORD(lParam);
	//	ys::InputManager::setKeyState(wParam,
	//		(keyflags & KF_REPEAT) == KF_REPEAT,
	//		(keyflags & KF_UP) == KF_UP);

	//	break;
	//}
	//case WM_KEYUP:
	//{
	//	auto keyflags = HIWORD(lParam);

	//	ys::InputManager::setKeyState(wParam,
	//		(keyflags & KF_REPEAT) == KF_REPEAT,
	//		(keyflags & KF_UP) == KF_UP);

	//	break;
	//}
	case WM_PAINT:
	{
		Beep(199, 100);
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);
		std::random_device rd;
		std::mt19937 re(rd());
		std::uniform_int_distribution<> uid(2, 10);
		int row = uid(re);
		int col = uid(re);
		ys::RandomShape::setSection(row, col);
		ys::RandomShape::render(hDC);

		::EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return (::DefWindowProc(hWnd, Msg, wParam, lParam));
}

#endif // PRACTICE__2_6

#endif // PRACTICE__2_week1

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

#ifdef PRACTICE__2_week2
#ifdef PRACTICE__2_7
#include "TextEditor.h"

#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#pragma comment (lib, "..\\..\\WinProgramming\\x64\\Debug\\MyEngine.lib")


RECT windowRect{ 0, 0, 1600 , 900 };

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"WinAPI Practice";

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WNDCLASSEX NewWndClass(UINT, WNDPROC, HINSTANCE, HICON,
	HCURSOR, HBRUSH, LPCWSTR, LPCWSTR, HICON);

bool isVal(int x, int y, int n, int m);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HWND hWnd;
	MSG msg;
	g_hInst = hInstance;

	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, FALSE);
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	::RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);

	ys::InputManager::Init();
	ys::TextEditor::Init();


	::ShowWindow(hWnd, nShowCmd);
	::UpdateWindow(hWnd);

	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {//event
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		//game logic update
		ys::TextEditor::Run();
	}
	return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	static int x{}, y{};
	static int screeenWidth{ windowRect.right - windowRect.left }, screenHeight{ windowRect.bottom - windowRect.top };
	std::wstring buff;

	switch (Msg) {//InputManager는 singleton으로 해보고 게임 로직 자체는 Component패턴으로 가보자
	case WM_CREATE:
	{
		CreateCaret(hWnd, NULL, 2, 20);
		ShowCaret(hWnd);
		break;
	}
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;
		lpMMI->ptMinTrackSize.x = windowRect.right - windowRect.left;
		lpMMI->ptMinTrackSize.y = windowRect.bottom - windowRect.top;
		break;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		screeenWidth = rect.right - rect.left;
		screenHeight = rect.bottom - rect.top;
		break;
	}
	case WM_MOUSEMOVE:
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		break;
	}
	case WM_CHAR:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		if (wParam != VK_BACK && wParam != VK_RETURN && wParam != VK_TAB && wParam != L'+' && wParam != L'-' && wParam != VK_ESCAPE)
		{
			hDC = GetDC(hWnd);
			ys::TextEditor::Add(hDC, wParam);
			ReleaseDC(hWnd, hDC);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYDOWN:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYUP:
	{
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);

		ys::TextEditor::Render(hDC);

		::EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		::PostQuitMessage(0);
		HideCaret(hWnd);
		DestroyCaret();
		return 0;
	default:
		break;
	}

	return (::DefWindowProc(hWnd, Msg, wParam, lParam));
}
#endif // PRACTICE__2_7

#ifdef PRACTICE__2_9
#include <random>
#include "selectShape.h"
#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#pragma comment (lib, "..\\..\\WinProgramming\\x64\\Debug\\MyEngine.lib")

RECT windowRect{ 0, 0, 120 , 120 };

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"WinAPI Practice";

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WNDCLASSEX NewWndClass(UINT, WNDPROC, HINSTANCE, HICON,
	HCURSOR, HBRUSH, LPCWSTR, LPCWSTR, HICON);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HWND hWnd;
	MSG msg;
	g_hInst = hInstance;

	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	ys::selectShape::setScreen(windowRect.right, windowRect.bottom);
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	::RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);

	ys::InputManager::Init();
	ys::selectShape::Init();

	::ShowWindow(hWnd, nShowCmd);
	::UpdateWindow(hWnd);
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {//event
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}
		//game logic update
		ys::selectShape::Run();
	}
	return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	static int x{}, y{};
	std::wstring line;


	switch (Msg) {//InputManager는 singleton으로 해보고 게임 로직 자체는 Component패턴으로 가보자
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;
		lpMMI->ptMinTrackSize.x = windowRect.right - windowRect.left;
		lpMMI->ptMinTrackSize.y = windowRect.bottom - windowRect.top;
		break;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		ys::selectShape::setScreen(rect.right - rect.left, rect.bottom - rect.top);
		break;
	}
	case WM_MOUSEMOVE:
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		break;
	}
	case WM_KEYDOWN:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYUP:
	{
		auto keyflags = HIWORD(lParam);

		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);
		if (ys::InputManager::getKeyUp((UINT)ys::Key::Q))
		{
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
	{
		Beep(199, 100);
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);

		ys::selectShape::render(hDC);

		::EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return (::DefWindowProc(hWnd, Msg, wParam, lParam));
}

#endif // PRACTICE__2_9

#ifdef PRACTICE__2_10
#include "ysRigidbodyGame.h"
#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#pragma comment (lib, "..\\..\\WinProgramming\\x64\\Debug\\MyEngine.lib")

RECT windowRect{ 0, 0, 900 , 900 };

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"WinAPI Practice";

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WNDCLASSEX NewWndClass(UINT, WNDPROC, HINSTANCE, HICON,
	HCURSOR, HBRUSH, LPCWSTR, LPCWSTR, HICON);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HWND hWnd;
	MSG msg;
	g_hInst = hInstance;

	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	ys::RigidbodyGame::setScreen(windowRect.right, windowRect.bottom);
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	::RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);

	ys::InputManager::Init();
	ys::RigidbodyGame::Init();

	::ShowWindow(hWnd, nShowCmd);
	::UpdateWindow(hWnd);
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {//event
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}
		//game logic update
		ys::RigidbodyGame::Run(hWnd);
	}
	return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	static int x{}, y{};
	std::wstring line;


	switch (Msg) {//InputManager는 singleton으로 해보고 게임 로직 자체는 Component패턴으로 가보자
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;
		lpMMI->ptMinTrackSize.x = windowRect.right - windowRect.left;
		lpMMI->ptMinTrackSize.y = windowRect.bottom - windowRect.top;
		break;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		ys::RigidbodyGame::setScreen(rect.right - rect.left, rect.bottom - rect.top);
		break;
	}
	case WM_MOUSEMOVE:
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		break;
	}
	case WM_KEYDOWN:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);
		if (ys::InputManager::getKeyDown((UINT)ys::Key::W) || ys::InputManager::getKeyDown((UINT)ys::Key::A) || 
			ys::InputManager::getKeyDown((UINT)ys::Key::S) || ys::InputManager::getKeyDown((UINT)ys::Key::D))
			InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYUP:
	{
		auto keyflags = HIWORD(lParam);

		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);
		if (ys::InputManager::getKeyUp((UINT)ys::Key::R))
			InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
	{
		Beep(199, 100);
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);

		ys::RigidbodyGame::render(hDC);

		::EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return (::DefWindowProc(hWnd, Msg, wParam, lParam));
}

#endif // PRACTICE__2_10

#ifdef PRACTICE__2_11
#include <windowsx.h>
#include "ysInputShape.h"

#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#pragma comment (lib, "..\\..\\WinProgramming\\x64\\Debug\\MyEngine.lib")

RECT windowRect{ 0, 0, 900 , 900 };

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"WinAPI Practice";

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WNDCLASSEX NewWndClass(UINT, WNDPROC, HINSTANCE, HICON,
	HCURSOR, HBRUSH, LPCWSTR, LPCWSTR, HICON);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HWND hWnd;
	MSG msg;
	g_hInst = hInstance;

	ys::InputManager::Init();
	ys::InputShape::Init();

	ys::InputShape::setScreen(windowRect.right, windowRect.bottom);
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	::RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);

	::ShowWindow(hWnd, nShowCmd);
	::UpdateWindow(hWnd);
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {//event
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}
		//game logic update
		ys::InputShape::Run();
	}
	return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	static int x{}, y{};
	std::wstring line;


	switch (Msg) {//InputManager는 singleton으로 해보고 게임 로직 자체는 Component패턴으로 가보자
	case WM_CREATE:
	{
		CreateCaret(hWnd, NULL, 2, 20);
		break;
	}
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;
		lpMMI->ptMinTrackSize.x = windowRect.right - windowRect.left;
		lpMMI->ptMinTrackSize.y = windowRect.bottom - windowRect.top;
		break;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		ys::InputShape::setScreen(rect.right - rect.left, rect.bottom - rect.top);
		break;
	}
	case WM_LBUTTONUP:
	{
		ys::InputShape::TextBoxClicked(hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case WM_CHAR:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		if (wParam != VK_BACK && wParam != VK_RETURN && wParam != VK_TAB && wParam != L'+' && wParam != L'-' && wParam != VK_ESCAPE)
		{
			ys::InputShape::Add(wParam);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYDOWN:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYUP:
	{
		auto keyflags = HIWORD(lParam);

		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
	{
		Beep(199, 100);
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);

		ys::InputShape::render(hWnd, hDC);

		::EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		::PostQuitMessage(0);
		HideCaret(hWnd);
		DestroyCaret();
		return 0;
	default:
		break;
	}

	return (::DefWindowProc(hWnd, Msg, wParam, lParam));
}

#endif // PRACTICE__2_11

#ifdef PRACTICE__2_12
#include <windowsx.h>
#include "ysShapeLand.h"

#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#pragma comment (lib, "..\\..\\WinProgramming\\x64\\Debug\\MyEngine.lib")

RECT windowRect{ 0, 0, 900 , 900 };

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"WinAPI Practice";

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WNDCLASSEX NewWndClass(UINT, WNDPROC, HINSTANCE, HICON,
	HCURSOR, HBRUSH, LPCWSTR, LPCWSTR, HICON);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HWND hWnd;
	MSG msg;
	g_hInst = hInstance;

	ys::InputManager::Init();
	ys::ShapeLand::Init();

	ys::ShapeLand::setScreen(windowRect.right, windowRect.bottom);
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	::RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);

	::ShowWindow(hWnd, nShowCmd);
	::UpdateWindow(hWnd);
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {//event
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}
		//game logic update
		ys::ShapeLand::Run(hWnd);
	}
	return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	static int x{}, y{};
	std::wstring line;


	switch (Msg) {//InputManager는 singleton으로 해보고 게임 로직 자체는 Component패턴으로 가보자
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;
		lpMMI->ptMinTrackSize.x = windowRect.right - windowRect.left;
		lpMMI->ptMinTrackSize.y = windowRect.bottom - windowRect.top;
		break;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		ys::ShapeLand::setScreen(rect.right - rect.left, rect.bottom - rect.top);
		break;
	}
	case WM_CHAR:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		if (wParam != VK_BACK && wParam != VK_RETURN && wParam != VK_TAB && wParam != L'+' && wParam != L'-' && wParam != VK_ESCAPE)
		{
			ys::ShapeLand::select(wParam);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	}
	case WM_KEYDOWN:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYUP:
	{
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);

		ys::ShapeLand::render(hDC);

		::EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return (::DefWindowProc(hWnd, Msg, wParam, lParam));
}

#endif // PRACTICE__2_12

#ifdef PRACTICE__2_13
#include <windowsx.h>
#include "ysWordPuzzle.h"

#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#pragma comment (lib, "..\\..\\WinProgramming\\x64\\Debug\\MyEngine.lib")

RECT windowRect{ 0, 0, 900 , 900 };

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"WinAPI Practice";

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WNDCLASSEX NewWndClass(UINT, WNDPROC, HINSTANCE, HICON,
	HCURSOR, HBRUSH, LPCWSTR, LPCWSTR, HICON);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HWND hWnd;
	MSG msg;
	g_hInst = hInstance;

	ys::InputManager::Init();
	ys::WordPuzzle::Init();

	ys::WordPuzzle::setScreen(windowRect.right, windowRect.bottom);
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	::RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);

	::ShowWindow(hWnd, nShowCmd);
	::UpdateWindow(hWnd);
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {//event
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}
		//game logic update
		ys::WordPuzzle::Run(hWnd);
	}
	return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	static int x{}, y{};
	std::wstring line;


	switch (Msg) {//InputManager는 singleton으로 해보고 게임 로직 자체는 Component패턴으로 가보자
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;
		lpMMI->ptMinTrackSize.x = windowRect.right - windowRect.left;
		lpMMI->ptMinTrackSize.y = windowRect.bottom - windowRect.top;
		break;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		ys::WordPuzzle::setScreen(rect.right - rect.left, rect.bottom - rect.top);
		break;
	}
	case WM_KEYDOWN:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYUP:
	{
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);

		ys::WordPuzzle::render(hDC);

		::EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return (::DefWindowProc(hWnd, Msg, wParam, lParam));
}

#endif // PRACTICE__2_13

#ifdef PRACTICE__2_14
#include <windowsx.h>
#include "ysWordPuzzle.h"

#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#pragma comment (lib, "..\\..\\WinProgramming\\x64\\Debug\\MyEngine.lib")

RECT windowRect{ 0, 0, 900 , 900 };

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"WinAPI Practice";

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

WNDCLASSEX NewWndClass(UINT, WNDPROC, HINSTANCE, HICON,
	HCURSOR, HBRUSH, LPCWSTR, LPCWSTR, HICON);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HWND hWnd;
	MSG msg;
	g_hInst = hInstance;

	ys::InputManager::Init();
	ys::WordPuzzle::Init();

	ys::WordPuzzle::setScreen(windowRect.right, windowRect.bottom);
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	::RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);

	::ShowWindow(hWnd, nShowCmd);
	::UpdateWindow(hWnd);
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {//event
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}
		//game logic update
		ys::WordPuzzle::Run(hWnd);
	}
	return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	static int x{}, y{};
	std::wstring line;


	switch (Msg) {//InputManager는 singleton으로 해보고 게임 로직 자체는 Component패턴으로 가보자
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* lpMMI = (MINMAXINFO*)lParam;
		lpMMI->ptMinTrackSize.x = windowRect.right - windowRect.left;
		lpMMI->ptMinTrackSize.y = windowRect.bottom - windowRect.top;
		break;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		ys::WordPuzzle::setScreen(rect.right - rect.left, rect.bottom - rect.top);
		break;
	}
	case WM_KEYDOWN:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYUP:
	{
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);

		ys::WordPuzzle::render(hDC);

		::EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return (::DefWindowProc(hWnd, Msg, wParam, lParam));
}
#endif // PRACTICE__2_14

#endif // PRACTICE__2_week2

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ