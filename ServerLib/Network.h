/******************************************************************************
���ϸ�	: ServerNetwork.h
����		: ������Ʈ��ũ�� �����
******************************************************************************/
#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
		class Network
		{
		protected:
			vector<HANDLE> m_vecIOCPThread;
			HANDLE	m_IOCP;
			SOCKET	m_ListenSock;
			bool	m_Shutdown;
			WSADATA m_WSAData;

			char m_ip[16];
			int m_port;
			int m_workThreadCount;

			virtual void CreateIOCP() {};

		private:
			HRESULT InitWinSock();
			void ReleaseWinSock();

		public:
			Network();
			virtual ~Network();

			virtual HRESULT Init();
			virtual void Release();

		public:
			const char *GetIP() const { return m_ip; }
			const int& GetPort() const { return m_port; }
			const int& GetThreadCount() const { return m_workThreadCount; }
			const bool&		IsShutdown()	const { return m_Shutdown; }
			const SOCKET &	GetListenSock() const { return m_ListenSock; }
			const HANDLE &	GetIOCPHandle() const { return m_IOCP; }
			const WSADATA& GetWSAData() const { return m_WSAData; }
		};
	}
}