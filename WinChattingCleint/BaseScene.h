/******************************************************************************
파일명	: BaseScene.h
목적		: Scene 클래스의 부모 클래스
******************************************************************************/
#pragma once

class BaseScene : public GameNode
{
protected:
	UINT		m_Id;
	string		m_Name;
	SendStream	m_SendStream;
	T_PACKET	m_Packet;
	static HANDLE m_Event;

public:
	BaseScene(const WCHAR* nodeName, UINT SceneNum);
	virtual ~BaseScene();

	//ID, Name 관련 함수
	UINT GetID() { return m_Id; }
	string GetName() { return m_Name; }
	void SetName(string name) { m_Name = name; }

	//Event관련 함수
	static void WaitForRecvPacket();
	static void SignalEvent();

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();
	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
