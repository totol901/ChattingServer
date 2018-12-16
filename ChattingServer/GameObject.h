/******************************************************************************
���ϸ�	: GameObject.h
����		: ���ӿ�����Ʈ ��ü
******************************************************************************/
#pragma once

class GameObject : public Object
{
protected:
	float m_LocationX;
	float m_LocationY;

public:
	GameObject();
	virtual~GameObject();

	void SetLocationX(float x) { m_LocationX = x; }
	void SetLocationY(float y) { m_LocationY = y; }
	void SetLocation(float x, float y) { m_LocationX = x; m_LocationY = y; }
	float& GetLocationX() { return m_LocationX; }
	float& GetLocationY() { return m_LocationY; }
};
