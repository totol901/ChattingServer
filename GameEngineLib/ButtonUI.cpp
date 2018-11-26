#include "stdafx.h"
#include "ButtonUI.h"

ButtonUI::ButtonUI(const TCHAR* nodeName)
	:UINode(nodeName),
	m_Color(D2D1::ColorF(1.0, 1.0, 1.0))
{
}

ButtonUI::~ButtonUI()
{
}

HRESULT ButtonUI::Init(D2D1_RECT_F ButtonRect, CALLBACK_FUNCTION_BUTTON func, const TCHAR * staticStr, D2D1_RECT_F StaticStrRect, D2D1::ColorF TextColor)
{
	HRESULT hr = UINode::Init(ButtonRect);

	m_ButtonRect = ButtonRect;

	m_CallBackFuntion = func;

	if (staticStr)
	{
		m_StaticStrRect.left = m_ButtonRect.left + StaticStrRect.left;
		m_StaticStrRect.top = m_ButtonRect.top + StaticStrRect.top;
		m_StaticStrRect.right = m_ButtonRect.left + StaticStrRect.right;
		m_StaticStrRect.bottom = m_ButtonRect.top + StaticStrRect.bottom;

		m_pStaticStr = staticStr;

		m_Color = TextColor;
	}
	
	return hr;
}

void ButtonUI::Release()
{
}

void ButtonUI::Update()
{
	//Active 됬는지 확인
	OnActive();

	//Active됬다면 키값 받기위해 텍스트 에디트에 포커싱함
	if (IsActive())
	{
		
	}
	//아니면 포커싱 메인으로 변경
	else
	{

	}
}

void ButtonUI::Render()
{
	if (IsRectRender())
	{
		D2D_PRIMITEVS->DrawRect(
			m_ButtonRect,
			D2D1::ColorF(1, 0, 0),
			1);
	}

	//TODO : 구현 해야함
	//UI 활동중이면 눌린 이미지나 rect 그려줌
	if (IsActive())
	{
		
	}

	if (m_pStaticStr.length() != 0)
	{
		//Text 그려줌
		D2D_TEXTMANAGER->TextRender(m_pStaticStr.c_str(), TEXT("굴림_15"),
			m_StaticStrRect,
			m_Color);
	}
}

LRESULT ButtonUI::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return UINode::MainProc(hWnd, iMessage, wParam, lParam);
}
