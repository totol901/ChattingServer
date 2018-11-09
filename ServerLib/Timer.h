/******************************************************************************
파일명	: Timer.h
목적		: 시간관련 클래스
******************************************************************************/
#pragma once

class Timer : public Singleton<Timer>
{
	friend Singleton;
private:
	Timer();
	~Timer();

	clock_t m_NowTime;
	clock_t m_LastTime;
	tm		m_TM;

	void Init();

public:
	const tm&	GetTM();
	string		NowTimeWithMilliSec();
	string		NowTimeWithSec();
	string		Today();
	void		Update();

	double ElipsedSec();
};
