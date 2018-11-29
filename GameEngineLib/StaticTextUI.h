#pragma once

class StaticTextUI : public UINode
{
private:
	D2D1_RECT_F m_StaticTextRect;
	D2D1::ColorF m_Color;

	wstring m_pTextStr;
	
public:
	StaticTextUI(const TCHAR* nodeName);
	~StaticTextUI();

	void SetTextStr(const TCHAR* str) { m_pTextStr = str; }

	HRESULT Init(D2D1_RECT_F UIRect, const TCHAR* staticText, D2D1::ColorF TextColor);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
