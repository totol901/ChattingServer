/******************************************************************************
파일명	: TextManager.h
목적		: 택스트를 관리하기 위한 객체
******************************************************************************/
#pragma once

class TextManager
{
private:
	IDWriteFactory* m_pWirteFactory;

	ID2D1SolidColorBrush* m_pTextColorBrush;

	map<wstring, IDWriteTextFormat*> map_IDWriteTextFormat;

public:
	TextManager();
	~TextManager();

	HRESULT Init();
	void Destory();

	/****************************************************************************
	함수명	: CreatTextFontAndInsertMap
	설명		: font를 생성하고 맵에 넣어줌
	*****************************************************************************/
	HRESULT CreatTextFontAndInsertMap(wstring fontName, float fontSize, const TCHAR* fileName);

	/****************************************************************************
	함수명	: FindTextFormat
	설명		: 폰트를 찾아서 반환함
	*****************************************************************************/
	IDWriteTextFormat* FindTextFormat(wstring fontName);

	/****************************************************************************
	함수명	: TextRender
	설명		: 해당 폰트로 텍스트를 그려줌
	*****************************************************************************/
	void TextRender(wstring str, const TCHAR* fontName, D2D1_RECT_F textRC, D2D1::ColorF fontColor);
};
