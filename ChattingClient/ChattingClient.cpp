// ChattingClient.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
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

	HANDLE hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	//IOCP에 쓰일 스레드 생성
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	HANDLE hThread[100];
	for (int i = 0; i < (int)SystemInfo.dwNumberOfProcessors * 2; i++)
	{
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, CompletionThread, (LPVOID)hCompletionPort, 0, NULL);
	}

	CreateIoCompletionPort((HANDLE)hSocket, hCompletionPort, (ULONG_PTR)&hSocket, 0);
	
	LPPER_IO_DATA pIO = new PER_IO_DATA;
	while (1)
	{
		int nError = 0;
		DWORD dwSend, dwFlags = 0;

		
		ZeroMemory(pIO, sizeof(PER_IO_DATA));

		cin >> pIO->buffer;

		pIO->wsaBuf.buf = pIO->buffer;
		pIO->wsaBuf.len = sizeof(pIO->buffer) + 1;


		WSASend(hSocket, &pIO->wsaBuf, 1, &dwSend, 0, &pIO->overlapped, NULL);
	}
	SAFE_DELETE(pIO);

	WaitForMultipleObjects(SystemInfo.dwNumberOfProcessors * 2, hThread, TRUE, INFINITE);

	closesocket(hSocket);

	WSAWINSOCK->DestroyInstance();

    return 0;
}

unsigned int WINAPI CompletionThread(LPVOID pComPort)
{
	DWORD dwErrCode = ERROR_SUCCESS;
	HANDLE hCompletionPort = (HANDLE)pComPort;
	DWORD BytesTransferred = 0;
	LPPER_HANDLE_DATA PerHandleData = nullptr;
	LPPER_IO_DATA PerIoData = nullptr;
	DWORD flags = 0;

	LPPER_IO_DATA pIO = new PER_IO_DATA;

	while (1)
	{
		int nError = 0;
		DWORD dwRecv, dwFlags = 0;

		BOOL bIsOk = GetQueuedCompletionStatus(hCompletionPort, &BytesTransferred,
			(PULONG_PTR)&PerHandleData,
			(LPOVERLAPPED*)&PerIoData,
			INFINITE);

		
		ZeroMemory(pIO, sizeof(PER_IO_DATA));
		pIO->wsaBuf.buf = pIO->buffer;
		pIO->wsaBuf.len = BUFSIZE;


		WSARecv(PerHandleData->hClntSock, &pIO->wsaBuf, 1, &dwRecv, &dwFlags, &pIO->overlapped, NULL);
	}

	SAFE_DELETE(pIO);

	return 0;
}
