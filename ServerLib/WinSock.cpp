#include "stdafx.h"
#include "WinSock.h"

WinSock::WinSock()
{
	ZeroMemory(&m_WSAData, sizeof(WSADATA));

	if (WSAStartup(MAKEWORD(2, 2), &m_WSAData) != 0)
	{
		cout << "# WSA startup ����!" << endl;
		exit(0);
	}

	cout << "### WSA 2.2 ���� ����!" << endl;
}

WinSock::~WinSock()
{
	WSACleanup();
}

const WSADATA & WinSock::GetWSAData()
{
	return m_WSAData;
}
