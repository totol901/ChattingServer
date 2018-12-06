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

	SLogPrint("** %s 안의 테이블을 보여줌 **", DB_NAME);

	while ((row = mysql_fetch_row(res)) != NULL)
	{
		SLogPrint(row[0]);
	}
	return true;
}

bool DBSession::CheckUserInfoQuery(const wstring& ID, const wstring& PW)
{
	CHAR strID[1024] = { 0, };
	CHAR strPW[1024] = { 0, };
	StrConvW2A((WCHAR*)ID.c_str(), strID, sizeof(strID));
	StrConvW2A((WCHAR*)ID.c_str(), strPW, sizeof(strPW));
	string id = strID;
	string pw = strPW;

	string query = "SELECT * FROM USER_INFO WHERE ID='";
	query += id + "' AND PW='";
	query += pw + "'";

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
		if (row[0] == id)
		{
			if (row[1] == pw)
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

	SLogPrint("CheckUserInfoQuery 성공...");

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

	SLogPrint("CheckUserInfoQuery 성공...");

	return true;
}

bool DBSession::InsertUserInfoQuery(wstring ID, wstring PW, wstring nickname)
{
	wstring query = L"INSERT INTO USER_INFO VALUE ('";

	query += ID;
	query += L"', '";
	query += PW + L"', '";
	query += nickname + L"')";

	char str[1024] = { 0, };
	StrConvW2A((WCHAR*)query.c_str(), str, sizeof(str));

	if (mysql_query(conn, str) != NULL)
	{
		SLogPrintAtFile("Query Error : %s", mysql_error(conn));
		return false;
	}

	SLogPrint("InsertUserInfoQuery 성공...");

	return true;
}

bool DBSession::DeleteUserInfoQuery(wstring ID)
{
	wstring query = L"DELETE FROM USER_INFO WHERE id = '";
	query += ID + L"'";

	char str[1024] = { 0, };
	StrConvW2A((WCHAR*)query.c_str(), str, sizeof(str));

	if (mysql_query(conn, str) != NULL)
	{
		SLogPrintAtFile("Query Error : %s", mysql_error(conn));
		return false;
	}
	
	SLogPrint("DeleteUserInfoQuery 성공...");

	return true;
}

wstring DBSession::FindNickname(wstring ID)
{
	wstring nickname;
	wstring query = L"SELECT nickname FROM USER_INFO WHERE ID='";
	query += ID + L"'";

	char str[1024] = { 0, };
	StrConvW2A((WCHAR*)query.c_str(), str, sizeof(str));
	
	//char q[256];
	//memset(q, 0, sizeof(q));
	//memcpy(q, query.c_str(), query.size());

	if (mysql_query(conn, str) != NULL)
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
			TCHAR str[1024] = { 0, };
			StrConvA2W(row[0], str, sizeof(str));
			nickname = str;
		}
	}
	if (res->row_count == 0)
	{
		return false;
	}

	SLogPrint("FindNickname Query 성공...");

	return nickname;
}

bool DBSession::InsertUserLogQuery(wstring ID, wstring log)
{
	wstring query = L"INSERT INTO USER_LOG VALUE ('";

	query += ID;
	query += L"', '";
	query += TIMER->NowTimeWithSec();
	query += L"', '";
	query += log + L"')";

	TCHAR q[256];
	memset(q, 0, sizeof(q));
	memcpy(q, query.c_str(), query.size()* sizeof(wchar_t));

	char str[1024] = { 0, };
	StrConvW2A(q, str, sizeof(str));

	if (mysql_query(conn, str) != NULL)
	{
		SLogPrintAtFile("Query Error : %s", mysql_error(conn));
		return false;
	}

	SLogPrint("InsertUserLogQuery 성공...");

	return true;
}
