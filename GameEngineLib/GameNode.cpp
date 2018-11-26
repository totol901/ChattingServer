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
	//�ڽ����� �������� ���� �Ѱ��� ���ٶ�� �ϸ�
	if (m_pChildrenHead == nullptr)
	{
		//���� ���� �θ������ ��Ͻ�Ų��
		node->SetParent(this);

		//���� �ϳ����̴� �� ù��° ���̱⵵ �ϴ�
		SetChildrenHead(node);

		//�ڷδ� ������ ���� �ʾ����� �� ������
		SetChildrenTail(nullptr);
	}
	else
	{
		//���� ��忡 ���� �� ��忡�� ������ �־��ش�
		m_pCurrent = m_pChildrenHead;

		//��忡 ������ ������� ������? == ����Ǿ� �ִ� �ڽ� ���� ������
		while (m_pCurrent->GetNext() != nullptr)
		{
			//��� ã�� ������
			m_pCurrent = m_pCurrent->GetNext();
		}

		node->SetParent(this);		//���� ���� ���� �θ�� ������ ��(self)

		node->SetPrev(m_pCurrent);	//������ ���������
		m_pCurrent->SetNext(node);	//������ �ƿ�� ���� ���� ����
		node->SetNext(nullptr);		//������ �����ְ�
		m_pChildrenTail = node;		//������ ����
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

	//������ Ŭ������ �ݵ�� CS_DBLCLKS ��Ÿ���� ��������
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

