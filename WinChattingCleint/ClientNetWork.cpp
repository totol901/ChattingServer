#include "stdafx.h"
#include "ClientNetWork.h"
#include "IOCPServerSession.h"

ClientNetwork::ClientNetwork()
{
}

ClientNetwork::~ClientNetwork()
{
	CloseHandle(m_IOCP);
}

void ClientNetwork::Init(char * serverIp, u_short serverPort)
{
	WSAInit();

	ConnectServer(serverIp, serverPort);
}

void ClientNetwork::WSAInit()
{
	WSAWINSOCK->Init();
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
		WSAERROR->err_print(TEXT("IOCP ���� ����!"));
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

void ClientNetwork::SetLinkIsOn(bool * ison)
{
	isOn = ison;
	p_mServerSession->SetLinkIsOn(ison);
}

unsigned int ClientNetwork::WorkThread(LPVOID param)
{
	HANDLE hCompletionPort = (HANDLE)param;
	DWORD BytesTransferred = 0;
	IOCPServerSession* pSession;
	IOData* pIOData = nullptr;

	while (1)
	{
		BOOL ret = GetQueuedCompletionStatus(hCompletionPort, &BytesTransferred,
			(PULONG_PTR)&pSession,
			(LPOVERLAPPED*)&pIOData,
			INFINITE);

		//���� ó�� �� ���� ó��
		if (!ret)
		{
			if (BytesTransferred == 0)
			{
				WSAERROR->err_print(TEXT("IOCP ���� ����!"));
				return 0;
			}
			WSAERROR->err_print(TEXT("IOCP ���� ����!"));
			return 0;
		}
		if (pSession == nullptr)
		{
			WSAERROR->err_print(TEXT("IOCP ���� ����!"));
			return 0;
		}
		if (BytesTransferred == 0)
		{
			WSAERROR->err_print(TEXT("������ ���� ���� ����.\n"));
			return 0;
		}

		//��Ŷ�� Ÿ�Կ� ���� Send, �Ľ�, ����ó�� ����
		switch (pIOData->GetType())
		{
		case IO_WRITE:
			pSession->OnSend((size_t)BytesTransferred);
			continue;

		case IO_READ:
			{
				T_PACKET *packet = pSession->OnRecv((size_t)BytesTransferred);

				if (packet != nullptr)
				{
					if(!pSession->PacketParsing(packet))
					{
						SAFE_DELETE(packet);
						//���� ��Ŷ�� ����
						return 0;
					}
					SAFE_DELETE(packet);
				}
			}
			continue;

		case IO_ERROR:
			WSAERROR->err_print(TEXT("IO ����\n"));
			ASSERT(false);
			continue;
		}
	}

	return 0;
}
