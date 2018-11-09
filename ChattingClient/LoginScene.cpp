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
	cout << "------�α��� â-----" << endl;
}

void LoginScene::Update()
{
	m_SendStream.clear();
	int selectNum = 0;
	bool roopOut = false;

	printf("1. ���� ���̵�� �α���.\n");
	printf("2. ���̵� ����.\n");
	printf("3. ����.\n");

	cout << "���� �Է� : ";
	cin >> selectNum;
	 
	switch (selectNum)
	{
		case 1://���� ���̵�� �α���
		{
			//ID, PW �Է¹���
			char id[15];
			char pw[15];
			cout << "ID �Է� : ";
			cin >> id;
			cout << "PW �Է� : ";
			cin >> pw;

			//stream�� ������ ����
			m_SendStream.write(id);
			m_SendStream.write(pw);

			//�Է� ���� ������ �۽�
			m_Packet.Clear();
			m_Packet.type = PK_REQ_LOGIN;
			m_Packet.SetStream(m_SendStream);

			//��Ŷ ����
			CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
		}
			break;

		case 2://���̵� ����
		{
			//ID, PW, nickname �Է¹���
			char id[15];
			char pw[15];
			char nickname[15];
			cout << "ID �Է� : ";
			cin >> id;
			cout << "PW �Է� : ";
			cin >> pw;
			cout << "nickname �Է� : ";
			cin >> nickname;

			m_SendStream.write(id);
			m_SendStream.write(pw);
			m_SendStream.write(nickname);

			//�Է� ���� ������ �۽�
			m_Packet.Clear();
			m_Packet.type = PK_REQ_CREATE_ID;
			m_Packet.SetStream(m_SendStream);

			//��Ŷ ����
			CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
		}
			break;

		case 3:
		{
			cout << "�����ϱ�" << endl;
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
			cout << "���ڸ� �ٽ� �Է��Ͻÿ�" << endl << endl;
			roopOut = true;
			break;
	}

	if (CLIENTNETWORK->IsOn())
	{
		WaitForRecvPacket();
	}

	cin.ignore();
}
