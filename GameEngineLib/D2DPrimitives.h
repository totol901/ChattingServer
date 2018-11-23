/******************************************************************************
���ϸ�	: D2DPrimitives.h
����		: D2D�� �⺻ ������ ���� �����ϱ� ���� ����
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
	�Լ���	: Point2FMake
	����		: D2D�� float�� POINT�� ����
	*****************************************************************************/
	inline D2D1_POINT_2F Point2FMake(const float& _x, const float& _y);

	/****************************************************************************
	�Լ���	: RectFMake
	����		: D2D�� Rect�� ����
	*****************************************************************************/
	inline D2D_RECT_F RectFMake(const float& _x, const float& _y, const float& _width, const float& _height);
	
	/****************************************************************************
	�Լ���	: RectFMakeCenter
	����		: D2D�� Rect�� �߾ӿ� ����
	*****************************************************************************/
	inline D2D_RECT_F RectFMakeCenter(const float& _x, const float& _y, const float& _width, const float& _height);
	
	
	/****************************************************************************
	�Լ���	: DrawRect
	����		: D2D�� Rect�� �׷���
	*****************************************************************************/
	inline void DrawRect(const D2D_RECT_F &rc, const D2D1::ColorF& _color, const float & _bold, ID2D1StrokeStyle* _style = NULL);
	
	/****************************************************************************
	�Լ���	: DrawRectCenter
	����		: D2D�� Rect�� �߾ӿ� �׷���
	*****************************************************************************/
	inline void DrawRectCenter(const float& x, const float& y, const float& width, const float& height, const D2D1::ColorF& _color, const float & _bold, ID2D1StrokeStyle* _style = NULL);
	
	/****************************************************************************
	�Լ���	: DrawLine
	����		: D2D�� ���� �׷���
	*****************************************************************************/
	inline void DrawLine(const D2D1_POINT_2F& _startPos, const D2D1_POINT_2F& _endPos, const D2D1::ColorF& _color, const float& _bold, ID2D1StrokeStyle* _style = NULL);
	
	/****************************************************************************
	�Լ���	: DrawEllipse
	����		: D2D�� Ÿ���� �׷���
	*****************************************************************************/
	inline void DrawEllipse(const float& x, const float& y, const float& radiusX, const float& radiusY, const D2D1::ColorF& _color);
	/****************************************************************************
	�Լ���	: DrawEllipse
	����		: D2D�� Ÿ���� �׷���
	*****************************************************************************/
	inline void DrawEllipse(const D2D1_ELLIPSE& _elipse, const D2D1::ColorF& _color);
};
