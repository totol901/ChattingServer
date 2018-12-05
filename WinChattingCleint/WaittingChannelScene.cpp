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

	//UI 자식 초기화
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
	cout << "------채널 대기방-----" << endl;
	m_SendStream.clear();
	int selectNum = 0;
	bool roopOut = false;

	printf("1. 채널 리스트 출력\n");
	printf("2. 채널 생성\n");
	printf("3. 채널 입장\n");
	printf("4. 종료.\n");

	cout << "숫자 입력 : ";
	cin >> selectNum;

	switch (selectNum)
	{
	case 1:
	{
		//채널 리스트 받아옴
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
		cout << "만들 channelName 입력 : ";
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

		cout << "입장할 channel 숫자 입력 : ";
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
		cout << "종료하기"<<endl;

		m_Packet.Clear();
		m_Packet.type = PK_REQ_EXIT;
		m_Packet.SetStream(m_SendStream);

		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
		//보내는 스트림 종료
		shutdown(CLIENTNETWORK->GetServerSession()->GetSocket(), SD_SEND);
	}
		break;

	default:
		cout << "숫자를 다시 입력하시오" << endl << endl;
		roopOut = true;
		break;
	}

	//Scene이 변경될때 까지 대기
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
