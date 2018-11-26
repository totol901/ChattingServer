/******************************************************************************
���ϸ�	: D2DPrimitives.h
����		: D2D�� �⺻ ������ ���� �����ϱ� ���� ����
******************************************************************************/
#pragma once

#ifndef D2D_RENDERTARGET
#define D2D_RENDERTARGET GameEngine::GetInstance()->GetGrapics()->GetGrapics2D()->GetDirect2D()->GetRenderTarget()
#endif

#ifndef DIRECT2D
#define DIRECT2D GameEngine::GetInstance()->GetGrapics()->GetGrapics2D()->GetDirect2D()
#endif

class D2DPrimitives
{
	friend Grapics2D;
private:
	D2DPrimitives(const D2DPrimitives&) = delete;
	D2DPrimitives(const D2DPrimitives&&) = delete;
	D2DPrimitives& operator = (const D2DPrimitives&) = delete;
	D2DPrimitives& operator = (const D2DPrimitives&&) = delete;
	D2DPrimitives();
	~D2DPrimitives();

public:
	void Init();
	void Destroy();

	/****************************************************************************
	�Լ���	: Point2FMake
	����		: D2D�� float�� POINT�� ����
	*****************************************************************************/
	inline D2D1_POINT_2F Point2FMake(const float& _x, const float& _y)
	{
		D2D1_POINT_2F pt = { _x, _y };

		return pt;
	}

	/****************************************************************************
	�Լ���	: RectFMake
	����		: D2D�� Rect�� ����
	*****************************************************************************/
	inline D2D_RECT_F RectFMake(const float& _x, const float& _y, const float& _width, const float& _height)
	{	
		D2D_RECT_F rc = { _x, _y, _x + _width, _y + _height };

		return rc;
	}
	/****************************************************************************
	�Լ���	: RectFMakeCenter
	����		: D2D�� Rect�� �߾ӿ� ����
	*****************************************************************************/
	inline D2D_RECT_F RectFMakeCenter(const float& _x, const float& _y, const float& _width, const float& _height)
	{
		D2D_RECT_F rc = { _x - (_width * 0.5f), _y - (_height * 0.5f), _x + (_width * 0.5f), _y + (_height * 0.5f) };

		return rc;
	}
	
	/****************************************************************************
	�Լ���	: DrawRect
	����		: D2D�� Rect�� �׷���
	*****************************************************************************/
	inline void DrawRect(const D2D_RECT_F &rc, const D2D1::ColorF& _color, const float & _bold, ID2D1StrokeStyle* _style = NULL)
	{
		ID2D1SolidColorBrush* pSolidBrush = nullptr;

		//�귯�� ����
		D2D_RENDERTARGET->CreateSolidColorBrush(_color, &pSolidBrush);

		DIRECT2D->GetBackBufferTarget()->DrawRectangle(rc, pSolidBrush, _bold, _style);

		SAFE_RELEASE(pSolidBrush);
	}
	
	/****************************************************************************
	�Լ���	: DrawRectCenter
	����		: D2D�� Rect�� �߾ӿ� �׷���
	*****************************************************************************/
	inline void DrawRectCenter(const float& x, const float& y, const float& width, const float& height, const D2D1::ColorF& _color, const float & _bold, ID2D1StrokeStyle* _style = NULL)
	{
		D2D_RECT_F rc = { x - (width * 0.5f), y - (height * 0.5f), x + (width * 0.5f), y + (height* 0.5f) };

		ID2D1SolidColorBrush* pSolidBrush = nullptr;

		//�귯�� ����
		D2D_RENDERTARGET->CreateSolidColorBrush(_color, &pSolidBrush);

		DIRECT2D->GetBackBufferTarget()->DrawRectangle(rc, pSolidBrush, _bold, _style);

		SAFE_RELEASE(pSolidBrush);
	}

	/****************************************************************************
	�Լ���	: DrawLine
	����		: D2D�� ���� �׷���
	*****************************************************************************/
	inline void DrawLine(const D2D1_POINT_2F& _startPos, const D2D1_POINT_2F& _endPos, const D2D1::ColorF& _color, const float& _bold, ID2D1StrokeStyle* _style = NULL)
	{
		ID2D1SolidColorBrush* pSolidBrush = nullptr;

		//�귯�� ����
		D2D_RENDERTARGET->CreateSolidColorBrush(_color, &pSolidBrush);

		//�� �׷���
		DIRECT2D->GetBackBufferTarget()->DrawLine(_startPos, _endPos, pSolidBrush, _bold, _style);

		SAFE_RELEASE(pSolidBrush);
	}

	/****************************************************************************
	�Լ���	: DrawEllipse
	����		: D2D�� Ÿ���� �׷���
	*****************************************************************************/
	inline void DrawEllipse(const float& x, const float& y, const float& radiusX, const float& radiusY, const D2D1::ColorF& _color)
	{
		D2D1_ELLIPSE ellipse;
		ellipse.point = { x, y };
		ellipse.radiusX = radiusX;
		ellipse.radiusY = radiusY;

		ID2D1SolidColorBrush* pSolidBrush = nullptr;

		//�귯�� ����
		D2D_RENDERTARGET->CreateSolidColorBrush(_color, &pSolidBrush);

		//Ÿ�� �׷���
		DIRECT2D->GetBackBufferTarget()->FillEllipse(ellipse, pSolidBrush);

		SAFE_RELEASE(pSolidBrush);
	}
	
	/****************************************************************************
	�Լ���	: DrawEllipse
	����		: D2D�� Ÿ���� �׷���
	*****************************************************************************/
	inline void DrawEllipse(const D2D1_ELLIPSE& _elipse, const D2D1::ColorF& _color)
	{
		ID2D1SolidColorBrush* pSolidBrush = nullptr;

		//�귯�� ����
		D2D_RENDERTARGET->CreateSolidColorBrush(_color, &pSolidBrush);

		//Ÿ�� �׷���
		DIRECT2D->GetBackBufferTarget()->FillEllipse(_elipse, pSolidBrush);

		SAFE_RELEASE(pSolidBrush);
	}
};
