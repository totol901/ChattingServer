#pragma once

enum E_SCENE
{
	NONE,
	LOGIN,
	WAITTING_CHANNEL,
	IN_CHANNEL
};

class SceneManager : public Singleton<SceneManager>
{
	friend Singleton;
private:
	SceneManager();
	~SceneManager();

	map<UINT, BaseScene*> m_mapScenes;

	BaseScene* m_pCurrentScene;
	BaseScene* m_pBeforeScene;

public:
	BaseScene* GetCurrentScene() { return m_pCurrentScene; }
	BaseScene* GetBeforeScene() { return m_pBeforeScene; }
	bool ChangeCurrentScene(UINT SceneNum);
	bool InsertScene(BaseScene* Scene);
	bool DeleteScene(BaseScene* Scene);
};
