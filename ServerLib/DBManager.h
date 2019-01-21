#pragma once

namespace ServerEngine
{
	namespace DatabaseSystem
	{
		class DBManager : public Singleton<DBManager>
		{
			friend Singleton;
		private:
			DBManager();
			~DBManager();

			DBManager(const DBManager&) = delete;
			DBManager(const DBManager&&) = delete;
			DBManager& operator = (const DBManager&) = delete;
			DBManager& operator = (const DBManager&&) = delete;

			int m_workerCount;
			std::vector<Database *>             m_dbPool;

			wstring m_serverName;
			wstring m_dbName;
			wstring m_login;
			wstring m_password;
			System::ThreadJobQueue<Query*>* m_pQueryPool;

		public:
			HRESULT Init();
			void Release();

			size_t runQueryCount();
			void pushQuery(Query *query);
			bool popQuery(Query **query);

			void run();
		};
	}
}
