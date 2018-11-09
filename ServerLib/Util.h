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
