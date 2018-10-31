#include "stdafx.h"
#include "WinSock.h"

WinSock::WinSock()
{
	memset(&m_WSAData, 0, sizeof(WSADATA));

	if (WSAStartup(MAKEWORD(2, 2), &m_WSAData) != 0)
	{
		cout << "# WSA startup 실패!" << endl;
		exit(0);
	}

	cout << "### WSA 2.2 세팅 성공!" << endl;
}

WinSock::~WinSock()
{
	WSACleanup();
}

const WSADATA & WinSock::GetWSAData()
{
	return m_WSAData;
}
