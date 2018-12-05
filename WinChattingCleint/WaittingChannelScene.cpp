#include "stdafx.h"
#include "WaittingChannelScene.h"

WaittingChannelScene::WaittingChannelScene(const WCHAR * nodeName, UINT nodeNum)
	:BaseScene(nodeName, nodeNum),
	m_pWaittingChannelUI(nullptr)
{
}

WaittingChannelScene::~WaittingChannelScene()
{
	SAFE_DELETE(m_pWaittingChannelUI);
}

HRESULT WaittingChannelScene::Init()
{
	HRESULT hr = BaseScene::Init();

	m_pWaittingChannelUI = new UINode(TEXT("WaittingChannelUI"));
	D2D_SIZE_F size = DIRECT2D->GetBackBufferTarget()->GetSize();
	hr = m_pWaittingChannelUI->Init(
		D2D_PRIMITEVS->RectFMakeCenter(
			size.width*0.5f,
			size.height*0.5f,
			size.width*0.5f,
			size.height*0.5f)
	);

	//UI �ڽ� �ʱ�ȭ
	m_pWaittingChannelUI->AddChildTextBoxUI(
		TEXT("IDTextBox"),
		D2D_PRIMITEVS->RectFMake(
			100,
			10,
			150.0f,
			30.0f),
		15,
		D2D1::ColorF(1.0f, 1.0f, 1.0f)
	);

	return hr;
}

void WaittingChannelScene::Release()
{
	BaseScene::Release();

	m_pWaittingChannelUI->Release();
}

void WaittingChannelScene::Update()
{
	BaseScene::Update();

	cout << endl;
	cout << "------ä�� ����-----" << endl;
	m_SendStream.clear();
	int selectNum = 0;
	bool roopOut = false;

	printf("1. ä�� ����Ʈ ���\n");
	printf("2. ä�� ����\n");
	printf("3. ä�� ����\n");
	printf("4. ����.\n");

	cout << "���� �Է� : ";
	cin >> selectNum;

	switch (selectNum)
	{
	case 1:
	{
		//ä�� ����Ʈ �޾ƿ�
		m_Packet.Clear();
		m_Packet.type = PK_REQ_WAITINGCHANNAL_ENTER;
		m_Packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
	}
		break;

	case 2:
	{
		m_SendStream.clear();
		char channelName[15];
		cout << "���� channelName �Է� : ";
		cin >> channelName;

		m_SendStream.write(channelName);

		m_Packet.Clear();
		m_Packet.type = PK_REQ_WAITINGCHANNAL_CHREAT_CHANNAL;
		m_Packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
	}
		break;

	case 3:
	{
		UINT channelNum = 0;
		char channelNumStr[15];

		cout << "������ channel ���� �Է� : ";
		cin >> channelNumStr;
		channelNum = atoi(channelNumStr);

		m_SendStream.write(&channelNum, sizeof(channelNum));

		m_Packet.Clear();
		m_Packet.type = PK_REQ_WAITINGCHANNAL_CHANNAL_JOIN;
		m_Packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
	}
	break;

	case 4:
	{
		cout << "�����ϱ�"<<endl;

		m_Packet.Clear();
		m_Packet.type = PK_REQ_EXIT;
		m_Packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
		//������ ��Ʈ�� ����
		shutdown(CLIENTNETWORK->GetServerSession()->GetSocket(), SD_SEND);
	}
		break;

	default:
		cout << "���ڸ� �ٽ� �Է��Ͻÿ�" << endl << endl;
		roopOut = true;
		break;
	}

	//Scene�� ����ɶ� ���� ���
	if (CLIENTNETWORK->IsOn())
	{
		WaitForRecvPacket();
	}
	cin.ignore();
}

void WaittingChannelScene::Render()
{
	BaseScene::Render();
}

LRESULT WaittingChannelScene::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return BaseScene::MainProc(hWnd, iMessage, wParam, lParam);
}
