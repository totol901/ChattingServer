/******************************************************************************
파일명	: Error.h
목적		: 윈속 에러 처리
사용방식	: Error 매크로 불러서 사용
******************************************************************************/
#pragma once


class Error :public Singleton<Error>
{
	friend Singleton;
private:
	Error();
	~Error();

	UINode* m_pPopupWaitingUI;	//서버 응답 지연시 등장할 UI
	UINode* m_pPopupErrorUI;

public:
	void PopupWaitingUIOnOff(bool onoff)
	{
		m_pPopupWaitingUI->SetOn(onoff);
	}

	UINode* GetPopupErrorUI() { return m_pPopupErrorUI; }
	void SetPopupErrorUI(UINode* PopupErrorUI) { m_pPopupErrorUI = PopupErrorUI; }
	
	void Init();
	/****************************************************************************
	함수명	: err_quit
	설명		:소켓 함수 오류 출력 후 종료
	*****************************************************************************/
	void err_quit(const char *msg);
	
	/****************************************************************************
	함수명	: err_display
	설명		: 소켓 함수 오류 출력
	*****************************************************************************/
	void err_display(const char *msg);
	
	/****************************************************************************
	함수명	: err_display
	설명		: errcode 출력
	*****************************************************************************/
	void err_display(int errcode);


	/****************************************************************************
	함수명	: err_print
	설명		: 에러 출력
	*****************************************************************************/
	void err_print(const TCHAR* str);

	void ErrUpdate();
	void ErrRender();
};
