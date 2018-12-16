#include "stdafx.h"
#include "Timer.h"

namespace ServerEngine
{
	namespace TimerSystem
	{
		Timer::Timer()
		{
			Init();
		}

		Timer::~Timer()
		{
		}

		void Timer::Init()
		{
			m_startTime = chrono::system_clock::now();
			//m_startClock = chrono::system_clock::to_time_t(chrono::system_clock::now());

			m_NowTime = clock();
			m_LastTime = clock();
		}

		time_t Timer::GetStartTime_t() const
		{
			return chrono::system_clock::to_time_t(m_startTime);
		}

		time_t Timer::GetNowTime_t() const
		{
			return chrono::system_clock::to_time_t(chrono::system_clock::now());
		}

		const tm & Timer::GetTM()
		{
			time_t m_NowTime = time(NULL);
			localtime_s(&m_TM, &m_NowTime);

			return m_TM;
		}

		wstring Timer::NowTimeWithMilliSec()
		{
			TCHAR currentTime[128] = L"";
			SYSTEMTIME st;
			GetLocalTime(&st);
			swprintf_s(currentTime, L"%d-%d-%d %d:%d:%d.%d",
				st.wYear, st.wMonth, st.wDay,
				st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

			return currentTime;
		}

		wstring Timer::NowTimeWithSec()
		{
			TCHAR currentTime[128] = L"";
			SYSTEMTIME st;
			GetLocalTime(&st);
			swprintf_s(currentTime, L"%d-%d-%d %d:%d:%d",
				st.wYear, st.wMonth, st.wDay,
				st.wHour, st.wMinute, st.wSecond);

			return currentTime;
		}

		wstring Timer::Today()
		{
			TCHAR currentTime[128] = L"";
			SYSTEMTIME st;
			GetLocalTime(&st);
			swprintf_s(currentTime, L"%d년%d월%d일%d시%d분%d_%d초",
				st.wYear, st.wMonth, st.wDay,
				st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

			return currentTime;
		}

		void Timer::Update()
		{
			m_LastTime = m_NowTime;
			m_NowTime = clock();
		}

		chrono::minutes Timer::DuraionMin(const chrono::system_clock::time_point & EndTime, const chrono::system_clock::time_point & StartTime)
		{
			return chrono::duration_cast<chrono::minutes>(EndTime- StartTime);
		}

		double Timer::ElipsedSec()
		{
			m_NowTime = clock();
			return double(m_NowTime - m_LastTime) / CLOCKS_PER_SEC;
		}
	}
}
