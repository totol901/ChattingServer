// ChattingClient.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"

int main()
{
	printf("--채팅 클라이언트--\n");

	const char*	ServerIp = "127.0.0.1";
	u_short	ServerPort = 9000;

	SendStream SendStream;

	//클라이언트 네트워크 선언 및 초기화
	CLIENTNETWORK->Init((char*)ServerIp, ServerPort);
	
	//신 만들고 신매니저에 넣어줌
	LoginScene* loginScene = new LoginScene(LOGIN);
	WaittingChannelScene* waittingChannelScene = new WaittingChannelScene(WAITTING_CHANNEL);
	InChannelScene* inChannelScene = new InChannelScene(IN_CHANNEL);
	SCENEAMANGER->InsertScene(loginScene);
	SCENEAMANGER->InsertScene(waittingChannelScene);
	SCENEAMANGER->InsertScene(inChannelScene);
	SCENEAMANGER->ChangeCurrentScene(LOGIN);
	
	

	while (1)
	{
		BaseScene* temp = SCENEAMANGER->GetCurrentScene();
		SCENEAMANGER->GetCurrentScene()->Update();
	}

	
    return 0;
}
