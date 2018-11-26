#include "stdafx.h"
#include "UINode.h"

UINode::UINode(const TCHAR* nodeName)
	:GameNode(nodeName),
	m_UIRect(D2D1::RectF()),
	m_bActive(false),
	m_bRectRender(true)
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

void UINode::Render()
{
	GameNode::Render();

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

LRESULT UINode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return GameNode::MainProc(hWnd, iMessage, wParam, lParam);
}
