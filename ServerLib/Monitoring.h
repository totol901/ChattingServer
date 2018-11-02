/******************************************************************************
파일명	: Monitoring.h
목적		: CPU, 메모리 사용량 체크
사용방식	: TaskManager에 work로 Monitering의 함수를 스레드로 돌려주면 됨
******************************************************************************/
#pragma once

class Monitoring : public Singleton<Monitoring>
{
	friend Singleton;
private:
	ULARGE_INTEGER m_LastCPU, m_LastSysCPU, m_LastUserCPU;
	int m_NumProcessors;
	HANDLE m_Self;

	Monitoring()
	{
		SYSTEM_INFO sysInfo;
		FILETIME ftime, fsys, fuser;

		GetSystemInfo(&sysInfo);
		m_NumProcessors = sysInfo.dwNumberOfProcessors;

		GetSystemTimeAsFileTime(&ftime);
		memcpy(&m_LastCPU, &ftime, sizeof(FILETIME));

		m_Self = GetCurrentProcess();
		GetProcessTimes(m_Self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&m_LastSysCPU, &fsys, sizeof(FILETIME));
		memcpy(&m_LastUserCPU, &fuser, sizeof(FILETIME));
	}
	~Monitoring() {}

public:
	/****************************************************************************
	함수명	: processCpuUsage
	설명		: 프로세스가 사용중인 CPU %
	리턴값	: double
	매개변수	: 없음
	*****************************************************************************/
	double processCpuUsage()
	{
		FILETIME ftime, fsys, fuser;
		ULARGE_INTEGER now, sys, user;
		double percent;

		GetSystemTimeAsFileTime(&ftime);//현재 시간
		memcpy(&now, &ftime, sizeof(FILETIME));

		GetProcessTimes(m_Self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&sys, &fsys, sizeof(FILETIME));	//커널모드에서 사용 시간
		memcpy(&user, &fuser, sizeof(FILETIME));//유저모드에서 사용 시간
		percent = (double)((sys.QuadPart - m_LastSysCPU.QuadPart) +
			(user.QuadPart - m_LastUserCPU.QuadPart));
		percent /= (now.QuadPart - m_LastCPU.QuadPart);
		percent /= m_NumProcessors;
		percent = percent * 100;

		return fixInRange<double>(0, percent, 100);
	}

	/****************************************************************************
	함수명	: processMemUsage
	설명		: 현재 프로세스가 쓰고있는 메모리
	리턴값	: SIZE_T
	매개변수	: 없음
	*****************************************************************************/
	SIZE_T processMemUsage()
	{
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));

		return (size_t)pmc.WorkingSetSize;
	}

	/****************************************************************************
	함수명	: physyicMemUsage
	설명		: 지금 쓰고 있는 전체 메모리
	리턴값	: SIZE_T
	매개변수	: 없음
	*****************************************************************************/
	SIZE_T physyicMemUsage()
	{
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);

		return (size_t)memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	}
};
