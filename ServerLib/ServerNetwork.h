/******************************************************************************
���ϸ�	: ServerNetwork.h
����		: ������Ʈ��ũ�� �����
******************************************************************************/
#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
		class ServerNetwork : public Network
		{
		protected:
			ServerNetwork();
			virtual ~ServerNetwork();

			void CreateListen();
			void CreateIOCP();

			virtual void CreateIOCPThreads() = 0;
			virtual void CreateAcceptThread() = 0;

		public:
			virtual HRESULT Init();
			virtual void Release();

		public:
			const bool&		IsShutdown()	const { return m_Shutdown; }
			const SOCKET &	GetListenSock() const { return m_ListenSock; }
			const HANDLE &	GetIOCPHandle() const { return m_IOCP; }
		};
	}
}