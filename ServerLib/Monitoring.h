/******************************************************************************
���ϸ�	: Monitoring.h
����		: CPU, �޸� ��뷮 üũ
******************************************************************************/
#pragma once

class Monitoring : public Singleton<Monitoring>
{
	friend Singleton;
private:
	ULARGE_INTEGER m_LastCPU, m_LastSysCPU, m_LastUserCPU;
	int		m_NumProcessors;
	HANDLE	m_Self;

	Monitoring()
	{
		SYSTEM_INFO sysInfo;
		FILETIME	ftime, fsys, fuser;

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
	�Լ���	: processCpuUsage
	����		: ���μ����� ������� CPU %
	*****************************************************************************/
	double processCpuUsage()
	{
		ULARGE_INTEGER	now, sys, user;
		FILETIME		ftime, fsys, fuser;
		double			percent;

		GetSystemTimeAsFileTime(&ftime);//���� �ð�
		memcpy(&now, &ftime, sizeof(FILETIME));

		GetProcessTimes(m_Self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&sys, &fsys, sizeof(FILETIME));	//Ŀ�θ�忡�� ��� �ð�
		memcpy(&user, &fuser, sizeof(FILETIME));//������忡�� ��� �ð�
		percent = (double)((sys.QuadPart - m_LastSysCPU.QuadPart) +
			(user.QuadPart - m_LastUserCPU.QuadPart));
		percent /= (double)(now.QuadPart - m_LastCPU.QuadPart);
		percent /= (double)m_NumProcessors;
		percent = percent * 100.0;

		return fixInRange<double>(0.0, percent, 100.0);
	}

	/****************************************************************************
	�Լ���	: processMemUsage
	����		: ���� ���μ����� �����ִ� �޸�
	*****************************************************************************/
	SIZE_T processMemUsage()
	{
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
		
		return (size_t)pmc.WorkingSetSize;
	}

	/****************************************************************************
	�Լ���	: physyicMemUsage
	����		: ���� ���� �ִ� ��ü �޸�
	*****************************************************************************/
	SIZE_T physyicMemUsage()
	{
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);

		return (size_t)memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	}
};
