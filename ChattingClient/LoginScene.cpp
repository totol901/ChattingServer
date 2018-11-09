#include "stdafx.h"
#include "LoginScene.h"

LoginScene::LoginScene()
	:BaseScene()
{
}

LoginScene::LoginScene(UINT id)
	: BaseScene(id)
{
}

LoginScene::~LoginScene()
{
}

void LoginScene::Init()
{
	cout << endl;
	cout << "------로그인 창-----" << endl;
}

void LoginScene::Update()
{
	m_SendStream.clear();
	int selectNum = 0;
	bool roopOut = false;

	printf("1. 기존 아이디로 로그인.\n");
	printf("2. 아이디 생성.\n");
	printf("3. 종료.\n");

	cout << "숫자 입력 : ";
	cin >> selectNum;
	 
	switch (selectNum)
	{
		case 1://기존 아이디로 로그인
		{
			//ID, PW 입력받음
			char id[15];
			char pw[15];
			cout << "ID 입력 : ";
			cin >> id;
			cout << "PW 입력 : ";
			cin >> pw;

			//stream에 데이터 넣음
			m_SendStream.write(id);
			m_SendStream.write(pw);

			//입력 받은 데이터 송신
			m_Packet.Clear();
			m_Packet.type = PK_REQ_LOGIN;
			m_Packet.SetStream(m_SendStream);

			//패킷 전송
			CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
		}
			break;

		case 2://아이디 생성
		{
			//ID, PW, nickname 입력받음
			char id[15];
			char pw[15];
			char nickname[15];
			cout << "ID 입력 : ";
			cin >> id;
			cout << "PW 입력 : ";
			cin >> pw;
			cout << "nickname 입력 : ";
			cin >> nickname;

			m_SendStream.write(id);
			m_SendStream.write(pw);
			m_SendStream.write(nickname);

			//입력 받은 데이터 송신
			m_Packet.Clear();
			m_Packet.type = PK_REQ_CREATE_ID;
			m_Packet.SetStream(m_SendStream);

			//패킷 전송
			CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
		}
			break;

		case 3:
		{
			cout << "종료하기" << endl;
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
			cout << "숫자를 다시 입력하시오" << endl << endl;
			roopOut = true;
			break;
	}

	if (CLIENTNETWORK->IsOn())
	{
		WaitForRecvPacket();
	}

	cin.ignore();
}
