/******************************************************************************
���ϸ�	: ClientSessionManager.h
����		: Ŭ���̾�Ʈ ���ǵ��� ������
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
			unordered_map<SOCKET, ClientSession*> m_mapClientSession;		//���Ʈ�� ����

			bool m_bIsOn;
			HANDLE m_HarbeatEndEvent;

			ClientSessionManager();
			virtual ~ClientSessionManager();

		public:
			virtual HRESULT Init();
			virtual void Release();

			const UINT GetGenerateSessionID();

			bool			AddClientSession(ClientSession* const clientSession);
			bool			DeleteClientSession(const SOCKET& socket);
			ClientSession*	FindClientSession(SOCKET socket);

			void SendSessionHartBeats();

			const size_t GetCCU() const { return m_mapClientSession.size(); }
		};
	}
}