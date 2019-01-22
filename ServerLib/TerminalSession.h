#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
		class TerminalSession : public Session
		{
		public:
			TerminalSession();
			~TerminalSession();

			/****************************************************************************
			�Լ���	: connectTo
			����		: ip, port�� ������ connect����
			���ϰ�	: bool
			�Ű�����	: char*, int
			*****************************************************************************/
			bool		ConnectTo(char *ip, int port);

			/****************************************************************************
			�Լ���	: onRecv
			����		: ����� ������ ���� ��Ŷ �޾ƿ�
			���ϰ�	: Package*
			�Ű�����	: size_t
			*****************************************************************************/
			Package*	OnRecv(size_t transferSize);
		};
	}
}
