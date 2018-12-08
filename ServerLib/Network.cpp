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
				cout << "# WSA startup ����!" << endl;
				return E_FAIL;
			}

			cout << "### WSA 2.2 ���� ����!" << endl;
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
			//������ ���� ��ٸ���.
			for (auto iter = m_vecIOCPThread.begin(); iter != m_vecIOCPThread.end();
				iter++)
			{
				WaitForSingleObject((*iter), INFINITE);
			}
			SLogPrint("IOCP ������ ���� ����");

			ReleaseWinSock();
		}
	}
}
