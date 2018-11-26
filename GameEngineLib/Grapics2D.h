/******************************************************************************
���ϸ�	: Grapics2D.h
����		: ������ 2D �׷����� ����ϴ� ��ü
******************************************************************************/
#pragma once

class Direct2D;
class D2DPrimitives;
class CameraManager;
class Grapics2D
{
	friend Grapics;
private:
	Grapics2D(const Grapics2D&) = delete;
	Grapics2D(const Grapics2D&&) = delete;
	Grapics2D& operator = (const Grapics2D&) = delete;
	Grapics2D& operator = (const Grapics2D&&) = delete;
	Grapics2D();
	~Grapics2D();

	Direct2D* m_pDirect2D;
	D2DPrimitives* m_pD2DPrimitive;
	CameraManager* m_pD2DCameraManager;

	HRESULT Init();
	void Destroy();

public:
	CameraManager* GetCamera() { return m_pD2DCameraManager; }
	 Direct2D* const		GetDirect2D()		const { return m_pDirect2D; }
	 D2DPrimitives* const	Get2DPrimitives()	const { return m_pD2DPrimitive; }
};

