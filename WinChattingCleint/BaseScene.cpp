#include "stdafx.h"
#include "BaseScene.h"

HANDLE BaseScene::m_Event = CreateEvent(NULL, FALSE, FALSE, NULL);

BaseScene::BaseScene(const WCHAR * nodeName, UINT SceneNum)
	:GameNode(nodeName),
	m_Id(SceneNum)
{
}

BaseScene::~BaseScene()
{
	CloseHandle(m_Event);
}

void BaseScene::WaitForRecvPacket()
{
	WaitForSingleObject(m_Event, INFINITE);
}

void BaseScene::SignalEvent()
{
	SetEvent(m_Event);
}

HRESULT BaseScene::Init()
{
	return GameNode::Init();
}

void BaseScene::Release()
{
	GameNode::Release();
}

void BaseScene::Update()
{
	GameNode::Update();
}

void BaseScene::Render()
{
	GameNode::Render();
}

LRESULT BaseScene::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return GameNode::MainProc(hWnd, iMessage, wParam, lParam);
}

