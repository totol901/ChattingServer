#pragma once


class BaseScene
{
protected:
	UINT m_Id;
	string m_Name;
	SendStream m_SendStream;
	T_PACKET m_Packet;

	HANDLE m_Event;

public:
	BaseScene();
	BaseScene(UINT id);
	virtual~BaseScene();

	UINT GetID() { return m_Id; }
	string GetName() { return m_Name; }
	void SetName(string name) { m_Name = name; }

	void WaitForRecvPacket();
	void SignalEvent();

	virtual void Init() = 0;
	virtual void Update() = 0;
};
