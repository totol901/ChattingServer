/******************************************************************************
���ϸ�	: GameEngine.h
����		: �̱������μ� �׷���, ���ҽ��� �����ϴ� �� ������ ��
******************************************************************************/
#pragma once

class Grapics;
class ResourceManager;
class GameEngine : public Singleton<GameEngine>
{
	friend Singleton;
private:
	GameEngine(const GameEngine&) = delete;
	GameEngine(const GameEngine&&) = delete;
	GameEngine& operator = (const GameEngine&) = delete;
	GameEngine& operator = (const GameEngine&&) = delete;
	GameEngine();
	~GameEngine();

	Grapics* m_pGrapics;
	ResourceManager* m_pResourceManager;
	
public:
	HRESULT Init();
	void Destroy();

	//���� �׷��� ��ü����
	 Grapics*const GetGrapics() { return m_pGrapics; }
	//���� ���ҽ��Ŵ��� ��ü ����
	 ResourceManager*const GetResourceManager() {return m_pResourceManager;}
};
