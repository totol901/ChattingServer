#pragma once

class TextBoxUI : public UINode
{
private:
	D2D1_RECT_F m_TextBoxRect;
	D2D1::ColorF m_Color;

	TCHAR* m_pTextStr;
	UINT m_MaxTextSize;

	bool m_bChattingFocusSwitch;

public:
	TextBoxUI(const TCHAR* nodeName);
	~TextBoxUI();

	HRESULT Init(D2D1_RECT_F TextBoxRect, UINT MaxTextSize, D2D1::ColorF TextColor);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

