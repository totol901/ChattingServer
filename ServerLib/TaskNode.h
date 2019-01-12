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
			�Լ���	: nextTick
			����		: nextTick_�� ���� ƽ���� ��������
			���ϰ�	: ����
			�Ű�����	: ����
			*****************************************************************************/
			void NextTick();

			/****************************************************************************
			�Լ���	: expired
			����		: �ش� task�� ���� �糪 �ȉ糪 Ȯ��
			���ϰ�	: bool
			�Ű�����	: ����
			*****************************************************************************/
			bool Expired();

			/****************************************************************************
			�Լ���	: tick
			����		: tick���� update�Ǵ� �Լ�
			���ϰ�	: ����
			�Ű�����	: ����
			*****************************************************************************/
			void Tick();
		};
	}
}