#pragma once

const UINT THREAD_MAX = 100;    //최대 쓰레드 수

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
	vector<WorkThread*> m_vecWorkThreads;	//WorkThread 전체를 담은 컨테이너
	stack<WorkThread*> m_WorkThreadWaitStack;	//Wait 상태가된 스래드를 담는 컨테이너
	
	queue<Work*> m_RecvWorkQueue;	//Work를 받아 넣어주는 큐
	queue<Work*> m_DoWorkQueue;		//스레드가 Work를 pop해서 처리하는 큐
	
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
