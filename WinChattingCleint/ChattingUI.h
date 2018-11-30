#pragma once

class ChattingUI
{
private:
	UINode* m_pChattingUI;

public:
	ChattingUI(const TCHAR* nodeName);
	~ChattingUI();

	HRESULT Init(D2D1_RECT_F UIRect);
	void Release();
	void Update();
	void Render();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
