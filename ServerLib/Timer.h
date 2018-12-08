/******************************************************************************
���ϸ�	: Timer.h
����		: �ð����� Ŭ����
******************************************************************************/
#pragma once

namespace ServerEngine
{
	namespace TimerSystem
	{
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
			wstring		NowTimeWithMilliSec();
			wstring		NowTimeWithSec();
			wstring		Today();
			void		Update();

			double ElipsedSec();
		};
	}
}
