#include "stdafx.h"
#include "InChannelScene.h"

InChannelScene::InChannelScene()
	:BaseScene()
{
}

//기존 아이디 접속중이면 접속 못하게 처리
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
	cout << "------채널-----" << endl;
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

		m_SendStream.write(message);

		//채널에 메시지 송신
		m_Packet.Clear();
		m_Packet.type = PK_REQ_CHANNAL_SEND_MESSAGE;
		m_Packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
		return;
	}
	break;

	case 2:
	{
		cout << "채널 나가기"<<endl;

		m_Packet.Clear();
		m_Packet.type = PK_REQ_CHANNAL_OUT;
		m_Packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
	}
	break;

	case 3:
	{
		cout << "종료하기"<<endl;

		//종료
		m_Packet.Clear();
		m_Packet.type = PK_REQ_EXIT;
		m_Packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
		//보내는 스트림 종료
		shutdown(CLIENTNETWORK->GetServerSession()->GetSocket(), SD_SEND);
	}
	break;

	default:
	{
		cout << "숫자를 다시 입력하시오" << endl;
		roopOut = true;
	}
	}

	if (CLIENTNETWORK->IsOn())
	{

		WaitForRecvPacket();

	}
	cin.ignore();
}
