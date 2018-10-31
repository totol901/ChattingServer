#pragma once

struct UserData
{
	UINT num_id;
	char id[15];
	char password[15];
	char nickname[15];
};

class DataManager : public Singleton<DataManager>
{
private:
	DataManager();
	~DataManager();
	map<string, UserData*> m_UserDataTable;

public:
	

	/****************************************************************************
	함수명	: insert
	설명		: data를 map에 넣어줌
	*****************************************************************************/
	bool insert(UserData* data);

	/****************************************************************************
	함수명	: insert
	설명		: id를 map에서 찾아 userdata를 리턴
	*****************************************************************************/
	UserData* find(const string& id);
};
