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
	//서버 connect
	p_mServerSession = new IOCPServerSession;
	p_mServerSession->OnConnect(serverIp, serverPort);

	//IOCP 생성
	const DWORD numberOfCurrentThredCount = 1;
	m_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 
		NULL, 0, numberOfCurrentThredCount);
	if (m_IOCP == nullptr)
	{
		WSAERROR->err_print(TEXT("IOCP 생성 실패!"));
		return;
	}

	//IOCP에 사용될 스레드 생성
	for (int i = 0; i < numberOfCurrentThredCount * 2; i++)
	{
		_beginthreadex(NULL, 0, WorkThread, m_IOCP, 0, NULL);
	}

	//ServerSession을 IOCP에 소켓 연결
	CreateIoCompletionPort((HANDLE)p_mServerSession->GetSocket(),
		m_IOCP, (ULONG_PTR)p_mServerSession, NULL);

	//recv 비동기 시작
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

		//오류 처리 및 종료 처리
		if (!ret)
		{
			if (BytesTransferred == 0)
			{
				WSAERROR->err_print(TEXT("IOCP 생성 실패!"));
				return 0;
			}
			WSAERROR->err_print(TEXT("IOCP 생성 실패!"));
			return 0;
		}
		if (pSession == nullptr)
		{
			WSAERROR->err_print(TEXT("IOCP 생성 실패!"));
			return 0;
		}
		if (BytesTransferred == 0)
		{
			WSAERROR->err_print(TEXT("서버와 정상 접속 종료.\n"));
			return 0;
		}

		//패킷의 타입에 따라 Send, 파싱, 에러처리 해줌
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
						//종료 패킷을 받음
						return 0;
					}
					SAFE_DELETE(packet);
				}
			}
			continue;

		case IO_ERROR:
			WSAERROR->err_print(TEXT("IO 에러\n"));
			ASSERT(false);
			continue;
		}
	}

	return 0;
}
