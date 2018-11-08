/******************************************************************************
파일명	: BaseScene.h
목적		: Scene 클래스의 부모 클래스
******************************************************************************/
#pragma once

class BaseScene
{
protected:
	UINT		m_Id;
	string		m_Name;
	SendStream	m_SendStream;
	T_PACKET	m_Packet;

	HANDLE		m_Event;

public:
	BaseScene();
	BaseScene(UINT id);
	virtual ~BaseScene();

	//ID, Name 관련 함수
	UINT GetID() { return m_Id; }
	string GetName() { return m_Name; }
	void SetName(string name) { m_Name = name; }

	//Event관련 함수
	void WaitForRecvPacket();
	void SignalEvent();

	virtual void Init() = 0;
	virtual void Update() = 0;
};
