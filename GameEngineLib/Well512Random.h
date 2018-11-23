/******************************************************************************
파일명	: RandomMT.h
목적		: 랜덤값을 얻기 위해 만듬
사용방식	: WELLRAND 매크로를 사용하여 함수를 불러옴
참고자료	: 1. http://www.iro.umontreal.ca/~panneton/well/WELL1024a.c (Well 랜덤 알고리즘 사이트)
		2. 한국콘텐츠 아카데미 - 알고리즘 참고
이력사항	: 18.10.11 수정 Well512Random 클래스 추가
******************************************************************************/
#pragma once

/****************************************************************************
매크로명	: WELLRAND
설명		: Well512Random 클래스의 싱글턴
*****************************************************************************/
#define WELLRAND				Well512Random::GetInstance()

class Well512Random : public Singleton<Well512Random>
{
	friend Singleton;
private:
	enum
	{
		DIFFER_VALUE = 100,
	};

private:
	unsigned long state[16];
	unsigned int index;

	/****************************************************************************
	함수명	: GetFloatValue
	설명		: GetValue()함수를 사용하여 랜덤 float값을 return
	리턴값	: float
	매개변수	: 없음
	이력사항	: 18.10.11 함수 생성
	*****************************************************************************/
	float GetFloatValue()
	{
		unsigned int nRandValue = GetValue();

		union { unsigned long ul; float f; } p;
		p.ul = (((nRandValue *= 16807) & 0x007fffff) - 1) | 0x3f800000;

		return p.f - 1.f;
	}

	/****************************************************************************
	함수명	: GetValue
	설명		: Well-Random512 알고리즘을 사용하여 Value값을 return
	리턴값	: unsigned int
	매개변수	: 없음
	이력사항	: 18.10.11 함수 생성
	*****************************************************************************/
	unsigned int GetValue()
	{
		unsigned int a, b, c, d;

		a = state[index];
		c = state[(index + 13) & 15];
		b = a ^ c ^ (a << 16) ^ (c << 15);
		c = state[(index + 9) & 15];
		c ^= (c >> 11);
		a = state[index] = b ^ c;
		d = a ^ ((a << 5) & 0xda442d24U);
		index = (index + 15) & 15;
		a = state[index];
		state[index] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);

		return state[index];
	}

	Well512Random(const Well512Random&) = delete;
	Well512Random(const Well512Random&&) = delete;
	Well512Random& operator= (const Well512Random&) = delete;
	Well512Random& operator= (const Well512Random&&) = delete;
	explicit Well512Random()
	{
		index = 0;
		unsigned int s = static_cast<unsigned int>(time(NULL));

		for (int i = 0; i < 16; i++)
		{
			state[i] = s;
			s += DIFFER_VALUE;
		}
	}
	~Well512Random() {}

public:
	/****************************************************************************
	함수명	: GetValue
	설명		: nMinValue ~ nMaxValue사이의 값을 랜덤으로 return
	리턴값	: unsigned int
	매개변수	: unsigned int, unsigned int
	이력사항	: 18.10.11 함수 생성
	*****************************************************************************/
	unsigned int GetValue(unsigned int nMinValue, unsigned int nMaxValue)
	{
		return nMinValue + (GetValue() % (nMaxValue - nMinValue + 1));
	}

	/****************************************************************************
	함수명	: GetValue
	설명		: 0 ~ nMaxValue사이의 값을 랜덤으로 return
	리턴값	: unsigned int
	매개변수	: unsigned int
	이력사항	: 18.10.11 함수 생성
	*****************************************************************************/
	unsigned int GetValue(unsigned int nMaxValue)
	{
		return GetValue() % (nMaxValue + 1);
	}

	/****************************************************************************
	함수명	: GetFloatValue
	설명		: fMinValue ~ fMaxValue사이의 값을 랜덤으로 return
	리턴값	: float
	매개변수	: float, float
	이력사항	: 18.10.11 함수 생성
	*****************************************************************************/
	float GetFloatValue(float fMinValue, float fMaxValue)
	{
		return fMinValue + GetFloatValue() * (fMaxValue - fMinValue);
	}

	/****************************************************************************
	함수명	: GetFloatValue
	설명		: 0.0f ~ fMaxValue사이의 값을 랜덤으로 return
	리턴값	: float
	매개변수	: float
	이력사항	: 18.10.11 함수 생성
	*****************************************************************************/
	float GetFloatValue(float fMaxValue)
	{
		return GetFloatValue() * fMaxValue;
	}
};
