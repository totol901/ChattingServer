/******************************************************************************
���ϸ�	: MainClass.h
����		: ���� ����, ���� ����͸�, ��ɾ �����
******************************************************************************/
#pragma once


class MainClass
{
private:
	bool m_isOn;

public:
	MainClass();
	~MainClass();

	BOOL Init();
	void Release();
	void Update();
	
	bool IsOn() { return m_isOn; }

	void Monitering();

	static unsigned int WINAPI MoniteringThread(LPVOID param);
};
