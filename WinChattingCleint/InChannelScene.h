/******************************************************************************
���ϸ�	: InChannelScene.h
����		: inChannel�� ��ü
******************************************************************************/
#pragma once

class InChannelScene : public BaseScene
{
public:
	InChannelScene(const WCHAR* nodeName, UINT nodeNum);
	~InChannelScene();

	HRESULT Init();
	void Update();
};

