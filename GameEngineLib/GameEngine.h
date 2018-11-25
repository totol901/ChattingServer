/******************************************************************************
���ϸ�	: GameEngine.h
����		: �̱������μ� �׷���, ���ҽ��� �����ϴ� �� ������ ��
******************************************************************************/
#pragma once

class Grapics;
class ResourceManager;
class GameEngine : public EngineSingleton<GameEngine>
{
	friend EngineSingleton;
private:
	GameEngine(const GameEngine&) = delete;
	GameEngine(const GameEngine&&) = delete;
	GameEngine& operator = (const GameEngine&) = delete;
	GameEngine& operator = (const GameEngine&&) = delete;
	GameEngine();
	~GameEngine();

	Grapics* m_pGrapics;
	ResourceManager* m_pResourceManager;

	void UseLFH();
	void SetWinDIP(int winWidth, int winHeight);
	
public:
	HRESULT Init(bool useLFH, int winWidth, int winHeight);
	void Destroy();

	//���� �׷��� ��ü����
	 Grapics*const GetGrapics() { return m_pGrapics; }
	//���� ���ҽ��Ŵ��� ��ü ����
	 ResourceManager*const GetResourceManager() {return m_pResourceManager;}
};
