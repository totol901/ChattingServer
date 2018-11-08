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
	cout << "------ä�� ����-----" << endl;
}

void WaittingChannelScene::Update()
{
	m_SendStream.clear();
	int selectNum = 0;
	bool roopOut = false;

	printf("1. ä�� ����Ʈ ���\n");
	printf("2. ä�� ����\n");
	printf("3. ä�� ����\n");
	printf("4. ����.\n");

	cout << "���� �Է� : ";
	cin >> selectNum;

	switch (selectNum)
	{
	case 1:
	{
		//ä�� ����Ʈ �޾ƿ�
		T_PACKET packet(PK_REQ_WAITINGCHANNAL_ENTER);
		packet.Size = sizeof(T_PACKET);

		CLIENTNETWORK->GetServerSession()->SendPacket(packet);
	}
	break;

	case 2:
	{
		m_SendStream.clear();
		char channelName[15];
		cout << "���� channelName �Է� : ";
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

		cout << "������ channel ���� �Է� : ";
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
		cout << "�����ϱ�";

		T_PACKET packet(PK_REQ_EXIT);
		packet.Size = sizeof(T_PACKET);
		packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(packet);
	}
		break;

	default:
		cout << "���ڸ� �ٽ� �Է��Ͻÿ�" << endl << endl;
		roopOut = true;
		break;
	}

	//Scene�� ����ɶ� ���� ���
	WaitForRecvPacket();
	cin.ignore();
}
