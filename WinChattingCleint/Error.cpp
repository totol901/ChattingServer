#include "stdafx.h"
#include "Error.h"

Error::Error()
	:m_pPopupWaitingUI(nullptr),
	m_pPopupErrorUI(nullptr)
{
}

Error::~Error()
{
	SAFE_DELETE(m_pPopupWaitingUI);
}

void Error::Init()
{
	m_pPopupWaitingUI = new UINode(TEXT("LoginPopupUI"));
	D2D_SIZE_F size = DIRECT2D->GetBackBufferTarget()->GetSize();
	HRESULT hr = m_pPopupWaitingUI->Init(
		D2D_PRIMITEVS->RectFMakeCenter(
			size.width * 0.5f,
			size.height * 0.5f,
			size.width * 0.5f,
			size.height * 0.35f)
	);

	//UI 자식 초기화
	m_pPopupWaitingUI->AddChildStaticTextUI(
		TEXT("LoginPopupLoadingText"),
		D2D_PRIMITEVS->RectFMake(
			70,
			50,
			25.0f,
			30.0f),
		TEXT("네트워크 처리 지연중....."),
		D2D1::ColorF(1.0f, 1.0f, 1.0f),
		false
	);
}

void Error::err_quit(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void Error::err_display(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void Error::err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[오류] %s", (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void Error::err_print(const TCHAR * str)
{
	cout << str << endl;
}

void Error::ErrUpdate()
{
	if (m_pPopupErrorUI)
	{
		m_pPopupErrorUI->Update();
	}
}

void Error::ErrRender()
{
	m_pPopupWaitingUI->Render();

	if (m_pPopupErrorUI)
	{
		m_pPopupErrorUI->Render();
	}
}
