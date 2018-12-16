/******************************************************************************
파일명	: GameObject.h
목적		: 개임오브젝트 객체
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
