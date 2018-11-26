#include "stdafx.h"

using namespace D2D1;

Direct2D::Direct2D()
	:m_pFactory(nullptr),
	m_pRenderTarget(nullptr),
	m_pBitmapTarget(nullptr),
	m_pGDIRendertarget(nullptr),
	m_pDCRenderTarget(nullptr)
{
}

Direct2D::~Direct2D()
{
}

HRESULT Direct2D::Init()
{
	HRESULT hr = CreateFactory();
	if (SUCCEEDED(hr))
	{
		//그리기 위한 랜더 타겟 생성
		hr = CreateRenderTarget();
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->QueryInterface(
			__uuidof(ID2D1GdiInteropRenderTarget),
			(void**)&m_pGDIRendertarget);
	}

	

	if (SUCCEEDED(hr))
	{
		//벡버퍼 역할을 할 비트맵 타겟 생성
		hr = m_pRenderTarget->CreateCompatibleRenderTarget(
			D2D1::SizeF(m_pRenderTarget->GetSize().width*2.0f,
				m_pRenderTarget->GetSize().height*2.0f),
			D2D1::SizeU((UINT32)m_pRenderTarget->GetSize().width,
			(UINT32)m_pRenderTarget->GetSize().height),
			&m_pBitmapTarget);
	}

	if (SUCCEEDED(hr))
	{
		// Create a DC render target.
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_IGNORE),
			0,
			0,
			D2D1_RENDER_TARGET_USAGE_NONE,
			D2D1_FEATURE_LEVEL_DEFAULT
		);
	
		hr = m_pFactory->CreateDCRenderTarget(&props, &m_pDCRenderTarget);
	}

	m_Size = m_pRenderTarget->GetSize();

	return hr;
}

void Direct2D::Destroy()
{
	SAFE_RELEASE(m_pDCRenderTarget);
	SAFE_RELEASE(m_pGDIRendertarget);
	SAFE_RELEASE(m_pBitmapTarget);
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pFactory);
}

void Direct2D::ResizeRenderTarget(D2D1_SIZE_U sizeU)
{
	m_pRenderTarget->Resize(sizeU);

	m_pRenderTarget->QueryInterface(
		__uuidof(ID2D1GdiInteropRenderTarget),
		(void**)&m_pGDIRendertarget);
}

void Direct2D::ResizeBackBufferTarget(D2D1_SIZE_U sizeU)
{
	SAFE_RELEASE(m_pBitmapTarget);

	HRESULT hr = m_pRenderTarget->CreateCompatibleRenderTarget(
		D2D1::SizeF(m_Size.width, m_Size.height),
		D2D1::SizeU((UINT32)m_pRenderTarget->GetSize().width,
		(UINT32)m_pRenderTarget->GetSize().height),
		&m_pBitmapTarget);

	if (FAILED(hr))
	{
		err_display(TEXT("ResizeBackBufferTarget"));
	}
}

void Direct2D::ResizeDCRenderTarget(D2D1_SIZE_U sizeU)
{
	SAFE_RELEASE(m_pDCRenderTarget);
	
	// Create a DC render target.
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_IGNORE),
		0,
		0,
		D2D1_RENDER_TARGET_USAGE_NONE,
		D2D1_FEATURE_LEVEL_DEFAULT
	);
	
	m_pFactory->CreateDCRenderTarget(&props, &m_pDCRenderTarget);
}

HRESULT Direct2D::CreateFactory()
{
	return D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_pFactory);
}

HRESULT Direct2D::CreateRenderTarget()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	//D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
	//rtProps.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;

	return m_pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		HwndRenderTargetProperties(g_hWnd, SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		&m_pRenderTarget);
}
