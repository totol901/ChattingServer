#include "stdafx.h"
#include "GameNode.h"

POINT GameNode::m_ptMouse = { 0, 0 };
BOOL GameNode::m_LeftButtonDown = FALSE;
BOOL GameNode::m_MButtonDown = FALSE;
BOOL GameNode::m_RightButtonDown = FALSE;
BOOL GameNode::m_LeftDoubleClick = FALSE;
int GameNode::m_WheelValue = 0;

GameNode::GameNode(const WCHAR* nodeName)
	:BasicNode(nodeName),
	m_pSelf(this),
	m_pParent(nullptr),
	m_pPrev(nullptr),
	m_pNext(nullptr),
	m_pChildrenHead(nullptr),
	m_pChildrenTail(nullptr),
	m_pCurrent(nullptr)
{
}

GameNode::~GameNode()
{
}

HRESULT GameNode::Init()
{
	return S_OK;
}

void GameNode::Release()
{
	m_pCurrent = m_pChildrenHead;

	GameNode* temp = nullptr;

	while (m_pCurrent != nullptr)
	{
		temp = m_pCurrent->GetNext();
		m_pCurrent->Release();
		SAFE_DELETE(m_pCurrent);
		m_pCurrent = m_pChildrenHead = temp;
	}
}

void GameNode::Update()
{
}

void GameNode::Render()
{
}

void GameNode::AddChild(GameNode * node)
{
	//자식으로 배정받은 씬이 한개도 없다라고 하면
	if (m_pChildrenHead == nullptr)
	{
		//현재 씬을 부모씬으로 등록시킨다
		node->SetParent(this);

		//씬이 하나뿐이니 맨 첫번째 씬이기도 하다
		SetChildrenHead(node);

		//뒤로는 연결이 되지 않았으니 널 값으로
		SetChildrenTail(nullptr);
	}
	else
	{
		//현재 노드에 제일 앞 노드에다 포인터 넣어준다
		m_pCurrent = m_pChildrenHead;

		//헤드에 다음께 비어있지 않으면? == 연결되어 있는 자식 씬이 있으면
		while (m_pCurrent->GetNext() != nullptr)
		{
			//계속 찾아 나간다
			m_pCurrent = m_pCurrent->GetNext();
		}

		node->SetParent(this);		//새로 들어온 씬의 부모는 현재의 씬(self)

		node->SetPrev(m_pCurrent);	//맏형을 현재씬으로
		m_pCurrent->SetNext(node);	//맏형의 아우로 들어온 씬을 지정
		node->SetNext(nullptr);		//포인터 없애주고
		m_pChildrenTail = node;		//막내로 지정
	}
}

void GameNode::RemoveChild(GameNode * node)
{
	int count = 0;
	m_pCurrent = m_pChildrenHead;
	GameNode* prev = nullptr;

	while (m_pCurrent != nullptr)
	{
		if (m_pCurrent == node)
		{
			if (count != 0)
			{
				prev->SetNext(m_pCurrent->GetNext());
				if (m_pCurrent->GetNext() != nullptr)
				{
					m_pCurrent->GetNext()->SetPrev(prev);
				}
				else
				{
					m_pChildrenTail = m_pCurrent;
				}
			}
			else
			{
				m_pChildrenHead = m_pCurrent->GetNext();
				if (m_pChildrenHead != nullptr)
				{
					m_pChildrenHead->SetPrev(nullptr);
				}
			}

			m_pCurrent->Release();
			SAFE_DELETE(m_pCurrent);
			break;
		}
		prev = m_pCurrent;
		m_pCurrent = m_pCurrent->GetNext();
		count++;
	}
}

LRESULT GameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	m_WheelValue = 0;

	switch (iMessage)
	{
	case WM_MOUSEMOVE:
		m_ptMouse.x = static_cast<LONG>LOWORD(lParam);
		m_ptMouse.y = static_cast<LONG>HIWORD(lParam);
		break;

	case WM_MOUSEWHEEL:
	{
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		
		m_WheelValue = zDelta / WHEEL_DELTA;	
	}
		break;

	case WM_MBUTTONDOWN:
		m_MButtonDown = TRUE;
		break;
	case WM_MBUTTONUP:
		m_MButtonDown = FALSE;
		break;

	//윈도우 클래스가 반드시 CS_DBLCLKS 스타일을 가져야함
	case WM_LBUTTONDBLCLK:
		m_LeftDoubleClick = TRUE;
		break;

	case WM_LBUTTONDOWN:
		m_LeftButtonDown = TRUE;
		break;
	case WM_LBUTTONUP:
		m_LeftButtonDown = FALSE;
		m_LeftDoubleClick = FALSE;
		break;
	case WM_RBUTTONDOWN:
		m_RightButtonDown = TRUE;
		break;
	case WM_RBUTTONUP:
		m_RightButtonDown = FALSE;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

