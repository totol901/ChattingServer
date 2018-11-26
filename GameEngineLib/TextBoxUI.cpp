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
	//Active ����� Ȯ��
	OnActive();
	
	//Active��ٸ� Ű�� �ޱ����� �ؽ�Ʈ ����Ʈ�� ��Ŀ����
	if (IsActive())
	{
		if (!m_bChattingFocusSwitch)
		{
			m_bChattingFocusSwitch = true;
			SetWindowText(g_hChat, m_pTextStr);
		}
		SetFocus(g_hChat);
	}
	//�ƴϸ� ��Ŀ�� �������� ����
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

	//UI Ȱ�����̸� Text ��� �޾ƿ�
	if (IsActive())
	{
		GetWindowText(g_hChat, m_pTextStr, m_MaxTextSize);

		// TODO : Ŀ�� ������ �����ؾ���
		//int Start_sel;
		//int End_sel;
		//SendMessage(hChat, EM_GETSEL, (WPARAM)&Start_sel, (LPARAM)&End_sel);
		//Chatting[End_sel] = L'\0';
		//RECT rt = { 0, };
		//// D3DXFont�� ���ŵ��Ű�, WINAPI�� DrawText�� ���ŵ� �ſ� ��  
		//Font->DrawText(Sprite, chat, -1, &rt, DT_CALCRECT, ORG_DX_RGBA(255, 255, 255, 255));
		//// 0.5�ʸ��� �����Ÿ���! (ä��â�� Ȱ��ȭ �Ǿ����� ��쿡��)  
		//if (timeGetTime() % 1000 >= 500 && GetFocus() == hChat)
		//{
		//	// ä�� ��ǥ + rt.right ��ġ�� Draw!  
		//}
	}

	//�޾ƿ� Text �׷���
	D2D_TEXTMANAGER->TextRender(m_pTextStr, TEXT("����_15"),
		m_TextBoxRect,
		m_Color);
}

LRESULT TextBoxUI::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return UINode::MainProc(hWnd, iMessage, wParam, lParam);
}
