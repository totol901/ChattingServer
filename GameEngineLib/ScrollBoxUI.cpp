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

	//위치 조정
	D2D1_RECT_F rect = uiRect;
	rect.left = m_UIRect.left + rect.left;
	rect.right = m_UIRect.left + width;
	rect.bottom = m_UIRect.bottom;
	rect.top = m_UIRect.bottom - height;

	//UI 생성
	StaticTextUI* pStaticTextUI = nullptr;
	pStaticTextUI = new StaticTextUI(UIName);
	pStaticTextUI->Init(rect, staticText, TextColor);

	//자리 꽉 차면 자리 만듬
	if (m_UIList.size() >= m_ListSize)
	{
		auto iter = m_UIList.back();
		SAFE_RELEASE(iter);
		SAFE_DELETE(iter);
		m_UIList.pop_back();
	}

	//들어갈 UI 크기만큼 자리 이동
	for (auto iter = m_UIList.begin(); iter != m_UIList.end(); iter++)
	{
		(*iter)->GetUIRect().bottom += width;
		(*iter)->GetUIRect().top += width;
	}

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
}

LRESULT ScrollBoxUI::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}
