#include "stdafx.h"
#include "MainClass.h"

MainClass::MainClass()
	:isOn(true)
{
	printf("--ä�� Ŭ���̾�Ʈ--\n");


	//TODO : Ŀ�ǵ� â���� ���� �Է¹ް� ���� �������κ�ȯ 
	//�н����� ó�� ��ȣȭ
	const char*	ServerIp = "192.168.0.2";
	u_short	ServerPort = 9000;

	//Ŭ���̾�Ʈ ��Ʈ��ũ ���� �� �ʱ�ȭ
	CLIENTNETWORK->Init((char*)ServerIp, ServerPort);
	CLIENTNETWORK->SetLinkIsOn(&isOn);

	//�� ����� �ŸŴ����� �־���
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
	//�� ������Ʈ
	while (isOn)
	{
		SCENEAMANGER->GetCurrentScene()->Update();
	}
}
