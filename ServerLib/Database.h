#pragma once

namespace ServerEngine
{
	namespace DatabaseSystem
	{
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

			Database(); 
			virtual ~Database();

		public:
			/****************************************************************************
			함수명	: InitDB
			설명		: Mysql 초기화
			*****************************************************************************/
			HRESULT InitDB();
			void Release();

			/****************************************************************************
			함수명	: MySQLError
			설명		: 쿼리 에러 내용을 리턴함
			*****************************************************************************/
			const char* MySQLError() const { return mysql_error(conn); }
		};
	}
}