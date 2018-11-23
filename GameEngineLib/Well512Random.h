/******************************************************************************
���ϸ�	: RandomMT.h
����		: �������� ��� ���� ����
�����	: WELLRAND ��ũ�θ� ����Ͽ� �Լ��� �ҷ���
�����ڷ�	: 1. http://www.iro.umontreal.ca/~panneton/well/WELL1024a.c (Well ���� �˰��� ����Ʈ)
		2. �ѱ������� ��ī���� - �˰��� ����
�̷»���	: 18.10.11 ���� Well512Random Ŭ���� �߰�
******************************************************************************/
#pragma once

/****************************************************************************
��ũ�θ�	: WELLRAND
����		: Well512Random Ŭ������ �̱���
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
	�Լ���	: GetFloatValue
	����		: GetValue()�Լ��� ����Ͽ� ���� float���� return
	���ϰ�	: float
	�Ű�����	: ����
	�̷»���	: 18.10.11 �Լ� ����
	*****************************************************************************/
	float GetFloatValue()
	{
		unsigned int nRandValue = GetValue();

		union { unsigned long ul; float f; } p;
		p.ul = (((nRandValue *= 16807) & 0x007fffff) - 1) | 0x3f800000;

		return p.f - 1.f;
	}

	/****************************************************************************
	�Լ���	: GetValue
	����		: Well-Random512 �˰����� ����Ͽ� Value���� return
	���ϰ�	: unsigned int
	�Ű�����	: ����
	�̷»���	: 18.10.11 �Լ� ����
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
	�Լ���	: GetValue
	����		: nMinValue ~ nMaxValue������ ���� �������� return
	���ϰ�	: unsigned int
	�Ű�����	: unsigned int, unsigned int
	�̷»���	: 18.10.11 �Լ� ����
	*****************************************************************************/
	unsigned int GetValue(unsigned int nMinValue, unsigned int nMaxValue)
	{
		return nMinValue + (GetValue() % (nMaxValue - nMinValue + 1));
	}

	/****************************************************************************
	�Լ���	: GetValue
	����		: 0 ~ nMaxValue������ ���� �������� return
	���ϰ�	: unsigned int
	�Ű�����	: unsigned int
	�̷»���	: 18.10.11 �Լ� ����
	*****************************************************************************/
	unsigned int GetValue(unsigned int nMaxValue)
	{
		return GetValue() % (nMaxValue + 1);
	}

	/****************************************************************************
	�Լ���	: GetFloatValue
	����		: fMinValue ~ fMaxValue������ ���� �������� return
	���ϰ�	: float
	�Ű�����	: float, float
	�̷»���	: 18.10.11 �Լ� ����
	*****************************************************************************/
	float GetFloatValue(float fMinValue, float fMaxValue)
	{
		return fMinValue + GetFloatValue() * (fMaxValue - fMinValue);
	}

	/****************************************************************************
	�Լ���	: GetFloatValue
	����		: 0.0f ~ fMaxValue������ ���� �������� return
	���ϰ�	: float
	�Ű�����	: float
	�̷»���	: 18.10.11 �Լ� ����
	*****************************************************************************/
	float GetFloatValue(float fMaxValue)
	{
		return GetFloatValue() * fMaxValue;
	}
};
