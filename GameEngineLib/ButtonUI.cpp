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
	//Active ����� Ȯ��
	OnActive();

	//Active��ٸ� Ű�� �ޱ����� �ؽ�Ʈ ����Ʈ�� ��Ŀ����
	if (IsActive())
	{
		
	}
	//�ƴϸ� ��Ŀ�� �������� ����
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

	//TODO : ���� �ؾ���
	//UI Ȱ�����̸� ���� �̹����� rect �׷���
	if (IsActive())
	{
		
	}

	if (m_pStaticStr.length() != 0)
	{
		//Text �׷���
		D2D_TEXTMANAGER->TextRender(m_pStaticStr.c_str(), TEXT("����_15"),
			m_StaticStrRect,
			m_Color);
	}
}

LRESULT ButtonUI::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return UINode::MainProc(hWnd, iMessage, wParam, lParam);
}
