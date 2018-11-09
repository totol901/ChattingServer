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
