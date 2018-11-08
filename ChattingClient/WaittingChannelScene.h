/******************************************************************************
파일명	: WaittingChannelScene.h
목적		: 웨이팅 채널씬 객체
******************************************************************************/
#pragma once

class WaittingChannelScene : public BaseScene
{
public:
	WaittingChannelScene();
	WaittingChannelScene(UINT id);
	~WaittingChannelScene();

	void Init();
	void Update();
};

