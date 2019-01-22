#include "stdafx.h"
#include "DBManager.h"

namespace ServerEngine
{
	namespace DatabaseSystem
	{
		DBManager::DBManager()
		{
		}

		DBManager::~DBManager()
		{
		}

		HRESULT DBManager::Init()
		{
			tinyxml2::XMLDocument config;

			if (!Util::loadConfig(&config))
			{
				return E_FAIL;
			}

			tinyxml2::XMLElement* root = config.FirstChildElement("App")->FirstChildElement("DataBase");
			if (!root) 
			{
				SLogPrintAtFile(L"@ not exist database setting");
				return E_FAIL;
			}

			tinyxml2::XMLElement* elem = root->FirstChildElement("ThreadCount");
			sscanf_s(elem->GetText(), "%d", &m_workerCount);

			array<WCHAR, 256> tmp;
			elem = root->FirstChildElement("ServerName");
			Util::StrConvA2W((char *)elem->GetText(), tmp.data(), tmp.max_size());
			m_serverName = tmp.data();

			elem = root->FirstChildElement("DB");
			Util::StrConvA2W((char *)elem->GetText(), tmp.data(), tmp.max_size());
			m_dbName = tmp.data();

			elem = root->FirstChildElement("Login");
			Util::StrConvA2W((char *)elem->GetText(), tmp.data(), tmp.max_size());
			m_login = tmp.data();

			elem = root->FirstChildElement("Password");
			Util::StrConvA2W((char *)elem->GetText(), tmp.data(), tmp.max_size());
			m_password = tmp.data();

			m_pQueryPool = new System::ThreadJobQueue<Query *>((WCHAR*)L"DBQueueJob");

			for (int i = 0; i < m_workerCount; ++i)
			{
				array<WCHAR, 128> patch = { 0, };
				ADODatabase *adodb = new ADODatabase();
				m_dbPool.push_back(adodb);
			}
			this->run();

			return S_OK;
		}

		void DBManager::Release()
		{
			SAFE_DELETE(m_pQueryPool);

			for (auto db : m_dbPool)
			{
				db->disconnect();
				SAFE_DELETE(db);
			}
		}

		size_t DBManager::runQueryCount()
		{
			return m_pQueryPool->size();
		}

		void DBManager::pushQuery(Query *query)
		{
			m_pQueryPool->push(query);
		}

		bool DBManager::popQuery(Query **query)
		{
			return m_pQueryPool->pop(*query);
		}

		void DBManager::run()
		{
			for (auto db : m_dbPool)
			{
				if (db->GetState() != DB_STOP)
				{
					continue;
				}

				if (!db->connect(m_serverName.c_str(), m_dbName.c_str(), m_login.c_str(), m_password.c_str())) 
				{
					SErrPrintAtFile(L"! db[%s] connection error", m_dbName.c_str());
				}

				db->run();
			}
		}
	}
}
