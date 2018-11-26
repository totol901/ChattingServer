/******************************************************************************
���ϸ�	: LoginScene.h
����		: �α��ξ� ó��
******************************************************************************/
#pragma once

class LoginScene : public BaseScene
{
private:
	UINode* m_pLoginUI;
	TextBoxUI* m_pIDTextBoxUI;
	TextBoxUI* m_pPassTextBoxUI;

public:
	LoginScene(const WCHAR* nodeName, UINT nodeNum);
	~LoginScene();

	HRESULT Init();
	void Release();
	void Update();
	void Render();
	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
