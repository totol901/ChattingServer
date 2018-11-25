/******************************************************************************
파일명	: GameEngine.h
목적		: 싱글턴으로서 그래픽, 리소스에 접근하는 문 역할을 함
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

	//엔진 그래픽 객체리턴
	 Grapics*const GetGrapics() { return m_pGrapics; }
	//엔진 리소스매니저 객체 리턴
	 ResourceManager*const GetResourceManager() {return m_pResourceManager;}
};
