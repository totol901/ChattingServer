/******************************************************************************
���ϸ�	: WaittingChannelScene.h
����		: ������ ä�ξ� ��ü
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
