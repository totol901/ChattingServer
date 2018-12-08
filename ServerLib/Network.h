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

			virtual void CreateIOCP() = 0;

		private:
			HRESULT InitWinSock();
			void ReleaseWinSock();

		public:
			Network();
			virtual ~Network();

			virtual HRESULT Init();
			virtual void Release();

		public:
			const bool&		IsShutdown()	const { return m_Shutdown; }
			const SOCKET &	GetListenSock() const { return m_ListenSock; }
			const HANDLE &	GetIOCPHandle() const { return m_IOCP; }
			const WSADATA& GetWSAData() const { return m_WSAData; }
		};
	}
}