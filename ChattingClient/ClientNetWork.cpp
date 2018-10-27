#include "stdafx.h"
#include "ClientNetWork.h"
#include "IOCPServerSession.h"

ClientNetwork::ClientNetwork(char* serverIp, u_short serverPort)
{
	WSAInit();

	ConnectServer(serverIp, serverPort);
}

ClientNetwork::~ClientNetwork()
{
	WSACleanup();
}

void ClientNetwork::WSAInit()
{
	//WSA �ʱ�ȭ
	WSADATA WSAData;
	ZeroMemory(&WSAData, sizeof(WSADATA));

	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		cout << "# WSA startup ����!" << endl;
		exit(0);
	}
	cout << "### WSA 2.2 ���� ����!" << endl;
}

void ClientNetwork::ConnectServer(const char* serverIp, const u_short& serverPort)
{
	//���� connect
	p_mServerSession = new IOCPServerSession;
	p_mServerSession->OnConnect(serverIp, serverPort);

	//IOCP ����
	const DWORD numberOfCurrentThredCount = 1;
	m_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 
		NULL, 0, numberOfCurrentThredCount);
	if (m_IOCP == nullptr)
	{
		cout << "IOCP ���� ����!" << endl;
		return;
	}

	//IOCP�� ���� ������ ����
	for (int i = 0; i < numberOfCurrentThredCount * 2; i++)
	{
		_beginthreadex(NULL, 0, WorkThread, m_IOCP, 0, NULL);
	}

	//ServerSession�� IOCP�� ���� ����
	CreateIoCompletionPort((HANDLE)p_mServerSession->GetSocket(),
		m_IOCP, (ULONG_PTR)p_mServerSession, NULL);

	//recv �񵿱� ����
	p_mServerSession->RecvStandBy();
}

unsigned int ClientNetwork::WorkThread(LPVOID param)
{
	HANDLE hCompletionPort = (HANDLE)param;
	DWORD BytesTransferred = 0;
	IOCPServerSession* pSession;
	IOData* pIOData = nullptr;
	DWORD flags = 0;
	DWORD sendBytes = 0;

	while (1)
	{
		BOOL ret = GetQueuedCompletionStatus(hCompletionPort, &BytesTransferred,
			(PULONG_PTR)&pSession,
			(LPOVERLAPPED*)&pIOData,
			INFINITE);

		if (!ret)
		{
			continue;
		}
		if (pSession == nullptr)
		{
			//SLog(L"! socket data broken");
			return 0;
		}

		if (BytesTransferred == 0)
		{
			cout << "������ ����Ǿ����ϴ�." << endl;
			continue;
		}

		switch (pIOData->GetType())
		{
		case IO_WRITE:
			pSession->OnSend((size_t)BytesTransferred);
			continue;

		case IO_READ:
		{
			string *packet = pSession->OnRecv((size_t)BytesTransferred);
			//cout << pIOData->GetDataBuff() << endl;
			if (packet != nullptr)
			{
				cout << packet->c_str() << endl;
			}
			
		}
		continue;

		case IO_ERROR:

			//SLog(L"* close by client error [%d][%s]", session->id(), session->clientAddress().c_str());
			continue;
		}
	}

	return 0;
}
