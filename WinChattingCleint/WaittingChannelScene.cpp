#include "stdafx.h"
#include "WaittingChannelScene.h"


WaittingChannelScene::WaittingChannelScene(const WCHAR * nodeName, UINT nodeNum)
	:BaseScene(nodeName, nodeNum)
{
}

WaittingChannelScene::~WaittingChannelScene()
{
}

HRESULT WaittingChannelScene::Init()
{
	return S_OK;
}

void WaittingChannelScene::Update()
{
	cout << endl;
	cout << "------ä�� ����-----" << endl;
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
		m_Packet.Clear();
		m_Packet.type = PK_REQ_WAITINGCHANNAL_ENTER;
		m_Packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
	}
		break;

	case 2:
	{
		m_SendStream.clear();
		char channelName[15];
		cout << "���� channelName �Է� : ";
		cin >> channelName;

		m_SendStream.write(channelName);

		m_Packet.Clear();
		m_Packet.type = PK_REQ_WAITINGCHANNAL_CHREAT_CHANNAL;
		m_Packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
	}
		break;

	case 3:
	{
		UINT channelNum = 0;
		char channelNumStr[15];

		cout << "������ channel ���� �Է� : ";
		cin >> channelNumStr;
		channelNum = atoi(channelNumStr);

		m_SendStream.write(&channelNum, sizeof(channelNum));

		m_Packet.Clear();
		m_Packet.type = PK_REQ_WAITINGCHANNAL_CHANNAL_JOIN;
		m_Packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
	}
	break;

	case 4:
	{
		cout << "�����ϱ�"<<endl;

		m_Packet.Clear();
		m_Packet.type = PK_REQ_EXIT;
		m_Packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
		//������ ��Ʈ�� ����
		shutdown(CLIENTNETWORK->GetServerSession()->GetSocket(), SD_SEND);
	}
		break;

	default:
		cout << "���ڸ� �ٽ� �Է��Ͻÿ�" << endl << endl;
		roopOut = true;
		break;
	}

	//Scene�� ����ɶ� ���� ���
	if (CLIENTNETWORK->IsOn())
	{
		WaitForRecvPacket();
	}
	cin.ignore();
}
