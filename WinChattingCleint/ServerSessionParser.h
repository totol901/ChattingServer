/******************************************************************************
���ϸ�	: ServerSessionParser.h
����		: ���� ���� ��ü�� �Ľ� ��ü
******************************************************************************/
#pragma once

class IOCPServerSession;
class ServerSessionParser
{
private:
	IOCPServerSession * m_ClientSession;

	RecvStream recvStream;
	SendStream SendStream;

	UINode* m_pErrorUI;	//�������� ���� ���� ��� UI
	StaticTextUI* m_pStaticTextUI;

	void ErrorOK();

	void SetRecvStream(T_PACKET* packet);

	//�α��� ����
	void AnsLogin(T_PACKET* packet);
	//���̵� ���� ����
	void AnsCreateId(T_PACKET* packet);
	//������ ä�� ����
	void AnsWaitingChannalEnter(T_PACKET* packet);
	//������ ä�ο��� ä�� ���� ����
	void AnsWaitingChannelCreateChannel(T_PACKET* packet);
	//������ ä�ο��� ä�� ���� ����
	void AnsWaitingChannelJoin(T_PACKET* packet);
	//ä�η� ���� ���� �޽��� ó��
	void RecvChannelMessage(T_PACKET* packet);
	//ä�� ������ ����
	void AnsChannelOut(T_PACKET* packet);
	//������ ����
	void AnsExit(T_PACKET* packet);

public:
	ServerSessionParser(IOCPServerSession* ServerSession);
	~ServerSessionParser();

	void Init();

	/****************************************************************************
	�Լ���	: PacketParsing
	����		: ���� ��Ŷ�� type�� ���� �з��Ͽ� buff�� �Ľ���
	*****************************************************************************/
	bool PacketParsing(T_PACKET* const packet);

	/****************************************************************************
	�Լ���	: ErrorPrint
	����		: ������ ���� ���� ������ ����ϱ� ����
	*****************************************************************************/
	void ErrorPrint(UINT errorNum);
};

