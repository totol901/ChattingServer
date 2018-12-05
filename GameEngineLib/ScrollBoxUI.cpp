#include "stdafx.h"
#include "ScrollBoxUI.h"

ScrollBoxUI::ScrollBoxUI(const TCHAR* scrollBoxName, const UINT& listSize)
	:UINode(scrollBoxName),
	m_ListSize(listSize)
{
	m_UIList.resize(listSize);
}

ScrollBoxUI::~ScrollBoxUI()
{
	for (auto iter = m_UIList.begin(); iter != m_UIList.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
}

void ScrollBoxUI::AddStaticTextBox(const TCHAR* UIName, const D2D1_RECT_F& uiRect, const TCHAR* staticText, const D2D1::ColorF& TextColor)
{
	float width = uiRect.right - uiRect.left;
	float height = uiRect.bottom - uiRect.top;

	//��ġ ����
	D2D1_RECT_F rect = uiRect;
	rect.left = m_UIRect.left + rect.left;
	rect.right = m_UIRect.left + width;
	rect.bottom = m_UIRect.bottom;
	rect.top = m_UIRect.bottom - height;

	//UI ����
	StaticTextUI* pStaticTextUI = nullptr;
	pStaticTextUI = new StaticTextUI(UIName);
	pStaticTextUI->Init(rect, staticText, TextColor);

	//�ڸ� �� ���� �ڸ� ����
	if (m_UIList.size() >= m_ListSize)
	{
		auto iter = m_UIList.back();
		SAFE_RELEASE(iter);
		SAFE_DELETE(iter);
		m_UIList.pop_back();
	}

	//�� UI ũ�⸸ŭ �ڸ� �̵�
	for (auto iter = m_UIList.begin(); iter != m_UIList.end(); iter++)
	{
		(*iter)->GetUIRect().bottom += height;
		(*iter)->GetUIRect().top += height;
	}

	//����Ʈ�� �־���
	m_UIList.push_back(pStaticTextUI);
}

HRESULT ScrollBoxUI::Init(D2D1_RECT_F UIRect)
{
	HRESULT hr = UINode::Init(UIRect);

	


	return hr;
}

void ScrollBoxUI::Release()
{
	for (auto iter = m_UIList.begin(); iter != m_UIList.end(); iter++)
	{
		(*iter)->Release();
	}
}

void ScrollBoxUI::Update()
{

}

void ScrollBoxUI::Render()
{
	//UI ���� �Ʒ��� �� ��Ʈ �׷���
	if (IsRectRender())
	{
		D2D_PRIMITEVS->DrawFillRect(
			m_UIRect,
			D2D1::ColorF(1, 0, 0)
		);
	}

	//���� �Ʒ��� �� UI���� ũ�� �׷����� ©�� �׸�
	for (auto iter = m_UIList.begin(); iter != m_UIList.end(); iter++)
	{
		//�� ���ķδ� �׸��� ����
		if ((*iter)->GetUIRect().bottom < m_UIRect.top)
		{
			break;
		}

		//�׷���� ��
		//if(DIRECT2D->GetBackBufferTarget()->draw
	}
}

LRESULT ScrollBoxUI::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		
	}

	return 0;
}
