/******************************************************************************
���ϸ�	: Util.h
����		: ��ƿ �Լ��� ����
******************************************************************************/
#pragma once

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

#undef SAFE_RELEASE
#define SAFE_RELEASE(obj)		\
{								\
	if((obj)) obj->Release();	\
	obj = nullptr;				\
}

//-------------------------------------------------------------------//
//������ �⺻ ��ũ�� ȸ�ǿ� __FUNCTION__ ����..
#define __W(x)              L##x
#define _W(x)               __W(x)
	
namespace ServerEngine
{
	namespace Util
	{

		inline bool loadConfig(IN OUT tinyxml2::XMLDocument *config)
		{
			if (!config->LoadFile("./config.xml"))
			{
				printf("! not exist config file.");
				return false;
			}
			return true;
		}

		/****************************************************************************
		�Լ���	: SAFE_LOCK
		����		: ������ ������ ���� �ǽ���
		�Ű�����	: Lock&
		*****************************************************************************/
		inline void SAFE_LOCK(Lock& lock)
		{
			LockSafeScope __lockSafe(&lock, _W(__FILE__), __LINE__);
		}

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

		/****************************************************************************
		�Լ���	: isOverFlower_uint
		����		: original�� add�� �� ������ overflow ���� �˻���
		���ϰ�	: bool
		�Ű�����	: unsigned int, unsigned int
		*****************************************************************************/
		inline bool isOverFlower_uint(unsigned int original, unsigned int add)
		{
			unsigned int before = original;
			unsigned int after = original + add;
			if ((original & 0x80000000) != (after & 0x80000000))
			{
				return false;
			}
			return true;
		}
	}
}
