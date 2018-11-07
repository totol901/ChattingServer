#include "stdafx.h"
#include "DBSession.h"

DBSession::DBSession()
{
	DB_HOST = "localhost";
	DB_NAME = "login_db";
	DB_SOCK = NULL;
	DB_USER = "root";
	DB_PASS = "12345";
}

DBSession::~DBSession()
{
	mysql_free_result(res);
	mysql_close(conn);
}

bool DBSession::InitDB()
{
	conn = mysql_init(NULL);

	if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME,
		DB_PORT, DB_SOCK, DB_OPT))
	{
		SLogPrintAtFile("DB Connect Error : %s", mysql_error(conn));
		return false;
	}

	//한글 쓰기 위한 세팅
	if (mysql_query(conn, "set character_set_client = euckr"))
	{
		SLogPrintAtFile("DB  Error : %s", mysql_error(conn));
		return false;
	}
	if (mysql_query(conn, "set character_set_connection = euckr"))
	{
		SLogPrintAtFile("DB  Error : %s", mysql_error(conn));
		return false;
	}
	if (mysql_query(conn, "set character_set_results = euckr"))
	{
		SLogPrintAtFile("DB  Error : %s", mysql_error(conn));
		return false;
	}

	if (mysql_query(conn, "SHOW TABLES"))
	{
		SLogPrintAtFile("DB Query Error : %s", mysql_error(conn));
		return false;
	}

	res = mysql_use_result(conn);

	SLogPrint("**DB Show Tables in %s **", DB_NAME);

	while ((row = mysql_fetch_row(res)) != NULL)
	{
		SLogPrint(row[0]);
	}
	return true;
}

bool DBSession::CheckUserInfoQuery(string ID, string PW)
{
	string query = "SELECT * FROM USER_INFO WHERE ID='";
	query += ID + "' AND PW='";
	query += PW + "'";

	char q[256];
	memset(q, 0, sizeof(q));
	memcpy(q, query.c_str(), sizeof(query));

	if (mysql_query(conn, q) != NULL)
	{
		SLogPrintAtFile("Query Error : %s", mysql_error(conn));
		return false;
	}
	
	res = mysql_store_result(conn);
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		if (row[0] == ID)
		{
			if (row[1] == PW)
			{
				break;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		if (row == NULL)
		{
			return false;
		}
	}
	if (res->row_count == 0)
	{
		return false;
	}

	SLogPrint("Query Success...");

	return true;
}

bool DBSession::InsertUserInfoQuery(string ID, string PW, string nickname)
{
	string query = "INSERT INTO USER_INFO VALUE ('";

	query += ID;
	query += "', '";
	query += PW + "', '";
	query += nickname + "')";

	if (mysql_query(conn, query.c_str()) != NULL)
	{
		SLogPrintAtFile("Query Error : %s", mysql_error(conn));
		return false;
	}

	SLogPrint("Query Success...");

	return true;
}

bool DBSession::DeleteUserInfoQuery(string ID)
{
	string query = "DELETE FROM USER_INFO WHERE id = '";
	query += ID + "'";

	if (mysql_query(conn, query.c_str()) != NULL)
	{
		SLogPrintAtFile("Query Error : %s", mysql_error(conn));
		return false;
	}
	
	SLogPrint("Query Success...");

	return true;
}

string DBSession::FindNickname(string ID)
{
	string nickname;
	string query = "SELECT nickname FROM USER_INFO WHERE ID='";
	query += ID + "'";
	
	char q[256];
	memset(q, 0, sizeof(q));
	memcpy(q, query.c_str(), query.size());

	if (mysql_query(conn, q) != NULL)
	{
		SLogPrintAtFile("Query Error : %s", mysql_error(conn));
		return false;
	}

	res = mysql_store_result(conn);
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		if (row == NULL)
		{
			return false;
		}
		else
		{
			nickname = row[0];
		}
	}
	if (res->row_count == 0)
	{
		return false;
	}

	SLogPrint("Query Success...");

	return nickname;
}

bool DBSession::InsertUserLogQuery(string ID, string log)
{
	string query = "INSERT INTO USER_LOG VALUE ('";

	query += ID;
	query += "', '";
	query += TIMER->NowTimeWithSec();
	query += "', '";
	query += log + "')";

	char q[256];
	memset(q, 0, sizeof(q));
	memcpy(q, query.c_str(), query.size());

	if (mysql_query(conn, q) != NULL)
	{
		SLogPrintAtFile("Query Error : %s", mysql_error(conn));
		return false;
	}

	SLogPrint("Query Success...");

	return true;
}
