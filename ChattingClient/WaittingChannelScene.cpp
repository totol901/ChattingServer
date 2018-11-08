#include "stdafx.h"
#include "WaittingChannelScene.h"

WaittingChannelScene::WaittingChannelScene()
	:BaseScene()
{
}

WaittingChannelScene::WaittingChannelScene(UINT id)
	:BaseScene(id)
{
}

WaittingChannelScene::~WaittingChannelScene()
{
}

void WaittingChannelScene::Init()
{
	cout << endl;
	cout << "------채널 대기방-----" << endl;
}

void WaittingChannelScene::Update()
{
	m_SendStream.clear();
	int selectNum = 0;
	bool roopOut = false;

	printf("1. 채널 리스트 출력\n");
	printf("2. 채널 생성\n");
	printf("3. 채널 입장\n");
	printf("4. 종료.\n");

	cout << "숫자 입력 : ";
	cin >> selectNum;

	switch (selectNum)
	{
	case 1:
	{
		//채널 리스트 받아옴
		T_PACKET packet(PK_REQ_WAITINGCHANNAL_ENTER);
		packet.Size = sizeof(T_PACKET);

		CLIENTNETWORK->GetServerSession()->SendPacket(packet);
	}
	break;

	case 2:
	{
		m_SendStream.clear();
		char channelName[15];
		cout << "만들 channelName 입력 : ";
		cin >> channelName;

		T_PACKET packet(PK_REQ_WAITINGCHANNAL_CHREAT_CHANNAL);
		packet.Size = sizeof(T_PACKET);
		m_SendStream.write(channelName, sizeof(channelName));
		packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(packet);
	}
	break;

	case 3:
	{
		UINT channelNum = 0;
		char channelNumStr[15];

		cout << "입장할 channel 숫자 입력 : ";
		cin >> channelNumStr;
		channelNum = atoi(channelNumStr);

		T_PACKET packet(PK_REQ_WAITINGCHANNAL_CHANNAL_JOIN);
		packet.Size = sizeof(T_PACKET);
		m_SendStream.write(&channelNum, sizeof(channelNum));
		packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(packet);
	}
	break;

	case 4:
	{
		cout << "종료하기";

		T_PACKET packet(PK_REQ_EXIT);
		packet.Size = sizeof(T_PACKET);
		packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(packet);
	}
		break;

	default:
		cout << "숫자를 다시 입력하시오" << endl << endl;
		roopOut = true;
		break;
	}

	//Scene이 변경될때 까지 대기
	WaitForRecvPacket();
	cin.ignore();
}
