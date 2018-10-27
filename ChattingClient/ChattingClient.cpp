// ChattingClient.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"

int main()
{
	char*	ServerIp = (char*)"127.0.0.1";
	u_short	ServerPort = 9000;

	ClientNetwork network(ServerIp, ServerPort);

	char message[1024] = { 0, };
	while (1)
	{
		printf("전송할데이터(종료를원할시exit)\n");

		scanf_s("%s", message, 1024);

		if (!strcmp(message, "exit"))
		{
			break;
		}
		network.GetServerSession()->sendPacket(message);
	}
	
    return 0;
}
