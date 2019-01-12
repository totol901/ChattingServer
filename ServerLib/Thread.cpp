#include "stdafx.h"
#include "Thread.h"

namespace ServerEngine
{
	namespace System
	{
		//Thread::Thread(_beginthreadex_proc_type func)
		//	:m_CurrentLock(nullptr)
		//{
		//	m_hThread = (HANDLE)_beginthreadex(
		//		NULL, 
		//		0,
		//		func,
		//		NULL,
		//		0,
		//		&m_dThreadID);
		//
		//	ThreadManager::GetInstance()->Push(this);
		//}

		Thread::Thread(void* arg, _beginthreadex_proc_type func)
		{
			m_hThread = (HANDLE)_beginthreadex(
				NULL, 
				0,
				func,
				arg,
				0,
				&m_dThreadID);

			ThreadManager::GetInstance()->Push(this);
		}

		Thread::~Thread()
		{
			WaitForSingleObject(m_hThread, INFINITE);
			CloseHandle(m_hThread);

			ThreadManager::GetInstance()->Remove(this->GetThreadID());
		}
	}
}
