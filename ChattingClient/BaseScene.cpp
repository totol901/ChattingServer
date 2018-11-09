#include "stdafx.h"
#include "BaseScene.h"

static UINT gerID = 0;

BaseScene::BaseScene()
	:m_Id(gerID++)
{
	m_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
}

BaseScene::BaseScene(UINT id)
	: m_Id(id)
{
	m_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
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

