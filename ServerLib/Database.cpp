#include "stdafx.h"
#include "Database.h"

namespace ServerEngine
{
	namespace DatabaseSystem
	{
		Database::Database()
		{
			DB_HOST = "localhost";
			DB_NAME = "login_db";
			DB_SOCK = NULL;
			DB_USER = "root";
			DB_PASS = "12345";
		}

		Database::~Database()
		{
		}

		HRESULT Database::InitDB()
		{
			conn = mysql_init(NULL);

			if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME,
				DB_PORT, DB_SOCK, DB_OPT))
			{
				SLogPrintAtFile("DB Connect Error : %s", mysql_error(conn));
				return E_FAIL;
			}

			//한글 쓰기 위한 세팅
			if (mysql_query(conn, "set character_set_client = euckr"))
			{
				SLogPrintAtFile("DB  Error : %s", mysql_error(conn));
				return E_FAIL;
			}
			if (mysql_query(conn, "set character_set_connection = euckr"))
			{
				SLogPrintAtFile("DB  Error : %s", mysql_error(conn));
				return E_FAIL;
			}
			if (mysql_query(conn, "set character_set_results = euckr"))
			{
				SLogPrintAtFile("DB  Error : %s", mysql_error(conn));
				return E_FAIL;
			}

			if (mysql_query(conn, "SHOW TABLES"))
			{
				SLogPrintAtFile("DB Query Error : %s", mysql_error(conn));
				return E_FAIL;
			}

			res = mysql_use_result(conn);

			SLogPrint("** %s 안의 테이블을 보여줌 **", DB_NAME);

			while ((row = mysql_fetch_row(res)) != NULL)
			{
				SLogPrint(row[0]);
			}

			return S_OK;
		}

		void Database::Release()
		{
			mysql_free_result(res);
			mysql_close(conn);
		}
	}
}