#include "stdafx.h"
#include "ADODatabase.h"

namespace ServerEngine
{
	namespace DatabaseSystem
	{
		ADODatabase::ADODatabase()
			:m_pThread(nullptr)
		{
			::CoInitialize(NULL);
			m_State = DB_STOP;

			p_DbConnection.CreateInstance(__uuidof(::Connection));
			if (p_DbConnection == NULL) 
			{
				SErrPrintAtFile(L"! Database init fail");
			}
			const int TIME_OUT = 30;
			this->setConnectTimeOut(TIME_OUT);
		}

		ADODatabase::~ADODatabase()
		{
			this->disconnect();
			if (p_DbConnection)
			{
				p_DbConnection.Release();
			}
			SAFE_DELETE(m_pThread);
			::CoUninitialize();
		}

		HRESULT	ADODatabase::setConnectTimeOut(long second)
		{
			if (!p_DbConnection)
			{
				return S_FALSE;
			}
			return p_DbConnection->put_ConnectionTimeout(second);
		}

		void ADODatabase::comError(const WCHAR *actionName, _com_error &e)
		{
			SLogPrintAtFile(L"! [%s]DB [%s] fail [%s]", m_DbName.c_str(), actionName, (WCHAR *)e.Description());
		}

		bool ADODatabase::connect()
		{
			if (!p_DbConnection)
			{
				return false;
			}

			try 
			{
				HRESULT hr = p_DbConnection->Open(m_ConnectionStr.c_str(), L"", L"", NULL);
				if (SUCCEEDED(hr)) 
				{
					p_DbConnection->PutCursorLocation(::adUseClient);
					SLogPrintAtFile(L"* [%s]DB connection success", m_DbName.c_str());
					m_State = DB_STANDBY;

					return true;
				}
			}
			catch (_com_error &e) 
			{
				this->comError(L"connction", e);
			}

			return false;
		}

		bool ADODatabase::connect(const WCHAR *provider, const WCHAR *serverName, const WCHAR *dbName, const WCHAR *id, const WCHAR *password)
		{
			array<WCHAR, 128> buffer;
			_snwprintf_s(buffer.data(), buffer.size(), _TRUNCATE, L"Provider=%s;Server=%s;Database=%s;Uid=%s;Pwd=%s;", provider, serverName, dbName, id, password);
			
			m_ConnectionStr = buffer.data();
			SLogPrintAtFile(L"* [%s]DB try connection provider = %s", m_DbName.c_str(), provider);

			return this->connect();
		}

		bool ADODatabase::connect(const WCHAR *serverName, const WCHAR *dbName, const WCHAR *id, const WCHAR *password)
		{
			m_DbName = dbName;
			SLogPrintAtFile(L"* connect try: %s, %s, %s", dbName, id, password);

			for (int index = 10; index < 20; ++index) 
			{
				array<WCHAR, 64> mssqlName;

				_snwprintf_s(mssqlName.data(), mssqlName.size(), _TRUNCATE, L"SQLNCLI%d", index);

				if (this->connect(mssqlName.data(), serverName, dbName, id, password)) 
				{
					SLogPrintAtFile(L"* database %s : %s connect", mssqlName, dbName);
					return true;
				}
			}

			//mssql 2005, 2008로 접속시
			if (this->connect(L"SQLNCLI", serverName, dbName, id, password)) 
			{
				SLogPrintAtFile(L"* database SQLNCLI : %s connect", dbName);
				return true;
			}
			return false;
		}

		bool ADODatabase::connected()
		{
			return p_DbConnection->State != ::adStateClosed ? true : false;
		}

		bool ADODatabase::disconnect()
		{
			if (!p_DbConnection) 
			{
				return false;
			}

			if (m_State == DB_STOP) 
			{
				return true;
			}

			try 
			{
				this->execute();

				if (!p_DbConnection)
				{
					return true;
				}
				p_DbConnection->Close();
				m_State = DB_STOP;

				m_ConnectionStr.clear();
				m_DbName.clear();
				SLogPrintAtFile(L"* database close");

				return true;
			}
			catch (...) 
			{
				SLogPrintAtFile(L"! Database[%s] disconnect fail", m_DbName.c_str());
			}

			return false;
		}

		void ADODatabase::execute()
		{
			if (DBMANAGER->runQueryCount() == 0) 
			{
				return;
			}

			Query *query = nullptr;
			if (DBMANAGER->popQuery(&query) == false) 
			{
				return;
			}

			QueryStatement *statement = query->GetStatement();

			const WCHAR *sqlQuery = statement->GetQuery();
			try 
			{
				m_State = DB_RUNNING;
				QueryRecord record;
				
				::_CommandPtr command;
				command.CreateInstance(__uuidof(::Command));
				command->ActiveConnection = p_DbConnection;
				command->CommandType = ::adCmdText;
				command->CommandText = sqlQuery;
				_variant_t resultVal;

				switch (statement->GetType()) 
				{
				case QUERY_NOT_RETURN:
					record = command->Execute(&resultVal, NULL, ::adCmdText | ::adExecuteNoRecords);
					break;
				case QUERY_WAIT_RETURN:
					record = command->Execute(&resultVal, NULL, ::adCmdText | ::adExecuteRecord);
					break;
				case QUERY_CALL_BACK:
					record = command->Execute(&resultVal, NULL, ::adCmdText | ::adAsyncFetchNonBlocking);
					break;
				}

				if (record.isEof()) 
				{
					int quertResultVal = atol((char*)((_bstr_t)resultVal));

					if (quertResultVal < 1) 
					{
						SLogPrintAtFile(L"* query : [%s] have error code [%d] ", sqlQuery, quertResultVal);
					}
					else 
					{
						record.setResultVal(quertResultVal);
					}
				}

				query->GetResult() = record;
				m_State = DB_STANDBY;

				SLogPrintAtFile(L"* Run query [%s] result [%d]", sqlQuery, record.resultVal());
			}
			catch (_com_error &e) 
			{
				this->comError(L"execute", e);
			}

			SAFE_DELETE(query);
		}

		unsigned int __stdcall ADODatabase::process(LPVOID adoDatabase)
		{
			ADODatabase* temp = (ADODatabase*)adoDatabase;

			while (Util::GlobalValue::GetInstance()->m_IsShutdown == false)
			{
				if (!temp->connected())
				{
					SLogPrintAtFile(L"! db[%s] connection disconnect", temp->m_DbName.c_str());
					ASSERT(FALSE);
				}
				temp->execute();

				CONTEXT_SWITCH;
			}

			return 0;
		}

		void ADODatabase::run()
		{
			m_pThread = new System::Thread(this, process);
		}
	}
}
