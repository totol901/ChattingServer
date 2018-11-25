/******************************************************************************
���ϸ�	: MainClass.h
����		: ChattingClient ��ü, ���� ���� �����
******************************************************************************/
#pragma once

class Config;
class MainClass : public GameNode
{
private:
	bool isOn;
	Config* m_pConfig;

	void MakeConsole();
	void ReleaseConsole();

public:
	MainClass();
	~MainClass();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	Config* GetConfig() { return m_pConfig; }
	LRESULT CALLBACK MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
