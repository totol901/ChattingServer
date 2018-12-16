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
	SLogPrint("IOCP 스레드 생성");
}

void LoginChattingServerNetwork::CreateAcceptThread()
{
	//acceptRoop 돌려줌
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
				SLogPrintAtFile("Accept루프 스레드 종료");
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
		LoginChattingClientSession* clientSession = new LoginChattingClientSession;
		clientSession->Init();

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
			clientSession->Release();
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

		//IOCP 에러처리
		if (!ret)
		{
			DWORD errorCode = GetLastError();
			
			//IOCP 정지됨
			if (errorCode == ERROR_ABANDONED_WAIT_0)
			{
				OutputDebugStringA("IOCP루프 스레드 종료");
				//스레드 종료
				return 0;
			}

			(LoginChattingServerNetwork::IsDeleteSession(pClientSession, bytesTransferred));
			continue;
		}

		//정상적 접속 종료
		if (!LoginChattingServerNetwork::IsDeleteSessionCommon(pClientSession, bytesTransferred))
		{
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
					//패킷을 파서의 큐에 담아주고, 스레드 풀로 파싱함
					pClientSession->GetClientSessionParser()->PushQueueRecvPk(pPacket);
					THREADPOOLMANAGER->MakeWork(
						bind(&LoginChattingClientSessionParser::RecvQueuePkParsing,
							pClientSession->GetClientSessionParser()
						)
					);
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

bool LoginChattingServerNetwork::IsDeleteSession(LoginChattingClientSession* pClientSession, DWORD bytesTransferred)
{
	EnterCriticalSection(&ClientSessionThreadCS);

	//하트비트에 문제 있다면, 클라에서 갑작스럽게 소켓 종료했을때 비정상 접속 종료
	if (bytesTransferred == 0 || !pClientSession->ChackHeartBeat())
	{
		//접속한 세션 제거
		if (!LoginChattingServerNetwork::DisconnectClientSession(pClientSession->GetSocket()))
		{
			return false;
		}

		if (pClientSession->GetPlayerData())
		{
			DATABASE->InsertUserLogQuery(pClientSession->GetPlayerData()->GetPlayerID(),
				L"비정상 접속 종료");

			//파일 로그, 데이터 베이스 로그에 남김
			SLogPrintAtFile(L"%s : 비정상 접속 종료",
				pClientSession->GetPlayerData()->GetPlayerID().c_str());

			if (pClientSession->GetPlayerData()->GetChannel())
			{
				pClientSession->GetClientSessionParser()->SendChannelOutAnnounce();
				
				pClientSession->GetPlayerData()->GetChannel()->DeleteClientSession(pClientSession);
				if (pClientSession->GetPlayerData()->GetChannel()->ChannelErase())
				{
					Channel* temp = pClientSession->GetPlayerData()->GetChannel();
					SAFE_DELETE(temp);
				}
				pClientSession->GetPlayerData()->SetChannel(nullptr);
			}
		}

		//로그인된 세션 제거
		LoginChattingServerNetwork::DisconnectLoginClientSession(pClientSession);
		LeaveCriticalSection(&ClientSessionThreadCS);

		return false;
	}
	LeaveCriticalSection(&ClientSessionThreadCS);

	return true;
}

bool LoginChattingServerNetwork::IsDeleteSessionCommon(LoginChattingClientSession * pClientSession, DWORD bytesTransferred)
{
	EnterCriticalSection(&ClientSessionThreadCS);

	//하트비트에 문제 있다면, 클라에서 갑작스럽게 소켓 종료했을때 비정상 접속 종료
	if (bytesTransferred == 0 || !pClientSession->ChackHeartBeat())
	{
		//접속한 세션 제거
		if (!LoginChattingServerNetwork::DisconnectClientSession(pClientSession->GetSocket()))
		{
			return false;
		}

		if (pClientSession->GetPlayerData())
		{
			DATABASE->InsertUserLogQuery(pClientSession->GetPlayerData()->GetPlayerID(),
				L"정상 접속 종료");
			//파일 로그, 데이터 베이스 로그에 남김
			SLogPrintAtFile(L"%s : 정상 접속 종료",
				pClientSession->GetPlayerData()->GetPlayerID().c_str());

			if (pClientSession->GetPlayerData()->GetChannel())
			{
				pClientSession->GetClientSessionParser()->SendChannelOutAnnounce();

				pClientSession->GetPlayerData()->GetChannel()->DeleteClientSession(pClientSession);
				if (pClientSession->GetPlayerData()->GetChannel()->ChannelErase())
				{
					Channel* temp = pClientSession->GetPlayerData()->GetChannel();
					SAFE_DELETE(temp);
				}
				pClientSession->GetPlayerData()->SetChannel(nullptr);
			}
		}

		//로그인된 세션 제거
		LoginChattingServerNetwork::DisconnectLoginClientSession(pClientSession);
		LeaveCriticalSection(&ClientSessionThreadCS);

		return false;
	}
	LeaveCriticalSection(&ClientSessionThreadCS);

	return true;
}

bool LoginChattingServerNetwork::DisconnectClientSession(SOCKET socket)
{
	//recv, send한 IOData 2개가 들어오게됨
	//때문에 임계영역 설정
	if (!CLIENTSESSIONMANAGER->DeleteClientSession(socket))
	{
		LeaveCriticalSection(&ClientSessionThreadCS);
		return false;
	}

	return true;
}

bool LoginChattingServerNetwork::DisconnectLoginClientSession(LoginChattingClientSession* pClientSession)
{
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


	return true;
}
