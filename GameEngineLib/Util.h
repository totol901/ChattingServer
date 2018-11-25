#pragma once
#include "Well512Random.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{if(p) { delete(p); (p) = NULL;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{if(p) { delete[](p); (p) = NULL;}}
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			{if(p) { (p)->Release(); (p) = NULL;}}
#endif

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


/**OverFlower check 함수:
32bit의 최고 큰 bit를 '비트 AND 연산'하여 original이 oveflower 유무 채크함 **/
inline bool isOverFlower_unit(unsigned int original, unsigned int add)
{
	unsigned int before = original;
	unsigned int after = original + add;

	if ((original & 0x80000000) != (after & 0x80000000))
	{
		return false;
	}

	return true;
}

//에러 출력해줌
inline void err_display(const TCHAR *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
}


/*char=>TCHAR 변환 함수*/
inline void StrConvA2T(CHAR* src, TCHAR* dest, size_t destLen)
{
#ifdef UNICODE						//r_winnt
	if (destLen < 1)
	{
		return;
	}
	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (int)destLen - 1);
#endif
}

/*TCHAR=>char 변환 함수*/
inline void StrConvT2A(TCHAR* src, CHAR* dest, size_t destLen)
{
#ifdef UNICODE						//r_winnt
	if (destLen < 1)
	{
		return;
	}
	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (int)destLen, NULL, FALSE);
#endif
}

/*char=>wchar 변환 함수*/
inline void StrConvA2W(CHAR* src, WCHAR* dest, size_t destLen)
{
	if (destLen < 1)
	{
		return;
	}
	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (int)destLen - 1);
}

/*wchar=>char 변환 함수*/
inline void StrConvW2A(WCHAR* src, CHAR* dest, size_t destLen)
{
	if (destLen < 1)
	{
		return;
	}
	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (int)destLen, NULL, FALSE);
}

