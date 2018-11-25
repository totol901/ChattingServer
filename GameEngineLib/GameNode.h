#pragma once

class GameNode : public BasicNode
{
private:
	static POINT m_ptMouse;
	static BOOL m_LeftButtonDown;
	static BOOL m_MButtonDown;
	static BOOL m_RightButtonDown;
	static BOOL m_LeftDoubleClick;
	static int m_WheelValue;

	GameNode* m_pSelf;			//�� �ڽ�
	GameNode* m_pParent;		//���� ��尡 �ڽĳ��� ��ϵǾ� ���� �� �� ���� == �θ�
	GameNode* m_pPrev;			//���� ��尡 �ڽĳ��� ��ϵǾ� ���� �� �� �� ���(��) == ��
	GameNode* m_pNext;			//���� ��尡 �ڽĳ��� ��ϵǾ� ���� �� �� �� ���(��) == ����

	GameNode* m_pChildrenHead;	//���� ����� �ڽ����� ��ϵǾ� �ִ� ���(��)�� �� �� �ּ� ����
	GameNode* m_pChildrenTail;	//���� ����� �ڽ����� ��ϵǾ� �ִ� ���(��)�� �� �� �ּ� ����
	GameNode* m_pCurrent;		//���� ����� �ڽ����� ��ϵǾ� �ִ� ����߿� ���� �ڽ�(��)

public:
	GameNode(const WCHAR* nodeName, UINT nodeNum);
	virtual ~GameNode();

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	inline const POINT& GetptMouse() { return m_ptMouse; }
	inline const BOOL& IsLeftButtonDown() { return m_LeftButtonDown; }
	inline const BOOL& IsMiddleButtonDown() { return m_MButtonDown; }
	inline const BOOL& IsRightButtonDown() { return m_RightButtonDown; }
	inline const BOOL& IsLeftDoubleClick() { return m_LeftDoubleClick; }
	inline const int& IsWheelValue() { return m_WheelValue; }
	
	virtual void AddChild(GameNode* node);
	virtual void RemoveChild(GameNode* node);

	inline void SetSelf(GameNode* node) { m_pSelf = node; }
	inline GameNode* GetSelf(void) { return m_pSelf; }

	inline void SetChildrenHead(GameNode* node) { m_pChildrenHead = node; }
	inline GameNode* GetChildrenHead(void) { return m_pChildrenHead; }

	inline void SetChildrenTail(GameNode* node) { m_pChildrenTail = node; }
	inline GameNode* GetChildrenTail(void) { return m_pChildrenTail; }

	inline void SetParent(GameNode* node) { m_pParent = node; }
	inline GameNode* GetParent(void) { return m_pParent; }

	inline void SetPrev(GameNode* node) { m_pPrev = node; }
	inline GameNode* GetPrev(void) { return m_pPrev; }

	inline void SetNext(GameNode* node) { m_pNext = node; }
	inline GameNode* GetNext(void) { return m_pNext; }
};

