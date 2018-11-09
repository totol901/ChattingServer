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
