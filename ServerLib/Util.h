/******************************************************************************
파일명	: Util.h
목적		: 유틸 함수들 모음
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
//컴파일 기본 메크로 회피용 __FUNCTION__ 같은..
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
		함수명	: SAFE_LOCK
		설명		: 안전한 스레드 락을 실시함
		매개변수	: Lock&
		*****************************************************************************/
		inline void SAFE_LOCK(Lock& lock)
		{
			LockSafeScope __lockSafe(&lock, _W(__FILE__), __LINE__);
		}

		//-------------------------------------------------------------------//
		//범위 보정 및 체크
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
		함수명	: isOverFlower_uint
		설명		: original에 add를 더 했을시 overflow 유무 검사함
		리턴값	: bool
		매개변수	: unsigned int, unsigned int
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
