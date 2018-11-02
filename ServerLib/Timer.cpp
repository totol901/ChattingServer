#include "stdafx.h"
#include "Timer.h"

Timer::Timer()
{
	Init();
}

Timer::~Timer()
{
}

void Timer::Init()
{
	m_NowTime = time(NULL);
	m_LastTime = time(NULL);
}

const tm & Timer::GetTM()
{
	m_NowTime = time(NULL);
	localtime_s(&m_TM, &m_NowTime);
	
	return m_TM;
}

string Timer::NowTimeWithMilliSec()
{
	char currentTime[128] = "";
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(currentTime, "%d-%d-%d_%d_%d_%d_%d", 
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return currentTime;
}
