// ChattingServer.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"

#define BUFSIZE 1024

#define IO_TYPE_SEND 0
#define IO_TYPE_RECV 1

typedef struct
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct
{
	OVERLAPPED overlapped;
	BOOL type;
	char buffer[BUFSIZE];
	WSABUF wsaBuf;
} PER_IO_DATA, *LPPER_IO_DATA;

unsigned int WINAPI CompletionThread(LPVOID pComPort);

void IOCP_Send(PER_HANDLE_DATA* pSocket, const char* pBuf)
{
	int nError = 0;
	DWORD dwSend = 0;

	LPPER_IO_DATA pIO = new PER_IO_DATA;
	ZeroMemory(pIO, sizeof(PER_IO_DATA));
	pIO->wsaBuf.buf = pIO->buffer;
	strcpy_s(pIO->wsaBuf.buf, BUFSIZE, pBuf);
	pIO->wsaBuf.len = sizeof(pBuf) + 1;
	pIO->type = IO_TYPE_SEND;

	nError = WSASend(pSocket->hClntSock, &pIO->wsaBuf, 1, &dwSend, 0, &pIO->overlapped, NULL);
	if (nError == 0)
	{
		//한번에 보내기 성공
	}
	else
	{
		if (nError == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING)
		{
			//IOCP I/O 보내기 요청 성공
		}
		else
		{
			//실패시
			closesocket(pSocket->hClntSock);
			SAFE_DELETE(pSocket);
			SAFE_DELETE(pIO);
		}
	}
}

void IOCP_Recv(PER_HANDLE_DATA* pSocket)
{
	int nError = 0;
	DWORD dwRecv, dwFlags = 0;

	LPPER_IO_DATA pIO = new PER_IO_DATA;
	ZeroMemory(pIO, sizeof(PER_IO_DATA));
	pIO->wsaBuf.buf = pIO->buffer;
	pIO->wsaBuf.len = BUFSIZE;
	pIO->type = IO_TYPE_RECV;

	nError = WSARecv(pSocket->hClntSock, &pIO->wsaBuf, 1, &dwRecv, &dwFlags, &pIO->overlapped, NULL);
	if (nError == 0)
	{
		//한번에 받기 성공
	}
	else
	{
		if (nError == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING)
		{
			//IOCP I/O 받기 요청 성공
		}
		else
		{
			//실패시
			closesocket(pSocket->hClntSock);
			SAFE_DELETE(pSocket);
			SAFE_DELETE(pIO);
		}
	}
}

int main()
{
	WSAWINSOCK;

	//리슨 
	SOCKET hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	servAddr.sin_port = htons(9000);

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		closesocket(hServSock);
		return -1;
	}
	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		closesocket(hServSock);
		return -1;
	}

	//IOCP 생성
	HANDLE hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hCompletionPort == NULL)
	{
		cout << "CreateIoCompletionPort() error code : " << GetLastError() << endl;
	}

	//IOCP에 쓰일 스레드 생성
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	for (int i = 0; i < (int)SystemInfo.dwNumberOfProcessors * 2; i++)
	{
		_beginthreadex(NULL, 0, CompletionThread, (LPVOID)hCompletionPort, 0, NULL);
	}

	LPPER_IO_DATA perIoData = nullptr;
	LPPER_HANDLE_DATA PerHandleData = nullptr;

	int RecvBytes = 0;
	int Flags = 0;

	//accept한 소켓 -> 클라이언트 소켓, IOCP로 처리
	//리슨 소켓 / 클라이언트 소켓 둘이 나눌수 있을듯 하다
	while (TRUE)
	{
		SOCKADDR_IN clntAddr;
		int addrLen = sizeof(clntAddr);

		SOCKET hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &addrLen);

		PerHandleData = new PER_HANDLE_DATA;
		PerHandleData->hClntSock = hClntSock;
		memcpy(&(PerHandleData->clntAddr), &clntAddr, addrLen);

		//억셉트된 소켓을 IOCP에 참가시킴
		CreateIoCompletionPort((HANDLE)hClntSock, hCompletionPort, (ULONG_PTR)PerHandleData, 0);

		//perIoData = new PER_IO_DATA;
		//memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));
		//
		//perIoData->wsaBuf.len = BUFSIZE;
		//perIoData->wsaBuf.buf = perIoData->buffer;
		//Flags = 0;
		//
		////대기 상태의 스레드를 깨우기 위함
		//WSARecv(PerHandleData->hClntSock, &(perIoData->wsaBuf), 1,
		//	(LPDWORD)&RecvBytes, (LPDWORD)&Flags, &(perIoData->overlapped), NULL);

		IOCP_Recv(PerHandleData);
	}

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

	while (1)
	{
		//스레드 대기 큐로 이동시킴[대기상태로] -> WSARecv가 완료되면 대기 큐에서 빠져나와 릴리즈 큐로 들어가며 return함
		BOOL bIsOk = GetQueuedCompletionStatus(hCompletionPort, &BytesTransferred,
			(PULONG_PTR)&PerHandleData,
			(LPOVERLAPPED*)&PerIoData,
			INFINITE);
		//cout << "GetQueuedCompletionStatus 리턴" << endl;

		//if (!bIsOk)
		//{
		//	dwErrCode = GetLastError();
		//
		//	//CancelIoEx를 호출 했을 경우 PerIoDate에 포인터가 들어옴
		//	if (PerIoData != NULL)
		//	{
		//		//CloseHandle로 IO취소됨
		//		if (dwErrCode == ERROR_OPERATION_ABORTED)
		//		{
		//			cout << "유저로 부터 IO 취소됨" << endl;
		//			//IOCP 계속 진행 가능
		//		}
		//		//IO 과정에 문제가 발생함!
		//		else
		//		{
		//			//해당 소켓 감시 종료를 위해 해당 소켓을 닫고 IOCP 대기상태로
		//			cout << "GQCS Error: " << dwErrCode << endl;
		//			closesocket(PerHandleData->hClntSock);
		//			SAFE_DELETE(PerHandleData);
		//			SAFE_DELETE(PerIoData);
		//			continue;
		//		}
		//	}
		//	//IOCP 핸들이 닫겼거나, IOCP에 오류가 있을 경우
		//	else
		//	{
		//		//만약 핸들 닫긴게 아니라면 오류 출력
		//		if (dwErrCode != ERROR_ABANDONED_WAIT_0)
		//		{
		//			cout << "GQCS Error : " << dwErrCode << endl;
		//		}
		//		break;
		//	}
		//}
		//
		//if (dwErrCode == ERROR_SUCCESS)
		//{
		//	//소켓 핸들을 닫았다면 GQCS는 TRUE, Internaldms 0이 아님
		//	if (PerIoData->overlapped.Internal != 0)
		//	{
		//		DWORD dwErrCode = WSAGetLastError();
		//		if (dwErrCode == WSAEINTR)
		//		{
		//			cout << "ClientSocket 유저가 닫음" << endl;
		//		}
		//		//IO과정에서 문제 발생 감시종료하기 위해 핸들 종료
		//		else
		//		{
		//			cout << "WSAERROR : " << dwErrCode << endl;
		//			closesocket(PerHandleData->hClntSock);
		//			SAFE_DELETE(PerHandleData);
		//			SAFE_DELETE(PerIoData);
		//		}
		//		//두 경우다 비동기 함수 호출하면 안됨 대기상태로
		//		continue;
		//	}
		//
		//	//소켓 연결 끊김
		//	if (BytesTransferred == 0)
		//	{
		//		if (PerIoData)
		//		{
		//			closesocket(PerHandleData->hClntSock);
		//			SAFE_DELETE(PerHandleData);
		//			SAFE_DELETE(PerIoData);
		//		}
		//		continue;
		//	}
		//
		//	//key가 0일경우 종료를 뜻함
		//	if (PerHandleData == 0)
		//	{
		//		break;
		//	}
		//}

		//소켓 연결 끊김
		if (BytesTransferred == 0)
		{
			if (PerIoData)
			{
				closesocket(PerHandleData->hClntSock);
				SAFE_DELETE(PerHandleData);
				SAFE_DELETE(PerIoData);
			}
			continue;
		}
		
		//key가 0일경우 종료를 뜻함
		if (PerHandleData == 0)
		{
			break;
		}

		//
		if (PerIoData)
		{
			//받은 데이터가 있음
			if (PerIoData->type == IO_TYPE_RECV)
			{
				cout << PerIoData->buffer << endl;
				IOCP_Recv(PerHandleData);
				IOCP_Send(PerHandleData, "world");
			}
			//보낸 데이터가 있음
			else if(PerIoData->type == IO_TYPE_SEND)
			{
				//처리할 일이 있으면 처리함
			}

			//IO를 삭제함
			SAFE_DELETE(PerIoData);
		}
	}

	return 0;
}
