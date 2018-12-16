/******************************************************************************
파일명	: Timer.h
목적		: 시간관련 클래스
******************************************************************************/
#pragma once

namespace ServerEngine
{
	namespace TimerSystem
	{
		//전역 상수
		const time_t TIME_MIN(60);
		const time_t TIME_HOUR(TIME_MIN * 60);
		const time_t TIME_DAY(TIME_HOUR * 24);

		//전역 inline 함수
		inline time_t TIME_TO_MIN(int& x) { return (x / TIME_MIN); }
		inline time_t TIME_TO_MIN(int&& x) { return (x / TIME_MIN); }
		inline time_t MIN_TO_TIME(int& x) { return (x * TIME_MIN); }
		inline time_t MIN_TO_TIME(int&& x) { return (x * TIME_MIN); }
		inline time_t TIME_TO_HOUR(int& x) { return (x / TIME_HOUR); }
		inline time_t TIME_TO_HOUR(int&& x) { return (x / TIME_HOUR); }
		inline time_t HOUR_TO_TIME(int& x) { return (x * TIME_HOUR); }
		inline time_t HOUR_TO_TIME(int&& x) { return (x * TIME_HOUR); }
		inline time_t TIME_TO_DAY(int& x) { return (x / TIME_DAY); }
		inline time_t TIME_TO_DAY(int&& x) { return (x / TIME_DAY); }
		inline time_t DAY_TO_TIME(int& x) { return (x * TIME_DAY); }
		inline time_t DAY_TO_TIME(int&& x) { return (x * TIME_DAY); }

		class Timer : public Singleton<Timer>
		{
			friend Singleton;
		private:
			Timer();
			~Timer();

			chrono::system_clock::time_point m_startTime;

			

			//time_t m_startClock;

			clock_t m_NowTime;
			clock_t m_LastTime;
			tm		m_TM;

			void Init();

		public:
			chrono::system_clock::time_point GetStartTimePoint() const { return m_startTime; }
			chrono::system_clock::time_point GetNowTimePoint() const { return chrono::system_clock::now(); }
			chrono::high_resolution_clock::time_point GetNowHighTimePoint() { return chrono::high_resolution_clock::now(); }
			
			time_t GetStartTime_t() const;
			time_t GetNowTime_t() const;
			const tm&	GetTM();
			wstring		NowTimeWithMilliSec();
			wstring		NowTimeWithSec();
			wstring		Today();
			void		Update();

			chrono::minutes DuraionMin(const chrono::system_clock::time_point& EndTime, const chrono::system_clock::time_point& StartTime);

			double ElipsedSec();
		};
	}
}
