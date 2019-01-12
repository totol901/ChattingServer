#include "stdafx.h"
#include "TaskManager.h"

namespace ServerEngine
{
	namespace System
	{
		TaskManager::TaskManager()
		{
		}

		TaskManager::~TaskManager()
		{
		}

		HRESULT TaskManager::Init()
		{
			tinyxml2::XMLDocument config;

			if (!Util::loadConfig(&config))
			{
				return E_FAIL;
			}
			
			tinyxml2::XMLElement* root = config.FirstChildElement("App")->FirstChildElement("Task");
			if (!root)
			{
				SLogPrintAtFile(L"@ not exist task setting");
				return E_FAIL;
			}

			tinyxml2::XMLElement* elem = root->FirstChildElement("ThreadCount");
			sscanf_s(elem->GetText(), "%d", &m_ThreadCount);

			for (int i = 0; i < m_ThreadCount; ++i)
			{
				Task *task = new Task(i);
				m_vecTaskPool.push_back(task);
				task->Run();
			}
			SLogPrintAtFile(L"* task thread, [%d] maked", m_ThreadCount);

			return S_OK;
		}

		void TaskManager::Release()
		{
			for (auto task : m_vecTaskPool)
			{
				SAFE_DELETE(task);
			}
		}

		void TaskManager::Add(Work * workObject, int freqSec, int durationSec)
		{
			const int MINIMAL_THREAD_COUNT = 1;
			if (m_ThreadCount < MINIMAL_THREAD_COUNT)
			{
				return;
			}
			static int nodeCount = 0;

			TaskNode *node = new TaskNode(workObject, freqSec, durationSec);
			int index = nodeCount % m_ThreadCount;
			Task *task = m_vecTaskPool[index];
			task->Add(node);

			++nodeCount;
		}
	}
}