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
	SLogPrint("IOCP 생성 성공!");
	
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
	SLogPrint("IOCP 스레드 생성");
}

void ServerNetwork::CreateAcceptThread()
{
	//acceptRoop 돌려줌
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
	SLogPrintAtFile("Listen 시작!");

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
				//억셉트 중단됨
				return 0;
			}

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
		if (!clientSession)
		{
			closesocket(clientSock);
			continue;
		}
		clientSession->SetSocket(clientSock);
		clientSession->SetSocketAddr(clientAddr);
		//소켓 옵션 세팅
		if (!clientSession->setSocketOpt())
		{
			closesocket(clientSession->GetSocket());
			SAFE_DELETE(clientSession);
			continue;
		}
		//세션매니저에 세션 추가
		CLIENTSESSIONMANAGER->AddClientSession(clientSession);

		//IOCP 연결
		CreateIoCompletionPort((HANDLE)clientSession->GetSocket(),
			serverNetwork->GetIOCPHandle(), 
			(ULONG_PTR)clientSession, 0);

		//IOCP 스레드를 대기모드에서 깨우기 위한 recv
		clientSession->RecvStandBy();
	}

	SLogPrintAtFile("Accept루프 스레드 종료");
	
	OutputDebugStringA("Accept루프 스레드 종료");

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

			//IOCP 정지됨
			if (errorCode == ERROR_ABANDONED_WAIT_0)
			{
				//스레드 종료
				return 0;
			}

			//비정상 접속 종료
			if (bytesTransferred == 0)
			{
				//recv, send한 IOData 2개가 들어오게됨
				//때문에 임계영역 설정
				EnterCriticalSection(&ClientSessionThreadCS);
				//접속한 세션 제거
				if (!CLIENTSESSIONMANAGER->
					DeleteClientSession(pClientSession->GetSocket()))
				{
					LeaveCriticalSection(&ClientSessionThreadCS);
					continue;
				}
				//로그인된 세션 제거
				if (pClientSession->GetPlayerData()->GetPlayerID() != L"")
				{
					if (!CLIENTSESSIONMANAGER->DeleteClientSessionID(pClientSession->GetPlayerData()->GetPlayerID()))
					{
						LeaveCriticalSection(&ClientSessionThreadCS);
						continue;
					}
					DATABASE->InsertUserLogQuery(pClientSession->GetPlayerData()->GetPlayerID(),
						L"비정상 접속 종료");
				}

				//파일 로그, 데이터 베이스 로그에 남김
				SLogPrintAtFile(L"%s : 비정상 접속 종료",
					pClientSession->GetPlayerData()->GetPlayerID().c_str());
			
				SAFE_DELETE(pClientSession);
				LeaveCriticalSection(&ClientSessionThreadCS);
			}
			continue;
		}

		if (pClientSession == nullptr)
		{
			SLogPrintAtFile("클라이언트 세션 삭제됨");
			return 0;
		}

		//정상적 접속 종료
		if (bytesTransferred == 0)
		{
			//recv, send한 IOData 2개가 들어오게됨
			//때문에 임계영역 설정
			EnterCriticalSection(&ClientSessionThreadCS);
			//접속한 세션 제거
			if (!CLIENTSESSIONMANAGER->DeleteClientSession(pClientSession->GetSocket()))
			{
				LeaveCriticalSection(&ClientSessionThreadCS);
				continue;
			}
			//로그인된 세션 제거
			if (pClientSession->GetPlayerData() != nullptr &&
				pClientSession->GetPlayerData()->GetPlayerID() != L"")
			{
				if (!CLIENTSESSIONMANAGER->DeleteClientSessionID(pClientSession->GetPlayerData()->GetPlayerID()))
				{
					LeaveCriticalSection(&ClientSessionThreadCS);
					continue;
				}
				DATABASE->InsertUserLogQuery(pClientSession->GetPlayerData()->GetPlayerID(),
					L"정상 접속 종료");
			}
		
			//파일 로그, 데이터 베이스 로그에 남김
			SLogPrintAtFile("%s : 정상 접속 종료",
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
						SLogPrintAtFile("종료 패킷 받음");
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

	OutputDebugStringA("IOCP루프 스레드 종료");

	return 0;
}
