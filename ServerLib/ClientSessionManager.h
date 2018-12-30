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
			//unordered_map<SOCKET, ClientSession*> m_mapClientSession;		//억셉트된 세션
			unordered_set<ClientSession*> m_setClientSession;

			bool m_bIsOn;
			HANDLE m_HarbeatEndEvent;

			ClientSessionManager();
			virtual ~ClientSessionManager();

		public:
			virtual HRESULT Init();
			virtual void Release();

			const UINT GetGenerateSessionID();

			bool			AddClientSession(ClientSession* const clientSession);
			bool			DeleteClientSession(ClientSession* cs);
			ClientSession*	FindClientSession(ClientSession* cs);

			void SendSessionHartBeats();
			const size_t GetCCU() const { return m_setClientSession.size(); }
		};
	}
}