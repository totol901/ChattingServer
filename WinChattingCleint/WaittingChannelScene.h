/******************************************************************************
���ϸ�	: WaittingChannelScene.h
����		: ������ ä�ξ� ��ü
******************************************************************************/
#pragma once

class WaittingChannelScene : public BaseScene
{
public:
	WaittingChannelScene(const WCHAR* nodeName, UINT nodeNum);
	~WaittingChannelScene();

	HRESULT Init();
	void Update();
};

