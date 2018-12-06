#pragma once

#define WORK_MAX    10000   //�ִ� �۾� ��
#define THREAD_MAX  100     //�ִ� ������ ��

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

