/******************************************************************************
파일명	: D2DPrimitives.h
목적		: D2D용 기본 도형을 쉽게 생성하기 위해 제작
******************************************************************************/
#pragma once

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
	inline D2D1_POINT_2F Point2FMake(const float& _x, const float& _y);

	/****************************************************************************
	함수명	: RectFMake
	설명		: D2D용 Rect를 만듬
	*****************************************************************************/
	inline D2D_RECT_F RectFMake(const float& _x, const float& _y, const float& _width, const float& _height);
	
	/****************************************************************************
	함수명	: RectFMakeCenter
	설명		: D2D용 Rect를 중앙에 만듬
	*****************************************************************************/
	inline D2D_RECT_F RectFMakeCenter(const float& _x, const float& _y, const float& _width, const float& _height);
	
	
	/****************************************************************************
	함수명	: DrawRect
	설명		: D2D용 Rect를 그려줌
	*****************************************************************************/
	inline void DrawRect(const D2D_RECT_F &rc, const D2D1::ColorF& _color, const float & _bold, ID2D1StrokeStyle* _style = NULL);
	
	/****************************************************************************
	함수명	: DrawRectCenter
	설명		: D2D용 Rect를 중앙에 그려줌
	*****************************************************************************/
	inline void DrawRectCenter(const float& x, const float& y, const float& width, const float& height, const D2D1::ColorF& _color, const float & _bold, ID2D1StrokeStyle* _style = NULL);
	
	/****************************************************************************
	함수명	: DrawLine
	설명		: D2D용 선을 그려줌
	*****************************************************************************/
	inline void DrawLine(const D2D1_POINT_2F& _startPos, const D2D1_POINT_2F& _endPos, const D2D1::ColorF& _color, const float& _bold, ID2D1StrokeStyle* _style = NULL);
	
	/****************************************************************************
	함수명	: DrawEllipse
	설명		: D2D용 타원을 그려줌
	*****************************************************************************/
	inline void DrawEllipse(const float& x, const float& y, const float& radiusX, const float& radiusY, const D2D1::ColorF& _color);
	/****************************************************************************
	함수명	: DrawEllipse
	설명		: D2D용 타원을 그려줌
	*****************************************************************************/
	inline void DrawEllipse(const D2D1_ELLIPSE& _elipse, const D2D1::ColorF& _color);
};
