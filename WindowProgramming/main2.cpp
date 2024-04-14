#include <Windows.h>
#include <tchar.h>
#include <iostream>

#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
#include "..\\..\\WinProgramming\\MyEngine_source\\ysInputManager.h"
#include "..\\..\\WinProgramming\\MyEngine_source\\ysTimer.h"
#pragma comment (lib, "..\\..\\WinProgramming\\x64\\Debug\\MyEngine.lib")

#define PRACTICE__3
//#define PRACTICE__3_1
//#define PRACTICE__3_2
#define PRACTICE__3_3

#ifdef PRACTICE__3
#ifdef PRACTICE__3_1
#include <windowsx.h>
#include "SnakeGame.h"

ys::SnakeGame game;
RECT windowRect{ 0, 0, 1200 , 1200 };

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
	game.Init(hWnd, windowRect);

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
		game.Run();
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
		game.setScreen(rect);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		game.LButton(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case WM_RBUTTONDOWN:
	{
		game.RButton(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case WM_KEYDOWN:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);

		//InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYUP:
	{
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);

		//InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);
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

#endif // PRACTICE__3_1

#ifdef PRACTICE__3_2
#include <windowsx.h>
#include "BrickOut.h"

ys::BrickOut game;
RECT windowRect{ 0, 0, 16 * 80, 9 * 80 };

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
		CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);

	game.Init(hWnd, windowRect);
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
		game.Run();
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
	case WM_LBUTTONDOWN:
	{
		ys::InputManager::setKeyState(VK_LBUTTON, FALSE, FALSE);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		ys::InputManager::setKeyState(VK_RBUTTON, FALSE, FALSE);
		break;
	}
	case WM_LBUTTONUP:
	{
		ys::InputManager::setKeyState(VK_LBUTTON, KF_REPEAT, KF_UP);
		break;
	}
	case WM_RBUTTONUP:
	{
		ys::InputManager::setKeyState(VK_RBUTTON, KF_REPEAT, KF_UP);
		break;
	}
	case WM_KEYDOWN:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);

		//InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYUP:
	{
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);

		//InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);
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
#endif // PRACTICE__3_2

#ifdef PRACTICE__3_3
#include <windowsx.h>
#include "Intersection.h"

ys::Intersection game;
RECT windowRect{ 0, 0, 16 * 80, 9 * 80 };

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
		CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, hInstance, NULL);

	game.Init(hWnd, windowRect);
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
		game.Run();
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
	}case WM_LBUTTONDOWN:
	{
		game.click(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		ys::InputManager::setKeyState(VK_LBUTTON, FALSE, FALSE);
		break;
	}
	case WM_LBUTTONUP:
	{
		ys::InputManager::setKeyState(VK_LBUTTON, KF_REPEAT, KF_UP);
		break;
	}
	case WM_KEYDOWN:
	{//이후 KF_ALTDOWN으로 Alt키 조합까지 구현가능
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);

		//InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYUP:
	{
		auto keyflags = HIWORD(lParam);
		ys::InputManager::setKeyState(wParam,
			(keyflags & KF_REPEAT) == KF_REPEAT,
			(keyflags & KF_UP) == KF_UP);

		//InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
	{
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
#endif // PRACTICE__3_3

#endif // PRACTICE__3

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ