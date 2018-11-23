#include "stdafx.h"
#include "TextManager.h"

TextManager::TextManager()
	:m_pWirteFactory(nullptr)
{
}

TextManager::~TextManager()
{
}

HRESULT TextManager::Init()
{
	//택스트 용 팩토리 만들어줌
	HRESULT hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,						//이 팩토리는 공유해서 쓸거다
		__uuidof(m_pWirteFactory),						//__uuidof : m_pWirteFactory형의 COM id를 찾아준다.
		reinterpret_cast<IUnknown**>(&m_pWirteFactory)	//IUnknown*형으로 강제 형변환하여 팩토리 생성!
	);

	if (SUCCEEDED(hr))
	{
		hr = D2D_RENDERTARGET->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&m_pTextColorBrush);
	}

	return hr;
}

void TextManager::Destory()
{
	SAFE_RELEASE(m_pWirteFactory);
	SAFE_RELEASE(m_pTextColorBrush);

	for (auto iter = map_IDWriteTextFormat.begin();
		iter != map_IDWriteTextFormat.end(); iter++)
	{
		SAFE_RELEASE(iter->second);
	}
	map_IDWriteTextFormat.clear();
}

HRESULT TextManager::CreatTextFontAndInsertMap(wstring fontName, float fontSize, const TCHAR* fileName)
{
	//폰트 넣어준다.
	IDWriteTextFormat* pTextFormat = nullptr;

	//텍스트 포맷을 만든다!
	HRESULT hr = m_pWirteFactory->CreateTextFormat(
		fileName,					//글골
		NULL,							//폰트 컬랙션 (NULL ->시스템 글골)
		DWRITE_FONT_WEIGHT_NORMAL,		//굵기
		DWRITE_FONT_STYLE_NORMAL,		//스타일(기울기)
		DWRITE_FONT_STRETCH_NORMAL,		//폭
		fontSize,					//문자 크기
		L"ko-kr",						//지역
		&pTextFormat
	);

	auto iter = map_IDWriteTextFormat.find(fontName);
	if (iter != map_IDWriteTextFormat.end())
	{
		SAFE_RELEASE(pTextFormat);
		return E_FAIL;
	}

	map_IDWriteTextFormat.insert(make_pair(fontName, pTextFormat));

	return hr;
}

IDWriteTextFormat * TextManager::FindTextFormat(wstring fontName)
{
	auto iter = map_IDWriteTextFormat.find(fontName);
	if (iter == map_IDWriteTextFormat.end())
	{
		return nullptr;
	}

	return iter->second;
}

void TextManager::TextRender(wstring str, const TCHAR * fontName, D2D1_RECT_F textRC, D2D1::ColorF fontColor)
{
	IDWriteTextFormat* textFormat = FindTextFormat(fontName);
	if (!textFormat)
	{
		Assert("텍스트 폰트 찾기 실패");
	}

	m_pTextColorBrush->SetColor(fontColor);

	
	DIRECT2D->GetBackBufferTarget()->DrawText(
		str.c_str(),
		(UINT)str.length(),
		textFormat,
		textRC,
		m_pTextColorBrush
	);
}
