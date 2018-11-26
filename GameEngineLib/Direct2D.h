/******************************************************************************
���ϸ�	: Direct2D.h
����		: Direct2D�� ���丮, ����Ÿ���� ��� ��ü
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
	�Լ���	: ResizeRenderTarget
	����		: ����Ÿ�� ����� �ٲ���
	*****************************************************************************/
	void ResizeRenderTarget(D2D1_SIZE_U sizeU);

	/****************************************************************************
	�Լ���	: ResizeBackBufferTarget
	����		: �������� ����� �ٲ���
	*****************************************************************************/
	void ResizeBackBufferTarget(D2D1_SIZE_U sizeU);

	/****************************************************************************
	�Լ���	: ResizeDCRenderTarget
	����		: DC ����Ÿ�� ������ �ٲ���
	*****************************************************************************/
	void ResizeDCRenderTarget(D2D1_SIZE_U sizeU);

	ID2D1GdiInteropRenderTarget* GetGdiRenderTarget(){return m_pGDIRendertarget;}
	ID2D1DCRenderTarget* GetDCRenderTarget() { return m_pDCRenderTarget; }
	ID2D1Factory *	const		 GetFactory()		{ return m_pFactory; }
	ID2D1HwndRenderTarget* const GetRenderTarget()	{ return m_pRenderTarget; }
	ID2D1BitmapRenderTarget*	 GetBackBufferTarget() {return m_pBitmapTarget; }
};
