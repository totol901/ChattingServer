#pragma once

namespace ServerEngine
{
	namespace System
	{
		const int TICK_INFINTY = 0;

		class TaskNode
		{
		private:
			Work* m_pWorkObj;

			int m_FreqSec;
			int m_DurationSec;

			time_t m_NextTick;
			time_t m_StartTick;

		public:
			TaskNode(Work *workObject, int freqSec, int durationSec);
			~TaskNode();

			/****************************************************************************
			함수명	: nextTick
			설명		: nextTick_을 다음 틱으로 변경해줌
			리턴값	: 없음
			매개변수	: 없음
			*****************************************************************************/
			void NextTick();

			/****************************************************************************
			함수명	: expired
			설명		: 해당 task가 만기 됬나 안됬나 확인
			리턴값	: bool
			매개변수	: 없음
			*****************************************************************************/
			bool Expired();

			/****************************************************************************
			함수명	: tick
			설명		: tick마다 update되는 함수
			리턴값	: 없음
			매개변수	: 없음
			*****************************************************************************/
			void Tick();
		};
	}
}