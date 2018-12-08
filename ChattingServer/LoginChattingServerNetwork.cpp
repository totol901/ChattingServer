#include "stdafx.h"
#include "LoginChattingServerNetwork.h"

static CRITICAL_SECTION ClientSessionThreadCS;

LoginChattingServerNetwork::LoginChattingServerNetwork()
	:ServerNetwork()
{
	InitializeCriticalSection(&ClientSessionThreadCS);
}

LoginChattingServerNetwork::~LoginChattingServerNetwork()
{
	DeleteCriticalSection(&ClientSessionThreadCS);
}

HRESULT LoginChattingServerNetwork::Init()
{
	HRESULT hr = ServerNetwork::Init();

	return hr;
}

void LoginChattingServerNetwork::Release()
{
	ServerNetwork::Release();
}

void LoginChattingServerNetwork::CreateIOCPThreads()
{
	SYSTEM_INFO SystemInfo;
	memset(&SystemInfo, 0, sizeof(SYSTEM_INFO));
	GetSystemInfo(&SystemInfo);
	for (int i = 0; i < (int)SystemInfo.dwNumberOfProcessors; i++)
	{
		HANDLE handle =	(HANDLE)_beginthreadex(NULL, 0,
			CompletionClientSessionThread,
			m_IOCP,
			0, NULL);
		m_vecIOCPThread.push_back(handle);
	}
	SLogPrint("IOCP ������ ����");
}

void LoginChattingServerNetwork::CreateAcceptThread()
{
	//acceptRoop ������
	_beginthreadex(NULL, 0,
		AcceptRoop,
		this,
		0, NULL);
}

unsigned int LoginChattingServerNetwork::AcceptRoop(LPVOID sNetwork)
{
	LoginChattingServerNetwork*	serverNetwork	= (LoginChattingServerNetwork*)sNetwork;
	SOCKET			clientSock		= INVALID_SOCKET;
	int				addrLen			= sizeof(SOCKADDR_IN);
	DWORD			RecvBytes		= 0;
	SOCKADDR_IN		clientAddr;

	while (!serverNetwork->IsShutdown())
	{
		memset(&clientAddr, 0, addrLen);
		addrLen		= sizeof(SOCKADDR_IN);
		RecvBytes	= 0;

		clientSock = WSAAccept(serverNetwork->GetListenSock(),
			(struct sockaddr *)&clientAddr, &addrLen, NULL, 0);
		
		if (clientSock == INVALID_SOCKET)
		{
			DWORD errorCode = WSAGetLastError();
			if (errorCode == WSAEINTR)
			{
				SLogPrintAtFile("Accept���� ������ ����");
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
		LoginChattingClientSession* clientSession = new LoginChattingClientSession;
		clientSession->Init();

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
			clientSession->Release();
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

unsigned int LoginChattingServerNetwork::CompletionClientSessionThread(LPVOID pComPort)
{
	HANDLE			completionPort		= (HANDLE)pComPort;
	DWORD			bytesTransferred	= 0;
	LoginChattingClientSession*	pClientSession		= nullptr;
	IOData* pIOData		= nullptr;

	while (1)
	{
		BOOL ret = GetQueuedCompletionStatus(completionPort, &bytesTransferred,
			(PULONG_PTR)&pClientSession,
			(LPOVERLAPPED*)&pIOData,
			INFINITE);		

		//IOCP ����ó��
		if (!ret)
		{
			DWORD errorCode = GetLastError();

			//IOCP ������
			if (errorCode == ERROR_ABANDONED_WAIT_0)
			{
				OutputDebugStringA("IOCP���� ������ ����");
				//������ ����
				return 0;
			}

			//������ ���� ����
			if (bytesTransferred == 0)
			{
				//������ ���� ����
				if (!LoginChattingServerNetwork::DisconnectClientSession(pClientSession->GetSocket()))
				{
					continue;
				}

				if (pClientSession->GetPlayerData())
				{
					DATABASE->InsertUserLogQuery(pClientSession->GetPlayerData()->GetPlayerID(),
						L"������ ���� ����");

					//���� �α�, ������ ���̽� �α׿� ����
					SLogPrintAtFile(L"%s : ������ ���� ����",
						pClientSession->GetPlayerData()->GetPlayerID().c_str());
				}

				//�α��ε� ���� ����
				LoginChattingServerNetwork::DisconnectLoginClientSession(pClientSession);
			}
			continue;
		}

		//������ ���� ����
		if (bytesTransferred == 0)
		{
			//������ ���� ����
			if (!LoginChattingServerNetwork::DisconnectClientSession(pClientSession->GetSocket()))
			{
				continue;
			}

			if (pClientSession->GetPlayerData())
			{
				DATABASE->InsertUserLogQuery(pClientSession->GetPlayerData()->GetPlayerID(),
					L"���� ���� ����");
				//���� �α�, ������ ���̽� �α׿� ����
				SLogPrintAtFile("%s : ���� ���� ����",
					pClientSession->GetPlayerData()->GetPlayerID().c_str());
			}

			//�α��ε� ���� ����
			LoginChattingServerNetwork::DisconnectLoginClientSession(pClientSession);

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
					//��Ŷ�� �ļ��� ť�� ����ְ�, ������ Ǯ�� �Ľ���
					pClientSession->GetClientSessionParser()->PushQueueRecvPk(pPacket);
					THREADPOOLMANAGER->MakeWork(
						bind(&LoginChattingClientSessionParser::RecvQueuePkParsing,
							pClientSession->GetClientSessionParser())
					);
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

bool LoginChattingServerNetwork::DisconnectClientSession(SOCKET socket)
{
	//recv, send�� IOData 2���� �����Ե�
	//������ �Ӱ迵�� ����
	EnterCriticalSection(&ClientSessionThreadCS);
	
	if (!CLIENTSESSIONMANAGER->DeleteClientSession(socket))
	{
		LeaveCriticalSection(&ClientSessionThreadCS);
		return false;
	}
	LeaveCriticalSection(&ClientSessionThreadCS);
	return true;
}

bool LoginChattingServerNetwork::DisconnectLoginClientSession(LoginChattingClientSession* pClientSession)
{
	EnterCriticalSection(&ClientSessionThreadCS);
	if (pClientSession->GetPlayerData() != nullptr &&
		pClientSession->GetPlayerData()->GetPlayerID() != L"")
	{
		if (!CLIENTSESSIONMANAGER->DeleteClientSessionID(pClientSession->GetPlayerData()->GetPlayerID()))
		{
			LeaveCriticalSection(&ClientSessionThreadCS);
			return false;
		}
	}

	pClientSession->Release();
	SAFE_DELETE(pClientSession);
	LeaveCriticalSection(&ClientSessionThreadCS);

	return true;
}
