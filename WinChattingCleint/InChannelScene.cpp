#include "stdafx.h"
#include "InChannelScene.h"

InChannelScene::InChannelScene()
	:BaseScene()
{
}

//���� ���̵� �������̸� ���� ���ϰ� ó��
InChannelScene::InChannelScene(UINT id)
	:BaseScene(id)
{
}

InChannelScene::~InChannelScene()
{
}

void InChannelScene::Init()
{
	
}

void InChannelScene::Update()
{
	cout << endl;
	cout << "------ä��-----" << endl;
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

		m_SendStream.write(message);

		//ä�ο� �޽��� �۽�
		m_Packet.Clear();
		m_Packet.type = PK_REQ_CHANNAL_SEND_MESSAGE;
		m_Packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
		return;
	}
	break;

	case 2:
	{
		cout << "ä�� ������"<<endl;

		m_Packet.Clear();
		m_Packet.type = PK_REQ_CHANNAL_OUT;
		m_Packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
	}
	break;

	case 3:
	{
		cout << "�����ϱ�"<<endl;

		//����
		m_Packet.Clear();
		m_Packet.type = PK_REQ_EXIT;
		m_Packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
		//������ ��Ʈ�� ����
		shutdown(CLIENTNETWORK->GetServerSession()->GetSocket(), SD_SEND);
	}
	break;

	default:
	{
		cout << "���ڸ� �ٽ� �Է��Ͻÿ�" << endl;
		roopOut = true;
	}
	}

	if (CLIENTNETWORK->IsOn())
	{

		WaitForRecvPacket();

	}
	cin.ignore();
}
