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

	GameNode* m_pSelf;			//나 자신
	GameNode* m_pParent;		//현재 노드가 자식노드로 등록되어 있을 때 젤 상위 == 부모
	GameNode* m_pPrev;			//현재 노드가 자식노드로 등록되어 있을 때 내 앞 노드(씬) == 형
	GameNode* m_pNext;			//현재 노드가 자식노드로 등록되어 있을 때 내 뒤 노드(씬) == 동생

	GameNode* m_pChildrenHead;	//현재 노드의 자식으로 등록되어 있는 노드(씬)의 맨 앞 주소 맏형
	GameNode* m_pChildrenTail;	//현재 노드의 자식으로 등록되어 있는 노드(씬)의 맨 뒷 주소 막내
	GameNode* m_pCurrent;		//현재 노드의 자식으로 등록되어 있는 노드중에 현재 자식(씬)

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

