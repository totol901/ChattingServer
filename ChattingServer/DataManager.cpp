#include "stdafx.h"
#include "DataManager.h"


DataManager::DataManager()
{
}

DataManager::~DataManager()
{
}

bool DataManager::insert(UserData* data)
{
	auto iter = m_UserDataTable.find(data->id);

	if (iter == m_UserDataTable.end())
	{
		m_UserDataTable.insert(make_pair(string(data->id), data));
		return true;
	}

	return false;
}

UserData * DataManager::find(const string & id)
{
	auto iter = m_UserDataTable.find(id);

	if (iter == m_UserDataTable.end())
	{
		return nullptr;
	}

	return iter->second;
}
