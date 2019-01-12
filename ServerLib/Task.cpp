#include "stdafx.h"
#include "Task.h"

namespace ServerEngine
{
	namespace System
	{
		Task::Task(int ID)
			:m_ID(ID),
			m_pThread(nullptr)
		{
		}

		Task::~Task()
		{
			SAFE_DELETE(m_pThread);

			for (auto node : m_TaskList)
			{
				SAFE_DELETE(node);
			}
		}

		void Task::Add(TaskNode * taskNode)
		{
			m_TaskList.push_back(taskNode);
		}

		void Task::Remove(TaskNode * taskNode)
		{
			m_TaskList.remove(taskNode);
			SAFE_DELETE(taskNode);
		}

		unsigned int WINAPI Task::Process(LPVOID parm)
		{
			Task* temp = (Task*)parm;
			while (!Util::GlobalValue::GetInstance()->m_IsShutdown)
			{
				vector<TaskNode *> deleteNodes;
				for (auto taskNode : temp->m_TaskList)
				{
					if (taskNode->Expired())
					{
						deleteNodes.push_back(taskNode);
						continue;
					}
					taskNode->Tick();
				}

				for (auto node : deleteNodes)
				{
					temp->Remove(node);
				}
				CONTEXT_SWITCH;
			}

			return 0;
		}

		void Task::Run()
		{
			if (!m_pThread)
			{
				m_pThread = new Thread(this, Process);
			}
		}
	}
}