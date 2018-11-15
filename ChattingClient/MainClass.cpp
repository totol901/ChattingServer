#include "stdafx.h"
#include "MainClass.h"

MainClass::MainClass()
	:isOn(true)
{
	printf("--채팅 클라이언트--\n");


	//TODO : 커맨드 창에서 직접 입력받고 서버 접속으로변환 
	//패스워드 처리 암호화
	const char*	ServerIp = "192.168.0.2";
	u_short	ServerPort = 9000;

	//클라이언트 네트워크 선언 및 초기화
	CLIENTNETWORK->Init((char*)ServerIp, ServerPort);
	CLIENTNETWORK->SetLinkIsOn(&isOn);

	//신 만들고 신매니저에 넣어줌
	LoginScene* loginScene = new LoginScene(LOGIN);
	WaittingChannelScene* waittingChannelScene = new WaittingChannelScene(WAITTING_CHANNEL);
	InChannelScene* inChannelScene = new InChannelScene(IN_CHANNEL);
	SCENEAMANGER->InsertScene(loginScene);
	SCENEAMANGER->InsertScene(waittingChannelScene);
	SCENEAMANGER->InsertScene(inChannelScene);
	SCENEAMANGER->ChangeCurrentScene(LOGIN);
}

MainClass::~MainClass()
{
}

void MainClass::Update()
{
	//신 업데이트
	while (isOn)
	{
		SCENEAMANGER->GetCurrentScene()->Update();
	}
}
