/******************************************************************************
파일명	: ServerSessionParser.h
목적		: 서버 세션 객체의 파싱 객체
******************************************************************************/
#pragma once

class IOCPServerSession;
class ServerSessionParser
{
private:
	IOCPServerSession * m_ClientSession;

	RecvStream recvStream;
	SendStream SendStream;

	UINode* m_pErrorUI;	//서버에서 보낸 오류 띄울 UI
	StaticTextUI* m_pStaticTextUI;

	void ErrorOK();

	void SetRecvStream(T_PACKET* packet);

	//로그인 답장
	void AnsLogin(T_PACKET* packet);
	//아이디 생성 답장
	void AnsCreateId(T_PACKET* packet);
	//웨이팅 채널 답장
	void AnsWaitingChannalEnter(T_PACKET* packet);
	//웨이팅 채널에서 채널 생성 답장
	void AnsWaitingChannelCreateChannel(T_PACKET* packet);
	//웨이팅 채널에서 채널 입장 답장
	void AnsWaitingChannelJoin(T_PACKET* packet);
	//채널로 부터 받은 메시지 처리
	void RecvChannelMessage(T_PACKET* packet);
	//채널 나가기 답장
	void AnsChannelOut(T_PACKET* packet);
	//나가기 답장
	void AnsExit(T_PACKET* packet);

public:
	ServerSessionParser(IOCPServerSession* ServerSession);
	~ServerSessionParser();

	void Init();

	/****************************************************************************
	함수명	: PacketParsing
	설명		: 들어온 페킷을 type에 따라 분류하여 buff를 파싱함
	*****************************************************************************/
	bool PacketParsing(T_PACKET* const packet);

	/****************************************************************************
	함수명	: ErrorPrint
	설명		: 서버로 부터 받은 오류를 출력하기 위함
	*****************************************************************************/
	void ErrorPrint(UINT errorNum);
};

