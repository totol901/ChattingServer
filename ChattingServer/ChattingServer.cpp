// ChattingServer.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"

#define BUFSIZE 1024

typedef struct
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct
{
	OVERLAPPED overlapped;
	char buffer[BUFSIZE];
	WSABUF wsaBuf;
} PER_IO_DATA, *LPPER_IO_DATA;

unsigned int WINAPI CompletionThread(LPVOID pComPort);

int main()
{
	WSAWINSOCK;

	HANDLE hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	for (int i = 0; i < (int)SystemInfo.dwNumberOfProcessors; i++)
	{
		_beginthreadex(NULL, 0,
			CompletionThread,
			(LPVOID)hCompletionPort
			, 0,NULL);
	}

	SOCKET hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	servAddr.sin_port = htons(9000);

	bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
	listen(hServSock, 5);

	LPPER_IO_DATA perIoData;
	LPPER_HANDLE_DATA PerHandleData;

	int RecvBytes;
	int  Flags;

	while (TRUE)
	{
		SOCKADDR_IN clntAddr;
		int addrLen = sizeof(clntAddr);

		SOCKET hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &addrLen);

		PerHandleData = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
		PerHandleData->hClntSock = hClntSock;
		memcpy(&(PerHandleData->clntAddr), &clntAddr, addrLen);

		CreateIoCompletionPort((HANDLE)hClntSock, hCompletionPort, (ULONG_PTR)PerHandleData, 0);

		perIoData = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));

		perIoData->wsaBuf.len = BUFSIZE;
		perIoData->wsaBuf.buf = perIoData->buffer;
		Flags = 0;

		WSARecv(PerHandleData->hClntSock, &(perIoData->wsaBuf), 1,
			(LPDWORD)&RecvBytes, (LPDWORD)&Flags, &(perIoData->overlapped), NULL);
	}

	WSAWINSOCK->DestroyInstance();

    return 0;
}

unsigned int WINAPI CompletionThread(LPVOID pComPort)
{
	HANDLE hCompletionPort = (HANDLE)pComPort;
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_DATA PerIoData;
	DWORD flags;
	DWORD sendBytes = 0;

	while (1)
	{
		GetQueuedCompletionStatus(hCompletionPort, &BytesTransferred,
			(PULONG_PTR)&PerHandleData,
			(LPOVERLAPPED*)&PerIoData,
			INFINITE);

		if (BytesTransferred == 0)
		{
			closesocket(PerHandleData->hClntSock);
			free(PerHandleData);
			free(PerIoData);
			continue;
		}

		PerIoData->wsaBuf.buf[BytesTransferred] = '\0';
		printf("Recv[%s]\n", PerIoData->wsaBuf.buf);

		T_PACKET* p = (T_PACKET*)PerIoData->buffer;

		PerIoData->wsaBuf.len = BytesTransferred;

		WSASend(PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, 
			&sendBytes, 0, NULL, NULL);
		cout << WSAGetLastError() << endl;

		memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
		PerIoData->wsaBuf.len = BUFSIZE;
		PerIoData->wsaBuf.buf = PerIoData->buffer;

		flags = 0;
		
		WSARecv(PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1,
			NULL, &flags, &(PerIoData->overlapped), NULL);
	}


	return 0;
}
