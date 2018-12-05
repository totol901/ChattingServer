#pragma once

//콜백 
typedef function<void(void)> CALLBACK_FUNCTION_BUTTON;

class UINode;
class ButtonUI : public UINode
{
private:
	D2D1_RECT_F m_ButtonRect;
	D2D1_RECT_F m_StaticStrRect;
	D2D1::ColorF m_Color;
	bool m_Clicked;
	wstring m_pStaticStr;

	CALLBACK_FUNCTION_BUTTON m_CallBackFuntion;

public:
	ButtonUI(const TCHAR* nodeName);
	~ButtonUI();

	/****************************************************************************
	함수명	: Init
	설명		: 버튼 초기화함, 버튼에 글자 안쓰려면 staticStr 이후 파라미터 무시
	*****************************************************************************/
	HRESULT Init(D2D1_RECT_F ButtonRect, CALLBACK_FUNCTION_BUTTON func, const TCHAR* staticStr = NULL, D2D1_RECT_F StaticStrRect = D2D1::RectF(0.0f, 0.0f, 100.0f, 40.0f),  D2D1::ColorF TextColor = D2D1::ColorF(1.0, 1.0, 1.0));
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual LRESULT CALLBACK MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
