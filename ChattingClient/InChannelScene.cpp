#include "stdafx.h"
#include "InChannelScene.h"

InChannelScene::InChannelScene()
	:BaseScene()
{
}

InChannelScene::InChannelScene(UINT id)
	:BaseScene(id)
{
}

InChannelScene::~InChannelScene()
{
}

void InChannelScene::Init()
{
	cout << endl;
	cout << "------채널-----" << endl;
}

void InChannelScene::Update()
{
	m_SendStream.clear();
	int selectNum = 0;
	bool roopOut = false;

	printf("1. 채널에 메시지 보내기\n");
	printf("2. 채널 나가기\n");
	printf("3. 종료.\n");

	cout << "숫자 입력 : ";
	cin >> selectNum;

	switch (selectNum)
	{
	case 1:
	{
		char message[64] = { 0, };
		cout << "채널에 보낼 메시지 입력 : ";
		cin >> message;
		
		//채널에 메시지 송신
		T_PACKET packet(PK_REQ_CHANNAL_SEND_MESSAGE);
		packet.Size = sizeof(T_PACKET);
		m_SendStream.write(message, sizeof(message));

		packet.SetStream(m_SendStream);
		CLIENTNETWORK->GetServerSession()->SendPacket(packet);
	}
	break;

	case 2:
	{
		cout << "채널 나가기";

		T_PACKET packet(PK_REQ_CHANNAL_OUT);
		packet.Size = sizeof(T_PACKET);
		packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(packet);
	}
	break;

	case 3:
	{
		cout << "종료하기";

		//종료
		T_PACKET packet(PK_REQ_EXIT);
		packet.Size = sizeof(T_PACKET);
		packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(packet);
	}
	break;

	default:
	{
		cout << "숫자를 다시 입력하시오" << endl;
		roopOut = true;
	}
	}

	WaitForRecvPacket();
	cin.ignore();
}
