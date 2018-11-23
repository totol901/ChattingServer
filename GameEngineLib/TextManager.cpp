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
	//�ý�Ʈ �� ���丮 �������
	HRESULT hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,						//�� ���丮�� �����ؼ� ���Ŵ�
		__uuidof(m_pWirteFactory),						//__uuidof : m_pWirteFactory���� COM id�� ã���ش�.
		reinterpret_cast<IUnknown**>(&m_pWirteFactory)	//IUnknown*������ ���� ����ȯ�Ͽ� ���丮 ����!
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
	//��Ʈ �־��ش�.
	IDWriteTextFormat* pTextFormat = nullptr;

	//�ؽ�Ʈ ������ �����!
	HRESULT hr = m_pWirteFactory->CreateTextFormat(
		fileName,					//�۰�
		NULL,							//��Ʈ �÷��� (NULL ->�ý��� �۰�)
		DWRITE_FONT_WEIGHT_NORMAL,		//����
		DWRITE_FONT_STYLE_NORMAL,		//��Ÿ��(����)
		DWRITE_FONT_STRETCH_NORMAL,		//��
		fontSize,					//���� ũ��
		L"ko-kr",						//����
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
		Assert("�ؽ�Ʈ ��Ʈ ã�� ����");
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
