/******************************************************************************
���ϸ�	: Singleton.h
����		: �̱��� ���� ����
�����	: public ����� �޾� ���
******************************************************************************/
#pragma once

template<class T>
class Singleton
{
protected:
	Singleton(const Singleton&) = delete;
	Singleton(const Singleton&&) = delete;
	Singleton& operator = (const Singleton&) = delete;
	Singleton& operator = (const Singleton&&) = delete;

	Singleton() {}
	virtual ~Singleton() {}
	
public:
	/****************************************************************************
	�Լ���	: GetInstance
	����		: T Ŭ������ �ν��Ͻ� �ּҸ� ���Ϲ���
	*****************************************************************************/
	static T* GetInstance()
	{
		static T Instatnce;

		return &Instatnce;
	}
};

