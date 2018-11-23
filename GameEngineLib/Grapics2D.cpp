#include "stdafx.h"
#include "Grapics2D.h"

Grapics2D::Grapics2D()
	:m_pDirect2D(nullptr),
	m_pD2DPrimitive(nullptr)
{
}

Grapics2D::~Grapics2D()
{
	SAFE_DELETE(m_pD2DPrimitive);
	SAFE_DELETE(m_pDirect2D);
}

HRESULT Grapics2D::Init()
{
	m_pDirect2D = new Direct2D;
	m_pD2DPrimitive = new D2DPrimitives;

	HRESULT hr = m_pDirect2D->Init();
	if (SUCCEEDED(hr))
	{
		m_pD2DPrimitive->Init();
	}

	return hr;
}

void Grapics2D::Destroy()
{
	m_pD2DPrimitive->Destroy();
	m_pDirect2D->Destroy();
}