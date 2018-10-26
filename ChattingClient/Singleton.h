/******************************************************************************
���ϸ�	: Singleton.h
����		: �̱��� ���� ����
�����	: public ����� �޾� ���
******************************************************************************/
#pragma once

template<class T>
class Singleton
{
private:
	static T* m_pInstatnce;

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
		if (m_pInstatnce == nullptr)
		{
			m_pInstatnce = new T;
		}

		return m_pInstatnce;
	}

	/****************************************************************************
	�Լ���	: DestroyInstance
	����		: �ν��Ͻ��� ������
	*****************************************************************************/
	void DestroyInstance()
	{
		SAFE_DELETE(m_pInstatnce);
	}
};

//m_pInstatnce�� nullptr�� �ʱ�ȭ
template<class T> T* Singleton<T>::m_pInstatnce = nullptr;
