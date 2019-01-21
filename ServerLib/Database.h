#pragma once

namespace ServerEngine
{
	namespace DatabaseSystem
	{
		typedef enum
		{
			DB_STOP,                // 정지
			DB_STANDBY,             // 쿼리 받을 준비
			DB_RUNNING,             // 쿼리 실행중
		} DB_STATE;

		class Database
		{
		protected:
			enum
			{
				DB_PORT = 3306,
				DB_OPT = 0
			};

			const char* DB_HOST;
			const char* DB_NAME;
			const char* DB_SOCK;
			const char* DB_USER;
			const char* DB_PASS;

			MYSQL*		conn;
			MYSQL_RES*	res;
			MYSQL_ROW	row;

			//-----------------------------
			DB_STATE        m_State;

		public:
			Database();
			virtual ~Database();

			/****************************************************************************
			함수명	: InitDB
			설명		: Mysql 초기화
			*****************************************************************************/
			HRESULT InitDB();
			void Release();

			virtual bool connect(const WCHAR *serverName, const WCHAR *dbName, const WCHAR *id, const WCHAR *password) {};// = 0;
			virtual bool connect() {};// = 0;
			virtual bool connected() {};// = 0;
			virtual bool disconnect() {};// = 0;

			virtual void run();// = 0;
			const DB_STATE& GetState() { return m_State; }

			/****************************************************************************
			함수명	: MySQLError
			설명		: 쿼리 에러 내용을 리턴함
			*****************************************************************************/
			const char* MySQLError() const { return mysql_error(conn); }
		};
	}
}