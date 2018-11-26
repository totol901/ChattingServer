/******************************************************************************
파일명	: D2DPrimitives.h
목적		: D2D용 기본 도형을 쉽게 생성하기 위해 제작
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
	함수명	: Point2FMake
	설명		: D2D용 float형 POINT를 만듬
	*****************************************************************************/
	inline D2D1_POINT_2F Point2FMake(const float& _x, const float& _y)
	{
		D2D1_POINT_2F pt = { _x, _y };

		return pt;
	}

	/****************************************************************************
	함수명	: RectFMake
	설명		: D2D용 Rect를 만듬
	*****************************************************************************/
	inline D2D_RECT_F RectFMake(const float& _x, const float& _y, const float& _width, const float& _height)
	{	
		D2D_RECT_F rc = { _x, _y, _x + _width, _y + _height };

		return rc;
	}
	/****************************************************************************
	함수명	: RectFMakeCenter
	설명		: D2D용 Rect를 중앙에 만듬
	*****************************************************************************/
	inline D2D_RECT_F RectFMakeCenter(const float& _x, const float& _y, const float& _width, const float& _height)
	{
		D2D_RECT_F rc = { _x - (_width * 0.5f), _y - (_height * 0.5f), _x + (_width * 0.5f), _y + (_height * 0.5f) };

		return rc;
	}
	
	/****************************************************************************
	함수명	: DrawRect
	설명		: D2D용 Rect를 그려줌
	*****************************************************************************/
	inline void DrawRect(const D2D_RECT_F &rc, const D2D1::ColorF& _color, const float & _bold, ID2D1StrokeStyle* _style = NULL)
	{
		ID2D1SolidColorBrush* pSolidBrush = nullptr;

		//브러쉬 생성
		D2D_RENDERTARGET->CreateSolidColorBrush(_color, &pSolidBrush);

		DIRECT2D->GetBackBufferTarget()->DrawRectangle(rc, pSolidBrush, _bold, _style);

		SAFE_RELEASE(pSolidBrush);
	}
	
	/****************************************************************************
	함수명	: DrawRectCenter
	설명		: D2D용 Rect를 중앙에 그려줌
	*****************************************************************************/
	inline void DrawRectCenter(const float& x, const float& y, const float& width, const float& height, const D2D1::ColorF& _color, const float & _bold, ID2D1StrokeStyle* _style = NULL)
	{
		D2D_RECT_F rc = { x - (width * 0.5f), y - (height * 0.5f), x + (width * 0.5f), y + (height* 0.5f) };

		ID2D1SolidColorBrush* pSolidBrush = nullptr;

		//브러쉬 생성
		D2D_RENDERTARGET->CreateSolidColorBrush(_color, &pSolidBrush);

		DIRECT2D->GetBackBufferTarget()->DrawRectangle(rc, pSolidBrush, _bold, _style);

		SAFE_RELEASE(pSolidBrush);
	}

	/****************************************************************************
	함수명	: DrawLine
	설명		: D2D용 선을 그려줌
	*****************************************************************************/
	inline void DrawLine(const D2D1_POINT_2F& _startPos, const D2D1_POINT_2F& _endPos, const D2D1::ColorF& _color, const float& _bold, ID2D1StrokeStyle* _style = NULL)
	{
		ID2D1SolidColorBrush* pSolidBrush = nullptr;

		//브러쉬 생성
		D2D_RENDERTARGET->CreateSolidColorBrush(_color, &pSolidBrush);

		//선 그려줌
		DIRECT2D->GetBackBufferTarget()->DrawLine(_startPos, _endPos, pSolidBrush, _bold, _style);

		SAFE_RELEASE(pSolidBrush);
	}

	/****************************************************************************
	함수명	: DrawEllipse
	설명		: D2D용 타원을 그려줌
	*****************************************************************************/
	inline void DrawEllipse(const float& x, const float& y, const float& radiusX, const float& radiusY, const D2D1::ColorF& _color)
	{
		D2D1_ELLIPSE ellipse;
		ellipse.point = { x, y };
		ellipse.radiusX = radiusX;
		ellipse.radiusY = radiusY;

		ID2D1SolidColorBrush* pSolidBrush = nullptr;

		//브러쉬 생성
		D2D_RENDERTARGET->CreateSolidColorBrush(_color, &pSolidBrush);

		//타원 그려줌
		DIRECT2D->GetBackBufferTarget()->FillEllipse(ellipse, pSolidBrush);

		SAFE_RELEASE(pSolidBrush);
	}
	
	/****************************************************************************
	함수명	: DrawEllipse
	설명		: D2D용 타원을 그려줌
	*****************************************************************************/
	inline void DrawEllipse(const D2D1_ELLIPSE& _elipse, const D2D1::ColorF& _color)
	{
		ID2D1SolidColorBrush* pSolidBrush = nullptr;

		//브러쉬 생성
		D2D_RENDERTARGET->CreateSolidColorBrush(_color, &pSolidBrush);

		//타원 그려줌
		DIRECT2D->GetBackBufferTarget()->FillEllipse(_elipse, pSolidBrush);

		SAFE_RELEASE(pSolidBrush);
	}
};
