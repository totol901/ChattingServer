#include "stdafx.h"
#include "DBSession.h"

DBSession::DBSession()
	:Database()
{
}

DBSession::~DBSession()
{
}

bool DBSession::CheckUserInfoQuery(const wstring& ID, const wstring& PW)
{
	CHAR strID[1024] = { 0, };
	CHAR strPW[1024] = { 0, };
	Util::StrConvW2A((WCHAR*)ID.c_str(), strID, sizeof(strID));
	Util::StrConvW2A((WCHAR*)PW.c_str(), strPW, sizeof(strPW));
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

	SLogPrint("CheckUserInfoQuery 己傍...");

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

	SLogPrint("CheckUserInfoQuery 己傍...");

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
	Util::StrConvW2A((WCHAR*)query.c_str(), str, sizeof(str));

	if (mysql_query(conn, str) != NULL)
	{
		SLogPrintAtFile("Query Error : %s", mysql_error(conn));
		return false;
	}

	SLogPrint("InsertUserInfoQuery 己傍...");

	return true;
}

bool DBSession::DeleteUserInfoQuery(wstring ID)
{
	wstring query = L"DELETE FROM USER_INFO WHERE id = '";
	query += ID + L"'";

	char str[1024] = { 0, };
	Util::StrConvW2A((WCHAR*)query.c_str(), str, sizeof(str));

	if (mysql_query(conn, str) != NULL)
	{
		SLogPrintAtFile("Query Error : %s", mysql_error(conn));
		return false;
	}
	
	SLogPrint("DeleteUserInfoQuery 己傍...");

	return true;
}

wstring DBSession::FindNickname(wstring ID)
{
	wstring nickname;
	wstring query = L"SELECT nickname FROM USER_INFO WHERE ID='";
	query += ID + L"'";

	char str[1024] = { 0, };
	Util::StrConvW2A((WCHAR*)query.c_str(), str, sizeof(str));
	
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
			Util::StrConvA2W(row[0], str, sizeof(str));
			nickname = str;
		}
	}

	if (res->row_count == 0)
	{
		return false;
	}

	SLogPrint("FindNickname Query 己傍...");

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
	Util::StrConvW2A(q, str, sizeof(str));

	if (mysql_query(conn, str) != NULL)
	{
		SLogPrintAtFile("Query Error : %s", mysql_error(conn));
		return false;
	}

	SLogPrint("InsertUserLogQuery 己傍...");

	return true;
}
