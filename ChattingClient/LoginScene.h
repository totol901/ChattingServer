#pragma once

enum E_LOGIN_ERROR
{
	LOGIN_ERROR_NONE,
	LOGIN_ERROR_WRONG_ID,
	LOGIN_ERROR_WRONG_PW,
	LOGIN_ERROR_CREATEID_ID_ALREADY_EXE,
	LOGIN_ERROR_CREATEID_NICKNAME_ALREADY_EXE
};

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
