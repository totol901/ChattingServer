/******************************************************************************
颇老疙	: InChannelScene.h
格利		: inChannel纠 按眉
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

