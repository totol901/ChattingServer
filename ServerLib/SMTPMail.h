#pragma once


namespace ServerEngine
{
	namespace Util
	{
		//sock에 (str + arg)를 버퍼에 담아 보냄
		inline void smtpWriteLine(SOCKET sock, const char *str, const char *arg)
		{
			array<char, 4096> buf;
			if (arg != NULL)
			{
				_snprintf_s(buf.data(), buf.size(), _TRUNCATE, str, arg);
			}
			else
			{
				_snprintf_s(buf.data(), buf.size(), _TRUNCATE, str, arg);
			}

			::send(sock, buf.data(), (int)buf.size(), 0);
		}

		/****************************************************************************
		함수명	: connectSMTP
		설명		: SMTP에 접속
		리턴값	: bool
		매개변수	: SOCKET*
		*****************************************************************************/
		inline bool connectSMTP(SOCKET *sock)
		{
			*sock = ::socket(AF_INET, SOCK_STREAM, 0);
			if (*sock == SOCKET_ERROR)
			{
				SLogPrint(L"! mail socket make fail");
				return false;
			}

			const int SMTP_PORT = 25;
			struct sockaddr_in serverAddr;
			serverAddr.sin_family = AF_INET;
			serverAddr.sin_port = htons((u_short)SMTP_PORT);
			inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr));

			//serverAddr 주소에 소켓을 보내 서버와 연결함
			if (::connect(*sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr))
				== SOCKET_ERROR)
			{
				return false;
			}
			return true;
		}

		/****************************************************************************
		함수명	: sendMail
		설명		: to에게 메일을 보내줌
		리턴값	: bool
		매개변수	: const char*, const char*, const char*, const char*
		*****************************************************************************/
		inline bool sendMail(const char *from, const char *to, const char *subject, const char *body)
		{
			SOCKET sock;
			if (!connectSMTP(&sock))
			{
				SLogPrint(L"! smtp mail port connect fail, mail :[%S] body => %S", subject, body);
				return false;
			}

			//SMTP에 소켓 송신
			smtpWriteLine(sock, "HELO %s", from);
			smtpWriteLine(sock, "MAIL FROM: %s", from);
			smtpWriteLine(sock, "RCPT TO: %s", to);
			smtpWriteLine(sock, "DATA", NULL);

			smtpWriteLine(sock, "From: %s", from);
			smtpWriteLine(sock, "To: %s", to);
			smtpWriteLine(sock, "Subject: %s", subject);

			smtpWriteLine(sock, "\n", NULL);

			smtpWriteLine(sock, "%s", body);

			smtpWriteLine(sock, ".", NULL);
			smtpWriteLine(sock, "QUIT", NULL);

			//송신 종료
			closesocket(sock);
			SLogPrint(L"* send mail [%S]", subject);

			return true;
		}
	}
}