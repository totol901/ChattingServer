#include "stdafx.h"
#include "ServerNetwork.h"

ServerNetwork::ServerNetwork()
{
	WSAWINSOCK->Init();
}

ServerNetwork::~ServerNetwork()
{
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
		cout << "IOCP 생성 실패!" << endl;
		return;
	}

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
}

void ServerNetwork::CreateListen()
{
	m_ListenSock = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN servAddr;
	memset(&servAddr, 0, sizeof(SOCKADDR_IN));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(9000);

	bind(m_ListenSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
	listen(m_ListenSock, 5);

	//acceptRoop 돌려줌
	_beginthreadex(NULL, 0,
		AcceptRoop,
		this,
		0, NULL);
}

unsigned int ServerNetwork::AcceptRoop(LPVOID sNetwork)
{
	ServerNetwork* serverNetwork = (ServerNetwork*)sNetwork;
	SOCKADDR_IN clientAddr;
	SOCKET clientSock = INVALID_SOCKET;
	int addrLen = sizeof(clientAddr);
	DWORD RecvBytes = 0;

	while (true)
	{
		addrLen = sizeof(SOCKADDR_IN);
		memset(&clientAddr, 0, addrLen);
		RecvBytes = 0;

		clientSock = accept(serverNetwork->GetListenSock(),
			(SOCKADDR*)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET)
		{
			return 0;
		}

		//클라이언트 세션 생성 후 매니저 컨터이너에 넣어줌
		ClientSession* clientSession = new ClientSession;
		clientSession->SetSocket(clientSock);
		clientSession->SetSocketAddr(clientAddr);
		
		cout << "클라접속!" << endl;

		if (!CLIENTSESSIONMANAGER->AddClientSession(clientSession))
		{
			//TODO : Log 처리, 클라이언트 세션 중복
		}

		//IOCP 연결
		CreateIoCompletionPort((HANDLE)clientSession->GetSocket(),
			serverNetwork->GetIOCPHandle(), 
			(ULONG_PTR)clientSession, 0);

		//IOCP 스레드를 대기모드에서 깨우기 위한 recv
		clientSession->RecvStandBy();
	}

	return 0;
}

unsigned int ServerNetwork::CompletionClientSessionThread(LPVOID pComPort)
{
	HANDLE completionPort = (HANDLE)pComPort;
	DWORD bytesTransferred = 0;
	ClientSession* pClientSession = nullptr;
	IOData* pIOData = nullptr;

	while (1)
	{
		BOOL ret = GetQueuedCompletionStatus(completionPort, &bytesTransferred,
			(PULONG_PTR)&pClientSession,
			(LPOVERLAPPED*)&pIOData,
			INFINITE);

		if (!ret)
		{
			//비정상 접속 종료
			if (bytesTransferred == 0)
			{
				if (!CLIENTSESSIONMANAGER->
					DeleteClientSession(pClientSession->GetSocket()))
				{
					//해당 클라이언트 세션 못찾음
				}
				SAFE_DELETE(pClientSession);
			}
			continue;
		}
		if (pClientSession == nullptr)
		{
			//SLog(L"! socket data broken");
			return 0;
		}

		//정상적 접속 종료
		if (bytesTransferred == 0)
		{
			if (!CLIENTSESSIONMANAGER->DeleteClientSession(pClientSession->GetSocket()))
			{
				// TODO : 매니저에서 찾을수가 없음 log 띄움
			}
			SAFE_DELETE(pClientSession);
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
						//문제가 생김 접속 종료됨
					}
					SAFE_DELETE(pPacket);
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
