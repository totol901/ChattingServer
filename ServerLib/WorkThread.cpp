#include "stdafx.h"
#include "WorkThread.h"

namespace ServerEngine
{
	namespace System
	{
		WorkThread::WorkThread()
			:m_hThread(NULL),
			m_dThreadID(0),
			m_bIsWork(FALSE),
			m_bIsOn(FALSE),
			m_pWork(nullptr)
		{
			m_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
		}

		WorkThread::~WorkThread()
		{
			m_bIsOn = FALSE;

			SignalEvent();
			//스레드가 루프를 빠져나올때 까지 클래스 파괴시키지 않게 기다림
			WaitForSingleObject(m_hThread, INFINITE);
			CloseHandle(m_Event);

			CloseHandle(m_hThread);
		}

		BOOL WorkThread::Init()
		{
			m_bIsOn = TRUE;
			m_bIsWork = FALSE;

			//스래드 생성
			m_hThread = (HANDLE)_beginthreadex(NULL, 0,
				WorkRoop,
				this,
				0,
				&m_dThreadID);

			if (!m_hThread)
			{
				return FALSE;
			}

			return TRUE;
		}

		unsigned int __stdcall WorkThread::WorkRoop(LPVOID workThread)
		{
			WorkThread* pWorkThread = (WorkThread*)workThread;

			while (pWorkThread->IsOn())
			{
				//한 사이클이 끝나면 정지
				WaitForSingleObject(pWorkThread->GetEvent(), INFINITE);

				//들어온 일을 해줌
				if (pWorkThread->GetWork())
				{
					pWorkThread->GetWork()->DoWork();
				}

				//work 받을 수 있는 상태 만듬
				delete(pWorkThread->GetWork());
				pWorkThread->SetWork(nullptr);
				pWorkThread->SetIsWork(FALSE);
				//할일이 끝났으니 다시 Wait스택에 넣어줌
				THREADPOOLMANAGER->PushWaitStack(pWorkThread);
				//한 사이클이 끝났으니 스위치 해줌
				//SwitchToThread();
			}

			return 0;
		}

		void WorkThread::HaveThreadWork(Work* work)
		{
			//이미 할당된 일이 있다면 무시함
			if (!IsWork())
			{
				//work 할당 받음
				SetIsWork(TRUE);
				SetWork(work);
				SignalEvent();
				return;
			}

			SLogPrint("%d: 스레드 work 무시됨", m_dThreadID);
		}

		void WorkThread::SignalEvent()
		{
			SetEvent(m_Event);
		}
	}
}
