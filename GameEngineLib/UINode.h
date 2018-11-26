/******************************************************************************
파일명	: UINode.h
목적		: 기능있는 UI들의 부모가 되는 노드(기능있는 UI노드를 관리함)
******************************************************************************/
#pragma once

class TextBoxUI;
class ButtonUI;
class UINode : public GameNode
{
private:
	D2D1_RECT_F m_UIRect;
	bool m_bActive;
	bool m_bRectRender;

public:
	UINode(const TCHAR* nodeName);
	virtual ~UINode();

	void AddChildTextBoxUI(const TCHAR* TextBoxName, D2D1_RECT_F TextBoxRect, UINT MaxTextLen, D2D1::ColorF TextColor);
	UINode* FindUI(const TCHAR* UIName);

	/****************************************************************************
	함수명	: OnActive
	설명		: UI를 마우스 클릭시 Active됨 (기본 - 변경시 override)
	*****************************************************************************/
	virtual void OnActive();
	bool IsActive() { return m_bActive; }
	void SetActive(bool bActive) { m_bActive = bActive; }
	void SetRectRender(bool RectRender){ m_bRectRender = RectRender; }
	bool IsRectRender() { return m_bRectRender; }

	virtual HRESULT Init(D2D1_RECT_F UIRect);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
