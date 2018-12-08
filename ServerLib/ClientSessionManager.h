/******************************************************************************
파일명	: ClientSessionManager.h
목적		: 클라이언트 세션들을 관리함
******************************************************************************/
#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
		class ClientSessionManager
		{
		protected:
			CRITICAL_SECTION ManagerCS;
			UINT						m_GenerateSessionID;
			map<SOCKET, ClientSession*> m_mapClientSession;		//억셉트된 세션

			ClientSessionManager();
			virtual ~ClientSessionManager();

		public:
			virtual HRESULT Init();
			virtual void Release();

			const UINT GetGenerateSessionID();

			bool			AddClientSession(ClientSession* const clientSession);
			bool			DeleteClientSession(const SOCKET& socket);
			ClientSession*	FindClientSession(SOCKET socket);

			const size_t GetCCU() const { return m_mapClientSession.size(); }
		};
	}
}