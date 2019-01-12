#pragma once

namespace ServerEngine
{
	namespace System
	{
		class TaskManager : public Singleton <TaskManager>
		{
			friend TaskManager;
		private:
			int m_ThreadCount;
			vector<Task *> m_vecTaskPool;

			TaskManager();
			~TaskManager();
			TaskManager(const TaskManager&) = delete;
			TaskManager(const TaskManager&&) = delete;
			TaskManager& operator = (const TaskManager&) = delete;
			TaskManager& operator = (const TaskManager&&) = delete;

		public:
			HRESULT Init();
			void Release();

			/****************************************************************************
			�Լ���	: add
			����		: workObject�� taskPool_�� �߰�����
			���ϰ�	: ����
			�Ű�����	: Work*, int, int
			*****************************************************************************/
			void Add(Work *workObject, int freqSec, int durationSec);
		};
	}
}
