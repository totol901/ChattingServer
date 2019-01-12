#include "stdafx.h"
#include "SessionManager.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		SessionManager::SessionManager()
			: m_lock((WCHAR*)L"SessionManager"),
			m_sessionCount(1)
		{
			this->commandFuncInitialize();
		}

		SessionManager::~SessionManager()
		{
		}

		HRESULT SessionManager::Init(int maxConnection)
		{
			this->commandFuncInitialize();

			return S_OK;
		}

		void SessionManager::Release()
		{
			vector<Session *> removeSessionVec;
			removeSessionVec.resize(m_sessionList.size());
			std::copy(m_sessionList.begin(), m_sessionList.end(), removeSessionVec.begin());
			for (auto session : removeSessionVec)
			{
				session->OnClose();
			}
			m_sessionList.clear();
		}

		list<Session*>& SessionManager::sessionList()
		{
			return m_sessionList;
		}

		UINT64 SessionManager::createSessionId()
		{
			return m_SessionSeed++;
		}


		bool SessionManager::addSession(Session *session)
		{
			Util::SAFE_LOCK(m_lock);
			auto findSession = std::find(m_sessionList.begin(), m_sessionList.end(), session);
			if (findSession != m_sessionList.end())
			{
				return false;
			}
			if (m_sessionCount > m_maxConnection)
			{
				SLogPrintAtFile(L"* session so busy. count[%d]", m_sessionCount);
				return false;
			}

			session->SetId(this->createSessionId());
			m_sessionList.push_back(session);
			++m_sessionCount;
			return true;
		}

		//소켓을 닫으라는 클라이언트에게 보냅니다.
		bool SessionManager::closeSession(Session *session)
		{
			Util::SAFE_LOCK(m_lock);

			if (session == nullptr)
			{
				return false;
			}

			auto findSession = std::find(m_sessionList.begin(), m_sessionList.end(), session);
			if (findSession != m_sessionList.end())
			{
				Session *delSession = *findSession;
				
				SLogPrintAtFile("* detected close by client [%s]", inet_ntoa(delSession->GetClientAddr().sin_addr));
				::closesocket(delSession->GetSocket());

				m_sessionList.remove(delSession);
				--m_sessionCount;

				SAFE_DELETE(delSession);

				return true;
			}

			return false;
		}


		//소켓을 강제로 닫습니다.
		void SessionManager::forceCloseSession(Session *session)
		{
			Util::SAFE_LOCK(m_lock);
			if (!session)
			{
				return;
			}

			//우아한 종료 유도. 원래는 클라이언트에서 서버 접속을 종료하도록 유도해야 한다.
			LINGER linger;
			linger.l_onoff = 1;   //사용
			linger.l_linger = 0;  //대기시간, 0일시 완료 안된 패킷 버리고 즉시 종료.

			::setsockopt(session->GetSocket(), SOL_SOCKET, SO_LINGER, (char *)&linger, sizeof(linger));
			this->closeSession(session);
		}

		Session * SessionManager::GetSession(UINT64 id)
		{
			Util::SAFE_LOCK(m_lock);
			Session *findSession = nullptr;

			for (auto session : m_sessionList)
			{
				if (session->GetId() == id)
				{
					findSession = session;
					break;
				}
			}

			return findSession;
		}


		void SessionManager::runCommand(wstring cmdLine)
		{
			std::size_t found = cmdLine.find(L' ');
			wstring command;
			wstring arg;
			if (found != wstring::npos)
			{
				command = cmdLine.substr(0, found);
				arg = cmdLine.substr(found);
			}
			else
			{
				command = cmdLine;
			}

			auto itr = m_serverCommand.find(command);
			if (itr == m_serverCommand.end())
			{
				return;
			}

			auto cmdFunc = m_serverCommand.at(command);
			if (cmdFunc)
			{
				cmdFunc(&m_sessionList, &arg);
			}
		}

		// 서버에서 내리는 치트키 정의
		void SessionManager::commandFuncInitialize()
		{
#if 0
			//기본적인 3개만 생성, 이후 늘어나면 별도 클래스로 분리
			auto notiyFunc = [](SessionList *sessionList, wstr_t *arg)
			{
				auto eachFunc = [arg](void *atom)
				{
					Session *session = (Session*)atom;
					if (session->type() == SESSION_TYPE_TERMINAL)
					{
						return;
					}
					array<CHAR, SIZE_256> tmpBuf;
					StrConvW2A((WCHAR*)arg->c_str(), tmpBuf.data(), tmpBuf.size());

					PK_S_ANS_CHATTING retPacket;
					retPacket.id_ = "Server";
					retPacket.text_ = "* Notity : ";
					retPacket.text_ += tmpBuf.data();

					session->sendPacket(&retPacket);
				};

				for (auto session : *sessionList)
				{
					eachFunc(session);
				}
			};

			auto kickoffFunc = [](SessionList *sessionList, wstr_t *arg)
			{
				vector<Session *> removeSessionVec;
				auto eachFunc = [&removeSessionVec, arg](void *atom)
				{
					Session *session = (Session*)atom;
					if (session->type() == SESSION_TYPE_TERMINAL)
					{
						return;
					}

					PK_S_ANS_CHATTING retPacket;
					retPacket.id_ = "Server";
					retPacket.text_ = "! Kick off by Server";
					session->sendPacket(&retPacket);

					removeSessionVec.push_back(session);
				};
				for (auto session : *sessionList)
				{
					eachFunc(session);
				}

				for (auto session : removeSessionVec)
				{
					session->onClose();
				}
			};

			auto exitFunc = [](SessionList *sessionList, wstr_t *arg)
			{
				_shutdown = true;
			};

			//명령어 등록
			serverCommand_.insert(make_pair(L"/notify", notiyFunc));
			serverCommand_.insert(make_pair(L"/kickoff", kickoffFunc));
			serverCommand_.insert(make_pair(L"/exit", exitFunc));
#endif
		}
	}
}