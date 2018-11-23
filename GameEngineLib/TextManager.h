/******************************************************************************
���ϸ�	: TextManager.h
����		: �ý�Ʈ�� �����ϱ� ���� ��ü
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
	�Լ���	: CreatTextFontAndInsertMap
	����		: font�� �����ϰ� �ʿ� �־���
	*****************************************************************************/
	HRESULT CreatTextFontAndInsertMap(wstring fontName, float fontSize, const TCHAR* fileName);

	/****************************************************************************
	�Լ���	: FindTextFormat
	����		: ��Ʈ�� ã�Ƽ� ��ȯ��
	*****************************************************************************/
	IDWriteTextFormat* FindTextFormat(wstring fontName);

	/****************************************************************************
	�Լ���	: TextRender
	����		: �ش� ��Ʈ�� �ؽ�Ʈ�� �׷���
	*****************************************************************************/
	void TextRender(wstring str, const TCHAR* fontName, D2D1_RECT_F textRC, D2D1::ColorF fontColor);
};
