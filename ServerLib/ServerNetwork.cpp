#include "stdafx.h"
#include "ServerNetwork.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		ServerNetwork::ServerNetwork()
			:Network()
		{

		}

		ServerNetwork::~ServerNetwork()
		{

		}

		HRESULT ServerNetwork::Init()
		{
			HRESULT hr = Network::Init();

			CreateIOCP();
			CreateListen();

			return hr;
		}

		void ServerNetwork::Release()
		{
			Network::Release();
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



		void ServerNetwork::CreateListen()
		{
			m_ListenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

			SOCKADDR_IN servAddr;
			memset(&servAddr, 0, sizeof(SOCKADDR_IN));
			servAddr.sin_family = AF_INET;
			servAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
			servAddr.sin_port = ::htons(9000);

			int reUseAddr = 1;
			setsockopt(m_ListenSock, SOL_SOCKET, SO_REUSEADDR, (char *)&reUseAddr, (int)sizeof(reUseAddr));

			// XXX : :: <-이거 삭제금지, std::bind로 변경됨
			::bind(m_ListenSock, (const SOCKADDR*)&servAddr, sizeof(servAddr));
			listen(m_ListenSock, 5);
			SLogPrintAtFile("Listen 시작!");

			CreateAcceptThread();
		}
	}
}
