#include "stdafx.h"
#include "ServerNetwork.h"

static CRITICAL_SECTION ClientSessionThreadCS;

ServerNetwork::ServerNetwork()
	:m_Shutdown(false)
{
	InitializeCriticalSection(&ClientSessionThreadCS);
	WSAWINSOCK->Init();
}

ServerNetwork::~ServerNetwork()
{
	m_Shutdown = true;
	DeleteCriticalSection(&ClientSessionThreadCS);
	CloseHandle(m_IOCP);
	closesocket(m_ListenSock);
}

void ServerNetwork::Init()
{
	CreateIOCP();
	CreateListen();	
}

void ServerNetwork::CreateIOCP()
{
	m_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE,
		NULL, 0, 0);
	if (m_IOCP == nullptr)
	{
		ASSERT(0);
		return;
	}
	SLogPrint("IOCP ���� ����!");
	
	CreateIOCPThreads();
}

void ServerNetwork::CreateIOCPThreads()
{
	SYSTEM_INFO SystemInfo;
	memset(&SystemInfo, 0, sizeof(SYSTEM_INFO));
	GetSystemInfo(&SystemInfo);
	for (int i = 0; i < (int)SystemInfo.dwNumberOfProcessors; i++)
	{
		_beginthreadex(NULL, 0,
			CompletionClientSessionThread,
			m_IOCP,
			0, NULL);
	}
	SLogPrint("IOCP ������ ����");
}

void ServerNetwork::CreateAcceptThread()
{
	//acceptRoop ������
	_beginthreadex(NULL, 0,
		AcceptRoop,
		this,
		0, NULL);
}

void ServerNetwork::CreateListen()
{
	m_ListenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN servAddr;
	memset(&servAddr, 0, sizeof(SOCKADDR_IN));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(9000);

	int reUseAddr = 1;
	setsockopt(m_ListenSock, SOL_SOCKET, SO_REUSEADDR, (char *)&reUseAddr, (int)sizeof(reUseAddr));

	bind(m_ListenSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
	listen(m_ListenSock, 5);
	SLogPrintAtFile("Listen ����!");

	CreateAcceptThread();
}

unsigned int ServerNetwork::AcceptRoop(LPVOID sNetwork)
{
	ServerNetwork*	serverNetwork	= (ServerNetwork*)sNetwork;
	SOCKET			clientSock		= INVALID_SOCKET;
	int				addrLen			= sizeof(SOCKADDR_IN);
	DWORD			RecvBytes		= 0;
	SOCKADDR_IN		clientAddr;

	while (!serverNetwork->IsShutdown())
	{
		memset(&clientAddr, 0, addrLen);
		addrLen		= sizeof(SOCKADDR_IN);
		RecvBytes	= 0;

		//clientSock = accept(serverNetwork->GetListenSock(),
		//	(SOCKADDR*)&clientAddr, &addrLen);
		clientSock = WSAAccept(serverNetwork->GetListenSock(),
			(struct sockaddr *)&clientAddr, &addrLen, NULL, 0);
		
		if (clientSock == INVALID_SOCKET)
		{
			DWORD errorCode = WSAGetLastError();
			if (errorCode == WSAEINTR)
			{
				//���Ʈ �ߴܵ�
				return 0;
			}

			SErrPrintAtFile("accept ����, ip = %s, port = %d",
				clientAddr.sin_addr, clientAddr.sin_port);
			return 0;
		}
		char ip[256] = {0,};
		inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
		SLogPrintAtFile("Ŭ���̾�Ʈ ����, ip = %s, port = %d",
			ip, clientAddr.sin_port);

		//Ŭ���̾�Ʈ ���� ���� �� �Ŵ��� �����̳ʿ� �־���
		ClientSession* clientSession = new ClientSession;
		if (!clientSession)
		{
			closesocket(clientSock);
			continue;
		}
		clientSession->SetSocket(clientSock);
		clientSession->SetSocketAddr(clientAddr);
		//���� �ɼ� ����
		if (!clientSession->setSocketOpt())
		{
			closesocket(clientSession->GetSocket());
			SAFE_DELETE(clientSession);
			continue;
		}
		//���ǸŴ����� ���� �߰�
		CLIENTSESSIONMANAGER->AddClientSession(clientSession);

		//IOCP ����
		CreateIoCompletionPort((HANDLE)clientSession->GetSocket(),
			serverNetwork->GetIOCPHandle(), 
			(ULONG_PTR)clientSession, 0);

		//IOCP �����带 ����忡�� ����� ���� recv
		clientSession->RecvStandBy();
	}

	SLogPrintAtFile("Accept���� ������ ����");
	
	OutputDebugStringA("Accept���� ������ ����");

	return 0;
}

unsigned int ServerNetwork::CompletionClientSessionThread(LPVOID pComPort)
{
	HANDLE			completionPort		= (HANDLE)pComPort;
	DWORD			bytesTransferred	= 0;
	ClientSession*	pClientSession		= nullptr;
	IOData*			pIOData				= nullptr;

	while (1)
	{
		BOOL ret = GetQueuedCompletionStatus(completionPort, &bytesTransferred,
			(PULONG_PTR)&pClientSession,
			(LPOVERLAPPED*)&pIOData,
			INFINITE);		

		if (!ret)
		{
			DWORD errorCode = GetLastError();

			//IOCP ������
			if (errorCode == ERROR_ABANDONED_WAIT_0)
			{
				//������ ����
				return 0;
			}

			//������ ���� ����
			if (bytesTransferred == 0)
			{
				//recv, send�� IOData 2���� �����Ե�
				//������ �Ӱ迵�� ����
				EnterCriticalSection(&ClientSessionThreadCS);
				//������ ���� ����
				if (!CLIENTSESSIONMANAGER->
					DeleteClientSession(pClientSession->GetSocket()))
				{
					LeaveCriticalSection(&ClientSessionThreadCS);
					continue;
				}
				//�α��ε� ���� ����
				if (pClientSession->GetPlayerData()->GetPlayerID() != L"")
				{
					if (!CLIENTSESSIONMANAGER->DeleteClientSessionID(pClientSession->GetPlayerData()->GetPlayerID()))
					{
						LeaveCriticalSection(&ClientSessionThreadCS);
						continue;
					}
					DATABASE->InsertUserLogQuery(pClientSession->GetPlayerData()->GetPlayerID(),
						L"������ ���� ����");
				}

				//���� �α�, ������ ���̽� �α׿� ����
				SLogPrintAtFile(L"%s : ������ ���� ����",
					pClientSession->GetPlayerData()->GetPlayerID().c_str());
			
				SAFE_DELETE(pClientSession);
				LeaveCriticalSection(&ClientSessionThreadCS);
			}
			continue;
		}

		if (pClientSession == nullptr)
		{
			SLogPrintAtFile("Ŭ���̾�Ʈ ���� ������");
			return 0;
		}

		//������ ���� ����
		if (bytesTransferred == 0)
		{
			//recv, send�� IOData 2���� �����Ե�
			//������ �Ӱ迵�� ����
			EnterCriticalSection(&ClientSessionThreadCS);
			//������ ���� ����
			if (!CLIENTSESSIONMANAGER->DeleteClientSession(pClientSession->GetSocket()))
			{
				LeaveCriticalSection(&ClientSessionThreadCS);
				continue;
			}
			//�α��ε� ���� ����
			if (pClientSession->GetPlayerData() != nullptr &&
				pClientSession->GetPlayerData()->GetPlayerID() != L"")
			{
				if (!CLIENTSESSIONMANAGER->DeleteClientSessionID(pClientSession->GetPlayerData()->GetPlayerID()))
				{
					LeaveCriticalSection(&ClientSessionThreadCS);
					continue;
				}
				DATABASE->InsertUserLogQuery(pClientSession->GetPlayerData()->GetPlayerID(),
					L"���� ���� ����");
			}
		
			//���� �α�, ������ ���̽� �α׿� ����
			SLogPrintAtFile("%s : ���� ���� ����",
				pClientSession->GetPlayerData()->GetPlayerID().c_str());
			
			SAFE_DELETE(pClientSession);
			LeaveCriticalSection(&ClientSessionThreadCS);
			continue;
		}

		switch (pIOData->GetType())
		{
		case IO_WRITE:
			pClientSession->OnSend((size_t)bytesTransferred);
			continue;

		case IO_READ:
			{
				T_PACKET *pPacket = pClientSession->OnRecv((size_t)bytesTransferred);

				if (pPacket != nullptr)
				{
					if (!pClientSession->PacketParsing(pPacket))
					{
						SLogPrintAtFile("���� ��Ŷ ����");
					}
					SAFE_DELETE(pPacket);
				}
			}
			continue;

		case IO_ERROR:
			SLogPrintAtFile("IO ����");
			continue;
		}
	}

	OutputDebugStringA("IOCP���� ������ ����");

	return 0;
}
