#pragma once


namespace ServerEngine
{
	namespace Util
	{
		//sock�� (str + arg)�� ���ۿ� ��� ����
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
		�Լ���	: connectSMTP
		����		: SMTP�� ����
		���ϰ�	: bool
		�Ű�����	: SOCKET*
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

			//serverAddr �ּҿ� ������ ���� ������ ������
			if (::connect(*sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr))
				== SOCKET_ERROR)
			{
				return false;
			}
			return true;
		}

		/****************************************************************************
		�Լ���	: sendMail
		����		: to���� ������ ������
		���ϰ�	: bool
		�Ű�����	: const char*, const char*, const char*, const char*
		*****************************************************************************/
		inline bool sendMail(const char *from, const char *to, const char *subject, const char *body)
		{
			SOCKET sock;
			if (!connectSMTP(&sock))
			{
				SLogPrint(L"! smtp mail port connect fail, mail :[%S] body => %S", subject, body);
				return false;
			}

			//SMTP�� ���� �۽�
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

			//�۽� ����
			closesocket(sock);
			SLogPrint(L"* send mail [%S]", subject);

			return true;
		}
	}
}