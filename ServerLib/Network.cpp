#include "stdafx.h"
#include "Network.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		Network::Network()
			:m_Shutdown(false)
		{
		}

		Network::~Network()
		{
		}

		HRESULT Network::InitWinSock()
		{
			memset(&m_WSAData, 0, sizeof(WSADATA));

			if (WSAStartup(MAKEWORD(2, 2), &m_WSAData) != 0)
			{
				cout << "# WSA startup 실패!" << endl;
				return E_FAIL;
			}

			cout << "### WSA 2.2 세팅 성공!" << endl;
			return S_OK;
		}

		void Network::ReleaseWinSock()
		{
			WSACleanup();
		}

		HRESULT Network::Init()
		{
			HRESULT hr = InitWinSock();

			return hr;
		}

		void Network::Release()
		{
			m_Shutdown = true;
			CloseHandle(m_IOCP);
			closesocket(m_ListenSock);
			//스래드 종료 기다린다.
			for (auto iter = m_vecIOCPThread.begin(); iter != m_vecIOCPThread.end();
				iter++)
			{
				WaitForSingleObject((*iter), INFINITE);
			}
			SLogPrint("IOCP 스래드 종료 성공");

			ReleaseWinSock();
		}
	}
}
