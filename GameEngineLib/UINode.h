/******************************************************************************
���ϸ�	: UINode.h
����		: ����ִ� UI���� �θ� �Ǵ� ���(����ִ� UI��带 ������)
******************************************************************************/
#pragma once
typedef function<void(void)> CALLBACK_FUNCTION_BUTTON;

class TextBoxUI;
class ButtonUI;
class StaticTextUI;
class UINode : public GameNode
{
protected:
	bool m_bActive;

	D2D1_RECT_F m_UIRect;

private:
	bool m_bRectRender;

	bool m_bOn;

public:
	UINode(const TCHAR* nodeName);
	virtual ~UINode();

	void AddChildButtonUI(const TCHAR* ButtonName, D2D1_RECT_F ButtonRect, CALLBACK_FUNCTION_BUTTON func, const TCHAR* staticStr = NULL, D2D1_RECT_F StaticStrRect = D2D1::RectF(0.0f, 0.0f, 100.0f, 30.0f), D2D1::ColorF TextColor = D2D1::ColorF(1.0, 1.0, 1.0));
	void AddChildTextBoxUI(const TCHAR* TextBoxName, D2D1_RECT_F TextBoxRect, UINT MaxTextLen, D2D1::ColorF TextColor);
	void AddChildStaticTextUI(const TCHAR* StaticTextName, D2D1_RECT_F UIRect, const TCHAR* staticText, D2D1::ColorF TextColor, bool isUIBoxRender);
	UINode* FindUI(const TCHAR* UIName);
	bool IsOn() { return m_bOn; }
	void SetOn(const bool& On) { m_bOn = On; }

	D2D1_RECT_F& GetUIRect() { return m_UIRect; }

	/****************************************************************************
	�Լ���	: OnActive
	����		: UI�� ���콺 Ŭ���� Active�� (�⺻ - ����� override)
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
