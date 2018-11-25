#include "stdafx.h"
#include "Config.h"

Config::Config()
	:m_pFile(nullptr),
	m_strIP(nullptr),
	m_Port(0),
	m_ScreenWidth(0),
	m_ScreenHeight(0),
	m_FullScreen(false)
{
}

Config::~Config()
{
	SAFE_DELETE_ARRAY(m_strIP);
}

HRESULT Config::Init()
{
	TCHAR* strINIPath = const_cast<TCHAR*>(TEXT("./config.ini"));
	//���� ����
	TCHAR* strSection = const_cast<TCHAR*>(TEXT("Server"));
	TCHAR* strKey = const_cast<TCHAR*>(TEXT("IP"));
	TCHAR* strValue = const_cast<TCHAR*>(TEXT("192.168.0.2"));
	//IP
	WritePrivateProfileString(strSection, strKey, strValue, strINIPath);
	//Port
	strKey = const_cast<TCHAR*>(TEXT("PORT"));
	strValue = const_cast<TCHAR*>(TEXT("9000"));
	WritePrivateProfileString(strSection, strKey, strValue, strINIPath);
	
	//Ŭ���̾�Ʈ ����
	//ȭ�� ũ��
	int screenWidth = (int)(GetSystemMetrics(SM_CXSCREEN) * 0.25f);
	int screenHeight = (int)(GetSystemMetrics(SM_CYSCREEN) * 0.25f);
	strSection = const_cast<TCHAR*>(TEXT("Client"));
	strKey = const_cast<TCHAR*>(TEXT("Screen"));
	TCHAR str[128] = { 0, };
	swprintf_s(str, TEXT("%d*%d"), screenWidth, screenHeight);
	WritePrivateProfileString(strSection, strKey, str, strINIPath);
	//Ǯ��ũ��
	strKey = const_cast<TCHAR*>(TEXT("FullScreen"));
	strValue = const_cast<TCHAR*>(TEXT("0"));
	WritePrivateProfileString(strSection, strKey, strValue, strINIPath);

	InputConfigData();

	return S_OK;
}

TCHAR * Config::ReadConfigValue(TCHAR * strSection, TCHAR * strkey)
{
	TCHAR* inBuf = new TCHAR[128];
	memset(inBuf, 0, 128);
	GetPrivateProfileString(strSection, strkey, NULL, inBuf, 128,
		(TEXT("./config.ini")));

	return inBuf;
}

void Config::InputConfigData()
{
	//���� ����
	//IP�޾ƿ�
	TCHAR* strSection = const_cast<TCHAR*>(TEXT("Server"));
	TCHAR* strKey = const_cast<TCHAR*>(TEXT("IP"));
	m_strIP = ReadConfigValue(strSection, strKey);
	
	//Port�޾ƿ�
	strKey = const_cast<TCHAR*>(TEXT("PORT"));
	TCHAR* str = ReadConfigValue(strSection, strKey);
	m_Port = (u_short)_tstoi(str);
	SAFE_DELETE_ARRAY(str);

	//Ŭ���̾�Ʈ ����
	//ȭ�� ũ�� �޾ƿ�
	strSection = const_cast<TCHAR*>(TEXT("Client"));
	strKey = const_cast<TCHAR*>(TEXT("Screen"));
	str = ReadConfigValue(strSection, strKey);
	TCHAR* con = NULL;
	TCHAR* tok = wcstok_s(str, L"*", &con);
	m_ScreenWidth = _tstoi(tok);
	tok = wcstok_s(NULL, L"*", &con);
	m_ScreenHeight = _tstoi(tok);
	SAFE_DELETE_ARRAY(str);

	//�ý�ũ�� ���� �޾ƿ�
	strKey = const_cast<TCHAR*>(TEXT("FullScreen"));
	str = ReadConfigValue(strSection, strKey);
	m_FullScreen = (bool)_tstoi(str);
	SAFE_DELETE_ARRAY(str);
}
