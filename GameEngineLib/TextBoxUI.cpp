#include "stdafx.h"
#include "TextBoxUI.h"

TextBoxUI::TextBoxUI(const TCHAR* nodeName)
	:UINode(nodeName),
	m_bChattingFocusSwitch(false),
	m_Color(D2D1::ColorF(1.0, 1.0, 1.0)),
	m_MaxTextSize(0),
	m_pTextStr(nullptr)
{
}

TextBoxUI::~TextBoxUI()
{
	SetFocus(g_hWnd);
	SAFE_DELETE_ARRAY(m_pTextStr);
}

HRESULT TextBoxUI::Init(D2D1_RECT_F TextBoxRect, UINT MaxTextSize, D2D1::ColorF TextColor)
{
	HRESULT hr = UINode::Init(TextBoxRect);

	m_TextBoxRect = TextBoxRect;

	m_MaxTextSize = MaxTextSize;
	m_pTextStr = new TCHAR[m_MaxTextSize + 1];
	memset(m_pTextStr, 0, m_MaxTextSize + 1);
	
	m_Color = TextColor;

	return hr;
}

void TextBoxUI::Release()
{
	memset(m_pTextStr, 0, m_MaxTextSize);
}

void TextBoxUI::Update()
{
	//Active 됬는지 확인
	OnActive();
	
	//Active됬다면 키값 받기위해 텍스트 에디트에 포커싱함
	if (IsActive())
	{
		if (!m_bChattingFocusSwitch)
		{
			m_bChattingFocusSwitch = true;
			SetWindowText(g_hChat, m_pTextStr);
		}
		SetFocus(g_hChat);
	}
	//아니면 포커싱 메인으로 변경
	else
	{
		m_bChattingFocusSwitch = false;
		SetFocus(g_hWnd);
	}
}

void TextBoxUI::Render()
{
	if (IsRectRender())
	{
		D2D_PRIMITEVS->DrawRect(
			m_TextBoxRect,
			D2D1::ColorF(1, 0, 0),
			1);
	}

	//UI 활동중이면 Text 계속 받아옴
	if (IsActive())
	{
		GetWindowText(g_hChat, m_pTextStr, m_MaxTextSize);

		// TODO : 커서 껌뻑임 구현해야함
		//int Start_sel;
		//int End_sel;
		//SendMessage(hChat, EM_GETSEL, (WPARAM)&Start_sel, (LPARAM)&End_sel);
		//Chatting[End_sel] = L'\0';
		//RECT rt = { 0, };
		//// D3DXFont를 쓰셔도돼고, WINAPI의 DrawText를 쓰셔도 돼요 ㅎ  
		//Font->DrawText(Sprite, chat, -1, &rt, DT_CALCRECT, ORG_DX_RGBA(255, 255, 255, 255));
		//// 0.5초마다 깜빡거리게! (채팅창이 활성화 되어있을 경우에만)  
		//if (timeGetTime() % 1000 >= 500 && GetFocus() == hChat)
		//{
		//	// 채팅 좌표 + rt.right 위치에 Draw!  
		//}
	}

	//받아온 Text 그려줌
	D2D_TEXTMANAGER->TextRender(m_pTextStr, TEXT("굴림_15"),
		m_TextBoxRect,
		m_Color);
}

LRESULT TextBoxUI::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return UINode::MainProc(hWnd, iMessage, wParam, lParam);
}
