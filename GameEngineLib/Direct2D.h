/******************************************************************************
파일명	: Direct2D.h
목적		: Direct2D의 팩토리, 랜더타겟을 담는 객체
******************************************************************************/
#pragma once

class Direct2D
{
	friend Grapics2D;
private:
	Direct2D(const Direct2D&) = delete;
	Direct2D(const Direct2D&&) = delete;
	Direct2D& operator = (const Direct2D&) = delete;
	Direct2D& operator = (const Direct2D&&) = delete;
	Direct2D();
	~Direct2D();

	ID2D1Factory* m_pFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1BitmapRenderTarget* m_pBitmapTarget;
	ID2D1GdiInteropRenderTarget* m_pGDIRendertarget;
	ID2D1DCRenderTarget* m_pDCRenderTarget;

	D2D1_SIZE_F m_Size;

	HRESULT CreateFactory();
	HRESULT CreateRenderTarget();

	HRESULT Init();
	void Destroy();

public:
	/****************************************************************************
	함수명	: ResizeRenderTarget
	설명		: 랜더타겟 사이즈를 바꿔줌
	*****************************************************************************/
	void ResizeRenderTarget(D2D1_SIZE_U sizeU);

	/****************************************************************************
	함수명	: ResizeBackBufferTarget
	설명		: 벡버버의 사이즈를 바꿔줌
	*****************************************************************************/
	void ResizeBackBufferTarget(D2D1_SIZE_U sizeU);

	/****************************************************************************
	함수명	: ResizeDCRenderTarget
	설명		: DC 랜더타겟 사이즈 바꿔줌
	*****************************************************************************/
	void ResizeDCRenderTarget(D2D1_SIZE_U sizeU);

	ID2D1GdiInteropRenderTarget* GetGdiRenderTarget(){return m_pGDIRendertarget;}
	ID2D1DCRenderTarget* GetDCRenderTarget() { return m_pDCRenderTarget; }
	ID2D1Factory *	const		 GetFactory()		{ return m_pFactory; }
	ID2D1HwndRenderTarget* const GetRenderTarget()	{ return m_pRenderTarget; }
	ID2D1BitmapRenderTarget*	 GetBackBufferTarget() {return m_pBitmapTarget; }
};
