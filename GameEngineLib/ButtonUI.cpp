#include "stdafx.h"
#include "ButtonUI.h"

ButtonUI::ButtonUI(const TCHAR* nodeName)
	:UINode(nodeName),
	m_Color(D2D1::ColorF(1.0, 1.0, 1.0)),
	m_Clicked(false)
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

	//Active��ٸ� ���� ǥ��
	if (IsActive())
	{
		m_Clicked = true;
	}
	//active ���¿��� �ش� ��ư��ġ���� ���콺 ���ٸ� �ݹ� ó��
	if(!IsLeftButtonDown())
	{
		if (m_Clicked)
		{
			D2D1_SIZE_F RenderTGsize = D2D_RENDERTARGET->GetSize();
			D2D1_SIZE_F BitTGSize = DIRECT2D->GetBackBufferTarget()->GetSize();

			float ratioWidth = BitTGSize.width / RenderTGsize.width;
			float ratioHeight = BitTGSize.height / RenderTGsize.height;

			RECT rc = { (int)(m_ButtonRect.left),
				(int)(m_ButtonRect.top),
				(int)(m_ButtonRect.right),
				(int)(m_ButtonRect.bottom)
			};

			POINT pt = GetptMouse();
			pt.x = (LONG)(pt.x * ratioWidth);
			pt.y = (LONG)(pt.y * ratioHeight);

			if (PtInRect(&rc, pt))
			{
				m_CallBackFuntion();
			}
		}
		m_bActive = false;
		m_Clicked = false;
	}
}

void ButtonUI::Render()
{
	if (IsRectRender())
	{
		D2D_PRIMITEVS->DrawFillRect(
			m_ButtonRect,
			D2D1::ColorF(1, 0, 0)
		);
	}

	//TODO : ���� �ؾ���
	//UI Ȱ�����̸� ���� �̹����� rect �׷���
	if (IsActive())
	{
		D2D_PRIMITEVS->DrawFillRect(
			m_ButtonRect,
			D2D1::ColorF(1.0f, 0.0f, 0.6f)
		);
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
