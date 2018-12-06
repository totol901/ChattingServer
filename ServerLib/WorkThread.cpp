#include "stdafx.h"
#include "WorkThread.h"

WorkThread::WorkThread()
	:m_hThread(NULL),
	m_dThreadID(0),
	m_bIsWork(FALSE)
{
}

WorkThread::~WorkThread()
{
	CloseHandle(m_hThread);
	m_bIsWork = false;
}

BOOL WorkThread::Init()
{
	//스래드 생성
	m_hThread = (HANDLE)_beginthreadex(NULL, 0,
		WorkRoop,
		this,
		0, &m_dThreadID);

	if (!m_hThread)
	{
		return FALSE;
	}

	return TRUE;
}

unsigned int __stdcall WorkThread::WorkRoop(LPVOID workThread)
{
	WorkThread* pWorkThread = (WorkThread*)workThread;

	while (1)
	{

	}


	return 0;
}
