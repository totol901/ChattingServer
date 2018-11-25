/******************************************************************************
���ϸ�	: Singleton.h
����		: �̱��� ���� ����
�����	: public ����� �޾� ���
******************************************************************************/
#pragma once

template<class T>
class EngineSingleton
{
protected:
	EngineSingleton(const EngineSingleton&) = delete;
	EngineSingleton(const EngineSingleton&&) = delete;
	EngineSingleton& operator = (const EngineSingleton&) = delete;
	EngineSingleton& operator = (const EngineSingleton&&) = delete;

	EngineSingleton() {}
	virtual ~EngineSingleton() {}
	
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
