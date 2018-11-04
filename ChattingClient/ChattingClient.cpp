// ChattingClient.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"

int main()
{
	char*	ServerIp = (char*)"127.0.0.1";
	u_short	ServerPort = 9000;

	CLIENTNETWORK->Init(ServerIp, ServerPort);
	
	printf("--채팅 클라이언트--\n");
	int selectNum = 0;
	//로그인 루프
	while (1)
	{
		printf("1. 기존 아이디로 로그인.\n");
		printf("2. 아이디 생성.\n");
		printf("3. 종료.\n");
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

			//입력 받은 데이터 송신
			T_PACKET packet(PK_REQ_LOGIN);
			packet.Size = sizeof(T_PACKET);
			memcpy(packet.buff, id, sizeof(id));
			memcpy(packet.buff + sizeof(id), pw, sizeof(pw));

			//패킷 전송
			CLIENTNETWORK->GetServerSession()->SendPacket(packet);
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

			//입력 받은 데이터 송신
			T_PACKET packet(PK_REQ_CREATE_ID);
			packet.Size = sizeof(T_PACKET);
			int index = 0;
			memcpy(packet.buff + index, id, sizeof(id));
			index += sizeof(id);
			memcpy(packet.buff + index, pw, sizeof(pw));
			index += sizeof(pw);
			memcpy(packet.buff + index,
				nickname, sizeof(nickname));

			//패킷 전송
			CLIENTNETWORK->GetServerSession()->SendPacket(packet);
		}
			break;

		case 3:
			return 0;

		default:
			cout << "숫자를 잘못 입력하였습니다." << endl;
			continue;
		}

		//
	}
	
    return 0;
}
