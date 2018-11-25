/******************************************************************************
파일명	: MainClass.h
목적		: ChattingClient 객체, 메인 로직 담당함
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
