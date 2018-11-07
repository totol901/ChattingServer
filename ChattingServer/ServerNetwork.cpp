#include "stdafx.h"
#include "ServerNetwork.h"

static CRITICAL_SECTION ClientSessionThreadCS;

ServerNetwork::ServerNetwork()
	:m_Shutdown(false)
{
	WSAWINSOCK->Init();
}

ServerNetwork::~ServerNetwork()
{
	m_Shutdown = true;
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
	SLogPrint("IOCP 생성 성공!");
	
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
	SLogPrint("IOCP 스레드 생성");
}

void ServerNetwork::CreateListen()
{
	m_ListenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN servAddr;
	memset(&servAddr, 0, sizeof(SOCKADDR_IN));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(9000);

	bind(m_ListenSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
	listen(m_ListenSock, 5);
	SLogPrintAtFile("Listen 시작!");

	//acceptRoop 돌려줌
	_beginthreadex(NULL, 0,
		AcceptRoop,
		this,
		0, NULL);
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

		clientSock = accept(serverNetwork->GetListenSock(),
			(SOCKADDR*)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET)
		{
			SErrPrintAtFile("accept 실패, ip = %s, port = %d",
				clientAddr.sin_addr, clientAddr.sin_port);
			return 0;
		}
		char ip[256] = {0,};
		inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
		SLogPrintAtFile("클라이언트 접속, ip = %s, port = %d",
			ip, clientAddr.sin_port);

		//클라이언트 세션 생성 후 매니저 컨터이너에 넣어줌
		ClientSession* clientSession = new ClientSession;
		clientSession->SetSocket(clientSock);
		clientSession->SetSocketAddr(clientAddr);
		//세션매니저에 세션 추가
		CLIENTSESSIONMANAGER->AddClientSession(clientSession);

		//IOCP 연결
		CreateIoCompletionPort((HANDLE)clientSession->GetSocket(),
			serverNetwork->GetIOCPHandle(), 
			(ULONG_PTR)clientSession, 0);
		
		//과제를 위한 추가 구문
		char buf[PAKCET_BUFF_SIZE] = { 0, };
		memcpy(buf, TIMER->NowTimeWithMilliSec().c_str(), 
			TIMER->NowTimeWithMilliSec().size());
		T_PACKET packet(PK_NONE);
		packet.Size = sizeof(T_PACKET);
		memcpy_s(packet.buff, (PAKCET_BUFF_SIZE),
			buf, strlen(buf) + 1);
		clientSession->SendPacket(packet);

		//IOCP 스레드를 대기모드에서 깨우기 위한 recv
		clientSession->RecvStandBy();
	}

	SLogPrintAtFile("Accept루프 스레드 종료");

	return 0;
}

unsigned int ServerNetwork::CompletionClientSessionThread(LPVOID pComPort)
{
	HANDLE			completionPort		= (HANDLE)pComPort;
	DWORD			bytesTransferred	= 0;
	ClientSession*	pClientSession		= nullptr;
	IOData*			pIOData				= nullptr;

	InitializeCriticalSection(&ClientSessionThreadCS);

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
				//recv, send한 IOData 2개가 들어오게됨
				//때문에 임계영역 설정
				EnterCriticalSection(&ClientSessionThreadCS);
				if (!CLIENTSESSIONMANAGER->
					DeleteClientSession(pClientSession->GetSocket()))
				{
					LeaveCriticalSection(&ClientSessionThreadCS);
					continue;
				}
				SAFE_DELETE(pClientSession);
				LeaveCriticalSection(&ClientSessionThreadCS);
			}
			continue;
		}
		if (pClientSession == nullptr)
		{
			SLogPrintAtFile("소켓 이상");
			return 0;
		}

		//정상적 접속 종료
		if (bytesTransferred == 0)
		{
			//recv, send한 IOData 2개가 들어오게됨
			//때문에 임계영역 설정
			EnterCriticalSection(&ClientSessionThreadCS);
			if (!CLIENTSESSIONMANAGER->DeleteClientSession(pClientSession->GetSocket()))
			{
				LeaveCriticalSection(&ClientSessionThreadCS);
				continue;
			}
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
						SLogPrintAtFile("페킷 파싱중 문제가 생김");
					}
					SAFE_DELETE(pPacket);
				}
			}
			continue;

		case IO_ERROR:
			SLogPrintAtFile("IO 에러");
			continue;
		}
	}

	DeleteCriticalSection(&ClientSessionThreadCS);

	return 0;
}
