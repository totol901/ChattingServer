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
			함수명	: add
			설명		: workObject를 taskPool_에 추가해줌
			리턴값	: 없음
			매개변수	: Work*, int, int
			*****************************************************************************/
			void Add(Work *workObject, int freqSec, int durationSec);
		};
	}
}
