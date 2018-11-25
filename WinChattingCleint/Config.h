#pragma once

class Config
{
private:
	FILE* m_pFile;

	TCHAR* m_strIP;
	u_short m_Port;
	int m_ScreenWidth;
	int m_ScreenHeight;
	bool m_FullScreen;

	void InputConfigData();

public:
	Config();
	~Config();

	HRESULT Init();

	TCHAR* ReadConfigValue(TCHAR* strSection, TCHAR* strkey);

	TCHAR* GetIP() { return m_strIP; }
	u_short GetPort() { return m_Port; }
	int GetScreenWidth() { return m_ScreenWidth; }
	int GetScreenHeight() { return m_ScreenHeight; }
	bool GetFullScreen() { return m_FullScreen; }
};
