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
			//�����尡 ������ �������ö� ���� Ŭ���� �ı���Ű�� �ʰ� ��ٸ�
			WaitForSingleObject(m_hThread, INFINITE);
			CloseHandle(m_Event);

			CloseHandle(m_hThread);
		}

		BOOL WorkThread::Init()
		{
			m_bIsOn = TRUE;
			m_bIsWork = FALSE;

			//������ ����
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
				//�� ����Ŭ�� ������ ����
				WaitForSingleObject(pWorkThread->GetEvent(), INFINITE);

				//���� ���� ����
				if (pWorkThread->GetWork())
				{
					pWorkThread->GetWork()->DoWork();
				}

				//work ���� �� �ִ� ���� ����
				delete(pWorkThread->GetWork());
				pWorkThread->SetWork(nullptr);
				pWorkThread->SetIsWork(FALSE);
				//������ �������� �ٽ� Wait���ÿ� �־���
				THREADPOOLMANAGER->PushWaitStack(pWorkThread);
				//�� ����Ŭ�� �������� ����ġ ����
				//SwitchToThread();
			}

			return 0;
		}

		void WorkThread::HaveThreadWork(Work* work)
		{
			//�̹� �Ҵ�� ���� �ִٸ� ������
			if (!IsWork())
			{
				//work �Ҵ� ����
				SetIsWork(TRUE);
				SetWork(work);
				SignalEvent();
				return;
			}

			SLogPrint("%d: ������ work ���õ�", m_dThreadID);
		}

		void WorkThread::SignalEvent()
		{
			SetEvent(m_Event);
		}
	}
}
