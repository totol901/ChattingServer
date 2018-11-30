#pragma once

class ScrollBoxUI : public UINode
{
private:
	list<UINode*> m_UIList;
	UINT m_ListSize;

public:
	ScrollBoxUI(const TCHAR* scrollBoxName, const UINT& listSize);
	~ScrollBoxUI();

	void AddStaticTextBox(const TCHAR* UIName, const D2D1_RECT_F& uiRect, const TCHAR* staticText, const D2D1::ColorF& TextColor);
	
	HRESULT Init(D2D1_RECT_F UIRect);
	void Release();
	void Update();
	void Render();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
