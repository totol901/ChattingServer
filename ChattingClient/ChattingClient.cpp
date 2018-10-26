// ChattingClient.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"


int main()
{
	WSAWINSOCK;

	SOCKET hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (hSocket == INVALID_SOCKET)
	{
		WSAERROR->err_quit("socket()");
	}
	
	SOCKADDR_IN recvAddr;
	memset(&recvAddr, 0, sizeof(recvAddr));
	recvAddr.sin_family = AF_INET; 
	if (inet_pton(AF_INET, "127.0.0.1", &recvAddr.sin_addr.s_addr) != 0)
	{
		cout << WSAGetLastError() << endl;
	}
	recvAddr.sin_port = htons(SERVERPORT);

	if (connect(hSocket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)
	{
		WSAERROR->err_quit("connect()");
	}

	WSAEVENT event = WSACreateEvent();

	WSAOVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));

	overlapped.hEvent = event;

	WSABUF dataBuf;
	char message[1024] = { 0, };
	int sendBytes = 0;
	int recvBytes = 0;
	int flags = 0;

	while (true)
	{
		flags = 0;
		printf("전송할데이터(종료를원할시exit)\n");

		scanf_s("%s", message, 1024);

		if (!strcmp(message, "exit"))
		{
			break;
		}

		dataBuf.len = (ULONG)strlen(message);
		dataBuf.buf = message;

		if (WSASend(hSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				WSAERROR->err_display("WSASend()");
			}
		}

		WSAWaitForMultipleEvents(1, &event, TRUE, WSA_INFINITE, FALSE);

		WSAGetOverlappedResult(hSocket, &overlapped, (LPDWORD)&sendBytes, FALSE, NULL);

		printf("전송된바이트수: %d \n", sendBytes);

		if (WSARecv(hSocket, &dataBuf, 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &overlapped, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				WSAERROR->err_display("WSARecv()");
			}
		}

		printf("Recv[%s]\n", dataBuf.buf);
	}

	closesocket(hSocket);

	WSAWINSOCK->DestroyInstance();

    return 0;
}

