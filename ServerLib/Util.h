/******************************************************************************
���ϸ�	: Util.h
����		: ��ƿ �Լ��� ����
******************************************************************************/
#pragma once

//Ÿ�� ����
typedef UINT UInt32;
typedef int Int32;

//-------------------------------------------------------------------//
// delete object
#undef	SAFE_DELETE
#define SAFE_DELETE(obj)						\
{												\
	if ((obj)) delete(obj);		    			\
    (obj) = nullptr;							\
}

//-------------------------------------------------------------------//
// delete object array
#undef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(arr)					\
{												\
	if ((arr)) delete [] (arr);		    		\
    (arr) = nullptr;							\
}
	
namespace ServerEngine
{
	namespace Util
	{
		//-------------------------------------------------------------------//
		//���� ���� �� üũ
		template<typename T>
		inline T fixInRange(const T& minimum, const T& x, const T& maximum)
		{
			return min(maximum, max(x, minimum));
		}

		template<typename T>
		inline bool isInRange(const T& minimum, const T& x, const T& maximum)
		{
			return (x == fixInRange(minimum, x, maximum)) ? true : false;
		}

		/*char=>TCHAR ��ȯ �Լ�*/
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

		/*TCHAR=>char ��ȯ �Լ�*/
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

		/*char=>wchar ��ȯ �Լ�*/
		inline void StrConvA2W(CHAR* src, WCHAR* dest, size_t destLen)
		{
			if (destLen < 1)
			{
				return;
			}
			MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (int)destLen - 1);
		}

		/*wchar=>char ��ȯ �Լ�*/
		inline void StrConvW2A(WCHAR* src, CHAR* dest, size_t destLen)
		{
			if (destLen < 1)
			{
				return;
			}
			WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (int)destLen, NULL, FALSE);
		}

		inline float GetDistance(float x1, float y1, float x2, float y2)
		{
			float x = x2 - x1;
			float y = y2 - y1;

			float result = sqrt(x * x + y * y);

			return result;
		}

		inline float Lerp(float x1, float x2, float time)
		{

		}
	}
}
