#include "stdafx.h"
#include "Grapics.h"

Grapics::Grapics()
	:m_pGrapics2D(nullptr)
{
}

Grapics::~Grapics()
{
	SAFE_DELETE(m_pGrapics2D);
}

HRESULT Grapics::Init()
{
	m_pGrapics2D = new Grapics2D;
	HRESULT hr = m_pGrapics2D->Init();

	return hr;
}

void Grapics::Destroy()
{
	m_pGrapics2D->Destroy();
}
