#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
constexpr auto SESSION_CAPACITY = 5000;

		class SessionManager : public Singleton<SessionManager>
		{
			friend Singleton;
		private:
			SessionManager();
			~SessionManager();

			SessionManager(const SessionManager&) = delete;
			SessionManager(const SessionManager&&) = delete;
			SessionManager& operator = (const SessionManager&) = delete;
			SessionManager& operator = (const SessionManager&&) = delete;

			typedef list<Session*> SessionList;

		private:
			SessionList	m_sessionList;
			int m_sessionCount;
			int m_maxConnection;
			Util::Lock m_lock;

			UINT64 m_SessionSeed;

			typedef std::function<void(SessionList *sessionList, wstring *arg)> cmdFunc;
			unordered_map<wstring, cmdFunc>   m_serverCommand;

		public:
			HRESULT Init(int maxConnection = SESSION_CAPACITY);
			void Release();

			/****************************************************************************
			�Լ���	: createSessionId
			����		: sessionSeed_++�Ͽ� ��������
			���ϰ�	: oid_t
			�Ű�����	: ����
			*****************************************************************************/
			UINT64				createSessionId();

			/****************************************************************************
			�Լ���	: addSession
			����		: session�� �߰����� ������ ������ �����ϸ� false
			���ϰ�	: bool
			�Ű�����	: Session*
			*****************************************************************************/
			bool				addSession(Session *session);

			/****************************************************************************
			�Լ���	: closeSession
			����		: session�� ��������, ��Ĺ�� ������� Ŭ���̾�Ʈ���� ��û
			���ϰ�	: bool
			�Ű�����	: Session*
			*****************************************************************************/
			bool				closeSession(Session *session);

			/****************************************************************************
			�Լ���	: forceCloseSession
			����		: session�� ��������, ��Ĺ�� ������ ����
			���ϰ�	: bool
			�Ű�����	: Session*
			*****************************************************************************/
			void				forceCloseSession(Session *session);

			/****************************************************************************
			�Լ���	: session
			����		: id�� ������ ��������
			���ϰ�	: Session*
			�Ű�����	: oid_t
			*****************************************************************************/
			Session				*GetSession(UINT64 id);

			/****************************************************************************
			�Լ���	: runCommand
			����		: cmd�� ���� ���� Ŀ�ǵ� ó��
			���ϰ�	: ����
			�Ű�����	: wstr_t
			*****************************************************************************/
			void                runCommand(wstring cmd);

			/****************************************************************************
			�Լ���	: commandFuncInitialize
			����		: Ŀ�ǵ� �ʱ�ȭ(ġƮŰ)
			���ϰ�	: ����
			�Ű�����	: ����
			*****************************************************************************/
			void                commandFuncInitialize();

		public:
			//get�Լ�
			list<Session*>		&sessionList();
		};
	}
}
