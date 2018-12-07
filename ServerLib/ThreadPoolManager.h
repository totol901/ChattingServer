#pragma once

const UINT THREAD_MAX = 100;    //�ִ� ������ ��

class ThreadPoolManager : public Singleton<ThreadPoolManager>
{
	friend Singleton;
private:
	ThreadPoolManager(const ThreadPoolManager&) = delete;
	ThreadPoolManager(const ThreadPoolManager&&) = delete;
	ThreadPoolManager& operator = (const ThreadPoolManager&) = delete;
	ThreadPoolManager& operator = (const ThreadPoolManager&&) = delete;
	
	ThreadPoolManager();
	~ThreadPoolManager();

private:
	HANDLE m_hThread;
	UINT m_dThreadID;

	BOOL m_IsOn;
	UINT m_threadMax;
	vector<WorkThread*> m_vecWorkThreads;	//WorkThread ��ü�� ���� �����̳�
	stack<WorkThread*> m_WorkThreadWaitStack;	//Wait ���°��� �����带 ��� �����̳�
	
	queue<Work*> m_RecvWorkQueue;	//Work�� �޾� �־��ִ� ť
	queue<Work*> m_DoWorkQueue;		//�����尡 Work�� pop�ؼ� ó���ϴ� ť
	
	CRITICAL_SECTION WaitStackCS;
	CRITICAL_SECTION WorkDoQueueCS;
	CRITICAL_SECTION WorkRecvQueueCS;

public:
	HRESULT Init(UINT threadMax = THREAD_MAX);
	void Release();

	static unsigned int WINAPI WorkThreadLoop(LPVOID lpvoid);

	void PushWaitStack(WorkThread* pWorkThread);
	WorkThread* PopWaitStack();

	void PushRecvWorkQueue(Work* work);
	Work* PopDoWorkQueue();

	Work* PopRecvWorkQueue();

	void MakeWork(WORK_FUNCTION_VOID_VOID func);
};
