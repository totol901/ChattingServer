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
	cout << "------ä��-----" << endl;
}

void InChannelScene::Update()
{
	m_SendStream.clear();
	int selectNum = 0;
	bool roopOut = false;

	printf("1. ä�ο� �޽��� ������\n");
	printf("2. ä�� ������\n");
	printf("3. ����.\n");

	cout << "���� �Է� : ";
	cin >> selectNum;

	switch (selectNum)
	{
	case 1:
	{
		char message[64] = { 0, };
		cout << "ä�ο� ���� �޽��� �Է� : ";
		cin >> message;
		
		//ä�ο� �޽��� �۽�
		T_PACKET packet(PK_REQ_CHANNAL_SEND_MESSAGE);
		packet.Size = sizeof(T_PACKET);
		m_SendStream.write(message, sizeof(message));

		packet.SetStream(m_SendStream);
		CLIENTNETWORK->GetServerSession()->SendPacket(packet);
	}
	break;

	case 2:
	{
		cout << "ä�� ������";

		T_PACKET packet(PK_REQ_CHANNAL_OUT);
		packet.Size = sizeof(T_PACKET);
		packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(packet);
	}
	break;

	case 3:
	{
		cout << "�����ϱ�";

		//����
		T_PACKET packet(PK_REQ_EXIT);
		packet.Size = sizeof(T_PACKET);
		packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(packet);
	}
	break;

	default:
	{
		cout << "���ڸ� �ٽ� �Է��Ͻÿ�" << endl;
		roopOut = true;
	}
	}

	WaitForRecvPacket();
	cin.ignore();
}
