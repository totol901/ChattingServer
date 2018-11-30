/******************************************************************************
파일명	: WaittingChannelScene.h
목적		: 웨이팅 채널씬 객체
******************************************************************************/
#pragma once

class Player;
class WaittingChannelScene : public BaseScene
{
private:
	UINode* m_pWaittingChannelUI;


public:
	WaittingChannelScene(const WCHAR* nodeName, UINT nodeNum);
	~WaittingChannelScene();

	HRESULT Init();
	void Release();
	void Update();
	void Render();
	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
