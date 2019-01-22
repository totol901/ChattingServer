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
			함수명	: connectTo
			설명		: ip, port의 서버에 connect해줌
			리턴값	: bool
			매개변수	: char*, int
			*****************************************************************************/
			bool		ConnectTo(char *ip, int port);

			/****************************************************************************
			함수명	: onRecv
			설명		: 연결된 서버로 부터 페킷 받아옴
			리턴값	: Package*
			매개변수	: size_t
			*****************************************************************************/
			Package*	OnRecv(size_t transferSize);
		};
	}
}
