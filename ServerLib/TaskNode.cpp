#include "stdafx.h"
#include "TaskNode.h"

namespace ServerEngine
{
	namespace System
	{
		TaskNode::TaskNode(Work *workObject, int freqSec, int durationSec)
			:m_pWorkObj(workObject),
			m_FreqSec(freqSec),
			m_DurationSec(durationSec),
			m_StartTick(TIMER->GetNowTime_t())
		{
			this->NextTick();
		}

		TaskNode::~TaskNode()
		{
			SAFE_DELETE(m_pWorkObj);
		}

		void TaskNode::NextTick()
		{
			m_NextTick = TIMER->GetNowTime_t() + m_FreqSec;
		}

		bool TaskNode::Expired()
		{
			if (m_pWorkObj == nullptr)
			{
				return true;
			}

			if (m_DurationSec != TICK_INFINTY)
			{
				if (m_DurationSec < TIMER->GetNowTime_t() - m_StartTick)
				{
					return true;
				}
			}

			return false;
		}

		void TaskNode::Tick()
		{
			if (m_NextTick < TIMER->GetNowTime_t())
			{
				m_pWorkObj->Tick();
				this->NextTick();
			}
		}
	}
}