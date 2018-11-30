#include "stdafx.h"
#include "ChattingUI.h"

ChattingUI::ChattingUI(const TCHAR* nodeName)
	:m_pChattingUI(nullptr)
{
}

ChattingUI::~ChattingUI()
{
	SAFE_DELETE(m_pChattingUI);
}

HRESULT ChattingUI::Init(D2D1_RECT_F UIRect)
{
	m_pChattingUI = new UINode(TEXT("ChattingUI"));
	HRESULT hr = m_pChattingUI->Init(UIRect);

	//m_pChattingUI->add

	return hr;
}

void ChattingUI::Release()
{
}

void ChattingUI::Update()
{
}

void ChattingUI::Render()
{
}

LRESULT ChattingUI::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}
