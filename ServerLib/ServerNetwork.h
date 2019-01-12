/******************************************************************************
파일명	: ServerNetwork.h
목적		: 서버네트워크를 담당함
******************************************************************************/
#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
		enum SERVER_STATUS
		{
			SERVER_STOP,
			SERVER_INITIALZE,
			SERVER_READY,
		};

		class ServerNetwork : public Network
		{
		protected:
			SERVER_STATUS m_status;
			//ConetentsProcess;

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