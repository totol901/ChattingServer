#include "stdafx.h"
#include "TextBoxUI.h"

TextBoxUI::TextBoxUI(const TCHAR* nodeName)
	:UINode(nodeName),
	m_bChattingFocusSwitch(false),
	m_Color(D2D1::ColorF(1.0, 1.0, 1.0)),
	m_MaxTextSize(0),
	m_pTextStr(nullptr),
	m_CurrentTextPos(0),
	m_bCharToStar(false)
{
}

TextBoxUI::~TextBoxUI()
{
	SAFE_DELETE_ARRAY(m_pTextStr);
}

void TextBoxUI::SetCharToStar(bool isStar)
{
	m_bCharToStar = isStar;
}

void TextBoxUI::OnActive()
{
	if (IsLeftButtonDown())
	{
		D2D1_SIZE_F RenderTGsize = D2D_RENDERTARGET->GetSize();
		D2D1_SIZE_F BitTGSize = DIRECT2D->GetBackBufferTarget()->GetSize();

		float ratioWidth = BitTGSize.width / RenderTGsize.width;
		float ratioHeight = BitTGSize.height / RenderTGsize.height;

		RECT rc = { (int)(m_TextBoxRect.left),
			(int)(m_TextBoxRect.top),
			(int)(m_TextBoxRect.right),
			(int)(m_TextBoxRect.bottom)
		};

		POINT pt = GetptMouse();
		pt.x = (LONG)(pt.x * ratioWidth);
		pt.y = (LONG)(pt.y * ratioHeight);

		if (PtInRect(&rc, pt))
		{
			m_bActive = true;
		}
		//�ƴϸ� ��Ŀ�� �������� ����
		else
		{
			m_bActive = false;
			m_bChattingFocusSwitch = false;

			HideCaret(g_hWnd);
			DestroyCaret();
		}
	}
}

HRESULT TextBoxUI::Init(D2D1_RECT_F TextBoxRect, UINT MaxTextSize, D2D1::ColorF TextColor)
{
	HRESULT hr = UINode::Init(TextBoxRect);

	m_TextBoxRect = TextBoxRect;

	m_MaxTextSize = MaxTextSize;
	m_pTextStr = new TCHAR[m_MaxTextSize + 1];
	memset(m_pTextStr, 0x00, m_MaxTextSize + 1);
	
	m_Color = TextColor;

	return hr;
}

void TextBoxUI::Release()
{
	memset(m_pTextStr, 0x00, m_MaxTextSize);
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
		}
	}
}

void TextBoxUI::Render()
{
	if (IsRectRender())
	{
		D2D_PRIMITEVS->DrawFillRect(
			m_TextBoxRect,
			D2D1::ColorF(1, 0.5, 0)
		);
	}

	//UI Ȱ�����̸� Text ��� �޾ƿ�
	if (IsActive())
	{
		// TODO : Ŀ�� ������ �����ؾ���
		//int Start_sel;
		//int End_sel;
		//SendMessage(m_hChattingExitBox, EM_GETSEL, (WPARAM)&Start_sel, (LPARAM)&End_sel);
		//m_pTextStr[End_sel] = L'\0';
		//RECT rt = { 0, };
		//// D3DXFont�� ���ŵ��Ű�, WINAPI�� DrawText�� ���ŵ� �ſ� ��  
		//Font->DrawText(Sprite, chat, -1, &rt, DT_CALCRECT, ORG_DX_RGBA(255, 255, 255, 255));
		//// 0.5�ʸ��� �����Ÿ���! (ä��â�� Ȱ��ȭ �Ǿ����� ��쿡��)  
		//if (timeGetTime() % 1000 >= 500 && GetFocus() == hChat)
		//{
		//	// ä�� ��ǥ + rt.right ��ġ�� Draw!  
		//}
	}

	if (m_bCharToStar)
	{
		TCHAR str[15] = {0, };
		
		for (int i = 0; i < m_CurrentTextPos; i++)
		{
			str[i] = L'*';
		}
		
		//�޾ƿ� Text �׷���
		D2D_TEXTMANAGER->TextRender(str, TEXT("����_15"),
			m_TextBoxRect,
			m_Color);
	}
	else
	{
		D2D_TEXTMANAGER->TextRender(m_pTextStr, TEXT("����_15"),
			m_TextBoxRect,
			m_Color);
	}
}

LRESULT TextBoxUI::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (m_bChattingFocusSwitch)
	{
		switch (iMessage)
		{
		case WM_CHAR:
			switch (wParam)
			{
			case VK_BACK:
				if (m_CurrentTextPos > 0)
				{
					m_CurrentTextPos--;
					m_pTextStr[m_CurrentTextPos] = 0x00;
				}
				break;

			case VK_SPACE:
				m_pTextStr[m_CurrentTextPos] = TEXT(' ');
				if (m_CurrentTextPos < m_MaxTextSize-1)
				{
					m_CurrentTextPos++;
					m_pTextStr[m_CurrentTextPos] = 0x00;
				}
				break;

			default:
				if (m_CurrentTextPos < m_MaxTextSize-1)
				{
					m_pTextStr[m_CurrentTextPos] = wParam;
					m_CurrentTextPos++;
					m_pTextStr[m_CurrentTextPos] = 0x00;
				}
				break;
			}
			
			break;
		
		}
	}

	return 0;
}
