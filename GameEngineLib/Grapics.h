/******************************************************************************
파일명	: Grapics.h
목적		: 엔진의 그래픽을 담당하는 객체
******************************************************************************/
#pragma once

class Grapics2D;
class Grapics
{
	friend GameEngine;
private:
	Grapics(const Grapics&) = delete;
	Grapics(const Grapics&&) = delete;
	Grapics& operator = (const Grapics&) = delete;
	Grapics& operator = (const Grapics&&) = delete;
	Grapics();
	~Grapics();

	Grapics2D* m_pGrapics2D;

public:
	HRESULT Init();
	void Destroy();

	Grapics2D* const GetGrapics2D() { return m_pGrapics2D; }
};

