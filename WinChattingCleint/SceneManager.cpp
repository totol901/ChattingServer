#include "stdafx.h"
#include "SceneManager.h"

SceneManager::SceneManager()
	:m_pCurrentScene(nullptr),
	m_pBeforeScene(nullptr)
{
}

SceneManager::~SceneManager()
{
	for (auto iter = m_mapScenes.begin();
		iter != m_mapScenes.end(); iter++)
	{
		SAFE_RELEASE(iter->second);
		SAFE_DELETE(iter->second);
	}
}

bool SceneManager::ChangeCurrentScene(UINT SceneNum)
{
	auto iter = m_mapScenes.find(SceneNum);
	if (iter != m_mapScenes.end())
	{
		m_pBeforeScene = m_pCurrentScene;
		m_pCurrentScene = iter->second;
		m_pCurrentScene->Init();
		return true;
	}

	return false;
}

bool SceneManager::InsertScene(BaseScene * Scene)
{
	auto iter = m_mapScenes.find(Scene->GetID());
	if (iter == m_mapScenes.end())
	{
		m_mapScenes.insert(make_pair(Scene->GetID(), Scene));
		return true;
	}
	
	return false;
}

bool SceneManager::DeleteScene(BaseScene * Scene)
{
	auto iter = m_mapScenes.find(Scene->GetID());
	if (iter == m_mapScenes.end())
	{
		return false;
	}

	SAFE_RELEASE(iter->second);
	SAFE_DELETE(iter->second);
	m_mapScenes.erase(iter);
	return true;
}
