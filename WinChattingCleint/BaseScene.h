/******************************************************************************
���ϸ�	: BaseScene.h
����		: Scene Ŭ������ �θ� Ŭ����
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

	//ID, Name ���� �Լ�
	UINT GetID() { return m_Id; }
	string GetName() { return m_Name; }
	void SetName(string name) { m_Name = name; }

	//Event���� �Լ�
	static void WaitForRecvPacket();
	static void SignalEvent();

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();
	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
