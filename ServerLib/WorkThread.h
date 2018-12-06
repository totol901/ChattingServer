#pragma once

#define WORK_MAX    10000   //최대 작업 수
#define THREAD_MAX  100     //최대 쓰레드 수

typedef function<void(void)> CALLBACK_FUNCTION_VOID_VOID;

class WorkThread
{
private:
	HANDLE m_hThread;
	UINT m_dThreadID;
	BOOL m_bIsWork;

public:
	WorkThread();
	~WorkThread();

	BOOL Init();

	static unsigned int WINAPI WorkRoop(LPVOID workThread);

	
};

