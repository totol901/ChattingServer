/******************************************************************************
파일명	: MainClass.h
목적		: 메인 로직, 서버 모니터링, 명령어를 담당함
******************************************************************************/
#pragma once

class MainClass
{
private:
	bool m_isOn;

public:
	MainClass();
	~MainClass();

	void Update();
	bool IsOn() { return m_isOn; }

	static unsigned int WINAPI MoniteringThread(LPVOID param);
};
