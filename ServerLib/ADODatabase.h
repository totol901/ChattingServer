#pragma once

namespace ServerEngine
{
	namespace DatabaseSystem
	{
		class ADODatabase : public Database
		{
		private:
			::_ConnectionPtr p_DbConnection;
			wstring m_ConnectionStr;
			wstring m_DbName;

			System::Thread* m_pThread;

		public:
			ADODatabase();
			~ADODatabase();

			HRESULT	setConnectTimeOut(long second);
			void comError(const WCHAR *actionName, _com_error &e);

			bool connect(const WCHAR *provider, const WCHAR *serverName, const WCHAR *dbName, const WCHAR *id, const WCHAR *password);
			bool connect(const WCHAR *serverName, const WCHAR *dbName, const WCHAR *id, const WCHAR *password);
			bool connect();
			bool connected();
			bool disconnect();

			void run();

		private:
			void execute();
			static unsigned int WINAPI process(LPVOID adoDatabase);
		};
	}
}
