// ChattingClient.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"

int main()
{
	char*	ServerIp = (char*)"127.0.0.1";
	u_short	ServerPort = 9000;

	CLIENTNETWORK->Init(ServerIp, ServerPort);

	char message[1024] = { 0, };
	
	while (1)
	{
		printf("전송할데이터(종료를원할시exit)\n");

		cin >> message;

		if (!strcmp(message, "exit"))
		{
			break;
		}
	
		T_PACKET packet(PK_NONE);
		packet.Size = sizeof(T_PACKET);
		memcpy_s(packet.buff, (PAKCET_BUFF_SIZE),
			message, strlen(message)+1);
		//cout << packet.buff << endl;
		CLIENTNETWORK->GetServerSession()->SendPacket(packet);
	}

    return 0;
}
