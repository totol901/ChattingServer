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
	m_NowTime = clock();
	m_LastTime = clock();
}

const tm & Timer::GetTM()
{
	time_t m_NowTime = time(NULL);
	localtime_s(&m_TM, &m_NowTime);
	
	return m_TM;
}

string Timer::NowTimeWithMilliSec()
{
	char currentTime[128] = "";
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(currentTime, "%d-%d-%d %d:%d:%d.%d", 
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return currentTime;
}

string Timer::NowTimeWithSec()
{
	char currentTime[128] = "";
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(currentTime, "%d-%d-%d %d:%d:%d",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond);

	return currentTime;
}

string Timer::Today()
{
	char currentTime[128] = "";
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(currentTime, "%d년%d월%d일%d시%d분%d_%d초",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return currentTime;
}

void Timer::Update()
{
	m_LastTime = m_NowTime;
	m_NowTime = clock();
}

double Timer::ElipsedSec()
{
	m_NowTime = clock();
	return double(m_NowTime - m_LastTime) / CLOCKS_PER_SEC;
}
