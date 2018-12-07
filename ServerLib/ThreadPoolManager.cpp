#include "stdafx.h"
#include "ThreadPoolManager.h"

ThreadPoolManager::ThreadPoolManager()
	:m_IsOn(TRUE)
{
	InitializeCriticalSection(&WaitStackCS);
	InitializeCriticalSection(&WorkDoQueueCS);
	InitializeCriticalSection(&WorkRecvQueueCS);
}

ThreadPoolManager::~ThreadPoolManager()
{
	m_IsOn = FALSE;

	//�����尡 ������ �������ö� ���� Ŭ���� �ı���Ű�� �ʰ� ��ٸ�
	WaitForSingleObject(m_hThread, INFINITE);

	//Ȥ�� ���������� �� Work Ŭ���� ���� ���ش�
	while (!m_RecvWorkQueue.empty())
	{
		Work* pTempWork = PopRecvWorkQueue();
		SAFE_DELETE(pTempWork);
	}
	while (!m_DoWorkQueue.empty())
	{
		Work* pTempWork = PopDoWorkQueue();
		SAFE_DELETE(pTempWork);
	}

	DeleteCriticalSection(&WaitStackCS);
	DeleteCriticalSection(&WorkDoQueueCS);
	DeleteCriticalSection(&WorkRecvQueueCS);

	SLogPrint("ThreadPoolManager �Ҹ��� ����");
}

HRESULT ThreadPoolManager::Init(UINT threadMax)
{
	m_threadMax = threadMax;
	m_vecWorkThreads.resize(threadMax);
	
	for (UINT i = 0; i < threadMax; i++)
	{
		WorkThread* temp = new WorkThread;
		if (!temp->Init())
		{
			SLogPrint("workThread init ����");
			return E_FAIL;
		}
		m_vecWorkThreads[i] = temp;
		m_WorkThreadWaitStack.push(temp);
	}

	//������ ����
	m_hThread = (HANDLE)_beginthreadex(NULL, 0,
		WorkThreadLoop,
		this,
		0,
		&m_dThreadID);

	if (!m_hThread)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

void ThreadPoolManager::Release()
{
	for (size_t i = 0; i < m_vecWorkThreads.size(); i++)
	{
		SAFE_DELETE(m_vecWorkThreads[i]);
	}

	SLogPrint("WorkThread ������ ���� �Ϸ�");
}

unsigned int __stdcall ThreadPoolManager::WorkThreadLoop(LPVOID lpvoid)
{
	ThreadPoolManager* pPoolManager = (ThreadPoolManager*)lpvoid;
	
	while (pPoolManager->m_IsOn)
	{
		//�� �̻� �� ���� ������ ����, ������ �����...
		if (!pPoolManager->m_DoWorkQueue.size())
		{
			EnterCriticalSection(&pPoolManager->WorkDoQueueCS);
			EnterCriticalSection(&pPoolManager->WorkRecvQueueCS);
			
			swap(pPoolManager->m_DoWorkQueue, pPoolManager->m_RecvWorkQueue);
			
			LeaveCriticalSection(&pPoolManager->WorkRecvQueueCS);
			LeaveCriticalSection(&pPoolManager->WorkDoQueueCS);
		}
		
		//���� ó��
		Work* ptempWork = pPoolManager->PopDoWorkQueue();
		if (ptempWork)
		{
			WorkThread* ptempWorkThread = pPoolManager->PopWaitStack();
			ptempWorkThread->HaveThreadWork(ptempWork);
		}
	}

	SLogPrint("WorkThreadPool ������ ���� �Ϸ�");

	return 0;
}

void ThreadPoolManager::PushWaitStack(WorkThread * pWorkThread)
{
	EnterCriticalSection(&WaitStackCS);

	m_WorkThreadWaitStack.push(pWorkThread);

	LeaveCriticalSection(&WaitStackCS);
}

WorkThread * ThreadPoolManager::PopWaitStack()
{
	EnterCriticalSection(&WaitStackCS);

	if (m_WorkThreadWaitStack.size() == 0)
	{
		LeaveCriticalSection(&WaitStackCS);
		return nullptr;
	}

	WorkThread * temp = m_WorkThreadWaitStack.top();
	m_WorkThreadWaitStack.pop();

	LeaveCriticalSection(&WaitStackCS);

	return temp;
}

void ThreadPoolManager::PushRecvWorkQueue(Work* work)
{
	EnterCriticalSection(&WorkRecvQueueCS);

	m_RecvWorkQueue.push(work);

	LeaveCriticalSection(&WorkRecvQueueCS);
}

Work* ThreadPoolManager::PopDoWorkQueue()
{
	EnterCriticalSection(&WorkDoQueueCS);
	if (m_DoWorkQueue.size() == 0)
	{
		LeaveCriticalSection(&WorkDoQueueCS);
		return nullptr;
	}

	Work* work = m_DoWorkQueue.front();

	m_DoWorkQueue.pop();

	LeaveCriticalSection(&WorkDoQueueCS);

	return work;
}

Work * ThreadPoolManager::PopRecvWorkQueue()
{
	EnterCriticalSection(&WorkRecvQueueCS);
	if (m_RecvWorkQueue.size() == 0)
	{
		LeaveCriticalSection(&WorkRecvQueueCS);
		return nullptr;
	}

	Work* work = m_RecvWorkQueue.front();

	m_RecvWorkQueue.pop();

	LeaveCriticalSection(&WorkRecvQueueCS);

	return work;
}

void ThreadPoolManager::MakeWork(WORK_FUNCTION_VOID_VOID func)
{
	//���� �Ҵ��� work�� workThread���� work ó���� ���� ���ش�
	Work* work = new Work;
	work->SetCallbackFunction(func);

	PushRecvWorkQueue(work);
}
