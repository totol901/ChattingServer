#include "stdafx.h"
#include "UINode.h"

UINode::UINode(const TCHAR* nodeName)
	:GameNode(nodeName),
	m_UIRect(D2D1::RectF()),
	m_bActive(false),
	m_bRectRender(true),
	m_bOn(false)
{
}

UINode::~UINode()
{
	//자식노드들 릴리즈하고 삭제함
	UINode* temp = (UINode*)this->GetChildrenHead();
	while (1)
	{
		if (temp != nullptr)
		{
			UINode* befNode = (UINode*)temp;
			temp = (UINode*)temp->GetNext();
			SAFE_DELETE(befNode);
		}
		else
		{
			break;
		}
	}
}

void UINode::AddChildButtonUI(const TCHAR* ButtonName, D2D1_RECT_F ButtonRect, CALLBACK_FUNCTION_BUTTON func, const TCHAR * staticStr, D2D1_RECT_F StaticStrRect, D2D1::ColorF TextColor)
{
	ButtonUI* pButtonUI = nullptr;
	pButtonUI = new ButtonUI(ButtonName);
	ButtonRect.left += m_UIRect.left;
	ButtonRect.top += m_UIRect.top;
	ButtonRect.right += m_UIRect.left;
	ButtonRect.bottom += m_UIRect.top;
	pButtonUI->Init(ButtonRect, func, staticStr, StaticStrRect, TextColor);
	AddChild(pButtonUI);
}

void UINode::AddChildTextBoxUI(const TCHAR* TextBoxName, D2D1_RECT_F TextBoxRect, UINT MaxTextLen, D2D1::ColorF TextColor)
{
	TextBoxUI* pTextBox = nullptr;
	pTextBox = new TextBoxUI(TextBoxName);
	TextBoxRect.left += m_UIRect.left;
	TextBoxRect.top += m_UIRect.top;
	TextBoxRect.right += m_UIRect.left;
	TextBoxRect.bottom += m_UIRect.top;
	pTextBox->Init(TextBoxRect, MaxTextLen, TextColor);
	AddChild(pTextBox);
}

void UINode::AddChildStaticTextUI(const TCHAR * StaticTextName, D2D1_RECT_F UIRect, const TCHAR * staticText, D2D1::ColorF TextColor, bool isUIBoxRender)
{
	StaticTextUI* pStaticText = nullptr;
	pStaticText = new StaticTextUI(StaticTextName);
	UIRect.left += m_UIRect.left;
	UIRect.top += m_UIRect.top;
	UIRect.right += m_UIRect.left;
	UIRect.bottom += m_UIRect.top;
	pStaticText->Init(UIRect, staticText, TextColor);
	pStaticText->SetRectRender(isUIBoxRender);
	AddChild(pStaticText);
}

UINode * UINode::FindUI(const TCHAR * UIName)
{
	UINode* temp = (UINode*)GetChildrenHead();

	while (temp)
	{
		if (temp->GetNodeName() == UIName)
		{
			return temp;
		}
		temp = (UINode*)temp->GetNext();
	}

	return nullptr;
}

void UINode::OnActive()
{
	if (IsLeftButtonDown())
	{
		D2D1_SIZE_F RenderTGsize = D2D_RENDERTARGET->GetSize();
		D2D1_SIZE_F BitTGSize = DIRECT2D->GetBackBufferTarget()->GetSize();

		float ratioWidth = BitTGSize.width / RenderTGsize.width;
		float ratioHeight = BitTGSize.height / RenderTGsize.height;

		RECT rc = { (int)(m_UIRect.left),
			(int)(m_UIRect.top),
			(int)(m_UIRect.right),
			(int)(m_UIRect.bottom)
		};

		POINT pt = GetptMouse();
		pt.x = (LONG)(pt.x * ratioWidth);
		pt.y = (LONG)(pt.y * ratioHeight);

		if (PtInRect(&rc, pt))
		{
			m_bActive = true;
		}
		
	}
}

HRESULT UINode::Init(D2D1_RECT_F UIRect)
{
	GameNode::Init();

	m_UIRect = UIRect;

	return S_OK;
}

void UINode::Release()
{
	GameNode::Release();

	//자식노드들 릴리즈하고 삭제함
	UINode* temp = (UINode*)this->GetChildrenHead();
	while (1)
	{
		if (temp != nullptr)
		{
			UINode* befNode = (UINode*)temp;
			temp->Release();
			temp = (UINode*)temp->GetNext();
		}
		else
		{
			break;
		}
	}
}

void UINode::Update()
{
	GameNode::Update();

	if (IsOn())
	{
		UINode* temp = (UINode*)this->GetChildrenHead();
		while (1)
		{
			if (temp != nullptr)
			{
				temp->Update();
				temp = (UINode*)temp->GetNext();
			}
			else
			{
				break;
			}
		}
	}
}

void UINode::Render()
{
	GameNode::Render();

	if (IsOn())
	{
		D2D_PRIMITEVS->DrawFillRect(
			m_UIRect,
			D2D1::ColorF(0.5, 0.25, 0)
		);

		//자식 랜더
		if (m_bRectRender)
		{
			UINode* temp = (UINode*)this->GetChildrenHead();
			while (1)
			{
				if (temp != nullptr)
				{
					temp->Render();
					temp = (UINode*)temp->GetNext();
				}
				else
				{
					break;
				}
			}
		}
	}
}

LRESULT UINode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return GameNode::MainProc(hWnd, iMessage, wParam, lParam);
}
