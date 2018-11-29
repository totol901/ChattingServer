#include "stdafx.h"
#include "StaticTextUI.h"

StaticTextUI::StaticTextUI(const TCHAR* nodeName)
	:UINode(nodeName),
	m_Color(D2D1::ColorF(0.0f, 0.0f, 0.0f))
{
}

StaticTextUI::~StaticTextUI()
{
}

HRESULT StaticTextUI::Init(D2D1_RECT_F UIRect, const TCHAR* staticText, D2D1::ColorF TextColor)
{
	HRESULT hr = UINode::Init(UIRect);
	m_StaticTextRect = UIRect;
	m_pTextStr = staticText;
	m_Color = TextColor;

	return hr;
}

void StaticTextUI::Release()
{
}

void StaticTextUI::Update()
{
}

void StaticTextUI::Render()
{
	if (IsRectRender())
	{
		D2D_PRIMITEVS->DrawFillRect(
			m_StaticTextRect,
			D2D1::ColorF(1, 0.5, 0)
		);
	}

	//¹Þ¾Æ¿Â Text ±×·ÁÁÜ
	D2D_TEXTMANAGER->TextRender(m_pTextStr, TEXT("±¼¸²_15"),
		m_StaticTextRect,
		m_Color);
}

LRESULT StaticTextUI::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return UINode::MainProc(hWnd, iMessage, wParam, lParam);
}
