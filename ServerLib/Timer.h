#pragma once

class Timer : public Singleton<Timer>
{
	friend Singleton;
private:
	Timer();
	~Timer();

	time_t m_NowTime;
	time_t m_LastTime;
	tm m_TM;
	void Init();

public:
	const tm& GetTM();
	string NowTimeWithMilliSec();
};

