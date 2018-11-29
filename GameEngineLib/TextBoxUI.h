#pragma once

class TextBoxUI : public UINode
{
private:
	D2D1_RECT_F m_TextBoxRect;
	D2D1::ColorF m_Color;

	TCHAR* m_pTextStr;
	UINT m_MaxTextSize;
	UINT m_CurrentTextPos;

	bool m_bChattingFocusSwitch;
	bool m_bCharToStar;
	
public:
	TextBoxUI(const TCHAR* nodeName);
	~TextBoxUI();

	void SetCharToStar(bool isStar);

	virtual void OnActive();
	TCHAR* GetText() { return m_pTextStr; }

	HRESULT Init(D2D1_RECT_F TextBoxRect, UINT MaxTextSize, D2D1::ColorF TextColor);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	

	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

