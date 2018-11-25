/******************************************************************************
파일명	: LoginScene.h
목적		: 로그인씬 처리
******************************************************************************/
#pragma once

class LoginScene : public BaseScene
{
private:

public:
	LoginScene();
	LoginScene(UINT id);
	~LoginScene();

	void Init();
	void Update();
};
