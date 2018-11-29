/******************************************************************************
파일명	: LoginScene.h
목적		: 로그인씬 처리
******************************************************************************/
#pragma once

enum eLoginSceneState
{
	LOGIN_COMMON,
	LOGIN_WAIT
};

class Player;
class LoginScene : public BaseScene
{
private:
	eLoginSceneState m_eLoginState;
	UINode* m_pLoginUI;
	
	TextBoxUI* m_pIDTextBoxUI;
	TextBoxUI* m_pPassTextBoxUI;

	HRESULT InitLoginUI();

	bool m_bStartLogin;

public:
	LoginScene(const WCHAR* nodeName, UINT nodeNum);
	~LoginScene();

	eLoginSceneState GetState() { return m_eLoginState; }
	void SetState(const eLoginSceneState& state) { m_eLoginState = state; }
	
	void SendLogin();
	void SendCreateID();

	///void SetPopupErrorUI(UINode* UINode) { m_pPopupErrorUI = UINode; }

	static unsigned int WINAPI LoginRecvThread(LPVOID param);

	HRESULT Init();
	void Release();
	void Update();
	void Render();
	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
