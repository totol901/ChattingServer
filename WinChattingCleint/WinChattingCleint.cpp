// ChattingClient.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"

//전역변수
HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;
HWND g_hChat = NULL;
const TCHAR* g_lpszClass = TEXT("ChattingClient_v0.01");
const TCHAR* g_ClientName = TEXT("채팅 클라");

MainClass* g_pMainClass = nullptr;

const int WINDOWX = 800;
const int WINDOWY = 600;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void setWindowsSize(int x, int y, int width, int height);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow)
{
	//메인 클래스 할당
	g_pMainClass = new MainClass;

	MSG message;		//윈도우 메시지 받을 구조체
	//COM객체 사용
	if (SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		WNDCLASSEX wndClass = { sizeof(WNDCLASSEX) };	//윈도우 정보를 담은 구조체

		g_hInstance = hInstance;

		wndClass.cbClsExtra = 0;											//클래스 여분 메모리
		wndClass.cbWndExtra = 0;											//윈도우 여분 메모리
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);		//윈도우 백그라운드 색상
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//윈도우 커서 모양
		wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);					//윈도우 아이콘 모양
		wndClass.hInstance = hInstance;										//윈도우 인스턴스
		wndClass.lpfnWndProc = (WNDPROC)WndProc;							//프로시져
		wndClass.lpszClassName = g_lpszClass;									//클래스 이름
		wndClass.lpszMenuName = NULL;										//메뉴 이름
		wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;							//윈도우 스타일

		//윈도우 정보를 등록
		RegisterClassEx(&wndClass);

		//Conifg에서 풀스크린 유무 가져옴
		if (g_pMainClass->GetConfig()->GetFullScreen())
		{
			//#ifdef FULLSCREEN
			DEVMODE	dm;		//디바이스 모드

			//디바이스모드를 0으로 초기화 한다
			ZeroMemory(&dm, sizeof(DEVMODE));

			dm.dmSize = sizeof(DEVMODE);
			dm.dmBitsPerPel = 32;			//트루컬러 32비트
			dm.dmPelsWidth = GetSystemMetrics(SM_CXSCREEN);			//해상도 가로길이
			dm.dmPelsHeight = GetSystemMetrics(SM_CYSCREEN);			//해상도 세로길이
			dm.dmDisplayFrequency = 60;		//재생빈도 60Hz
			dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

			//화면이 종료되면 자동으로 원래 화면의 해상도로 복구시켜주는 코드
			if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				ChangeDisplaySettings(&dm, 0);
			}

			//윈도우 창 생성
			g_hWnd = CreateWindow(
				g_lpszClass,						//윈도우 클래스 이름
				g_ClientName,						//윈도우 타이틀 바에 이름
				WS_POPUPWINDOW | WS_MAXIMIZE,	//윈도우 스타일
				0,								//윈도우 창 시작 X좌표
				0,								//윈도우 창 시작 Y좌표
				GetSystemMetrics(SM_CXSCREEN),	//윈도우 창 가로크기
				GetSystemMetrics(SM_CYSCREEN),	//윈도우 창 세로크기
				NULL,							//부모 윈도우 유무
				(HMENU)NULL,					//메뉴 핸들
				hInstance,						//인스턴스 윈도우 지정
				NULL);

			setWindowsSize(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

			//#else
		}
		else
		{
			//윈도우 창 생성
			g_hWnd = CreateWindow(
				g_lpszClass,				//윈도우 클래스 이름
				g_ClientName,				//윈도우 타이틀 바에 이름
				WS_OVERLAPPEDWINDOW,	//윈도우 스타일
				CW_USEDEFAULT,			//윈도우 창 시작 X좌표
				CW_USEDEFAULT,			//윈도우 창 시작 Y좌표
				static_cast<UINT>(ceil(640.f * 96.0f / 96.f)),					//윈도우 창 가로크기
				static_cast<UINT>(ceil(480.f * 96.0f / 96.f)),					//윈도우 창 세로크기
				NULL,					//부모 윈도우 유무
				(HMENU)NULL,			//메뉴 핸들
				hInstance,				//인스턴스 윈도우 지정
				NULL);

			//#endif
		}

		g_hChat = CreateWindow(L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 2000, 2000, 300, 30, g_hWnd, (HMENU)100, g_hInstance, NULL);
		PostMessage(g_hWnd, EM_LIMITTEXT, (WPARAM)30, 0);

		//게임 엔진 초기화
		if (SUCCEEDED(GAME_ENGINE->Init(true, g_pMainClass->GetConfig()->GetScreenWidth(), g_pMainClass->GetConfig()->GetScreenHeight())))
		{
			//화면에 윈도우 창을 보여준다
			ShowWindow(g_hWnd, cmdShow);

			//메인게임 초기화 실패면
			if (FAILED(g_pMainClass->Init()))
			{
				SAFE_DELETE(g_pMainClass);
				GAME_ENGINE->Destroy();
				CoUninitialize();
				return 0; //꺼버렷!
			}

			UpdateWindow(g_hWnd);

			while (true)
			{
				if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
				{
					if (message.message == WM_QUIT)
					{
						break;
					}
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
				else
				{
					//TIMEMANAGER->update(60.0f);
					g_pMainClass->Update();

					g_pMainClass->Render();
				}
			}
		}

		//메시지 루프 다돌았으면
		SAFE_RELEASE(g_pMainClass); //메인게임 해제
		SAFE_DELETE(g_pMainClass);

		//윈도우 등록된것 해제
		UnregisterClass(g_lpszClass, hInstance);

		GAME_ENGINE->Destroy();
	}
	else
	{
		SAFE_DELETE(g_pMainClass);
		return 0;
	}

	//COM 해제
	CoUninitialize();

	return (int)message.wParam;
}

//일단 나중에
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return g_pMainClass->MainProc(hWnd, iMessage, wParam, lParam);
}

//클라이언트 영역 재조정 함수
void setWindowsSize(int x, int y, int width, int height)
{
	RECT winRect;

	winRect.left = 0;
	winRect.top = 0;
	winRect.right = width;
	winRect.bottom = height;

	//비작업 영역을 제외한 작업 영역안의 크기만 맞춰줌.
	AdjustWindowRect(&winRect, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, false);

	SetWindowPos(g_hWnd, NULL, x, y, (winRect.right - winRect.left),
		(winRect.bottom - winRect.top), SWP_NOZORDER | SWP_NOMOVE);
}
