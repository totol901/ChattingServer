#include "stdafx.h"
#include "D2DPrimitives.h"
#include "Direct2D.h"

D2DPrimitives::D2DPrimitives()
{
}

D2DPrimitives::~D2DPrimitives()
{
}

void D2DPrimitives::Init()
{
}

void D2DPrimitives::Destroy()
{
}

inline D2D1_POINT_2F D2DPrimitives::Point2FMake(const float& _x, const float& _y)
{
	D2D1_POINT_2F pt = { _x, _y};

	return pt;
}

inline D2D_RECT_F D2DPrimitives::RectFMake(const float& _x, const float& _y, const float& _width, const float& _height)
{
	D2D_RECT_F rc = { _x, _y, _x + _width, _y + _height };

	return rc;
}

inline D2D_RECT_F D2DPrimitives::RectFMakeCenter(const float& _x, const float& _y, const float& _width, const float& _height)
{
	D2D_RECT_F rc = { _x - (_width * 0.5f), _y - (_height * 0.5f), _x + (_width * 0.5f), _y + (_height * 0.5f) };

	return rc;
}

inline void D2DPrimitives::DrawRect(const D2D_RECT_F & rc, const D2D1::ColorF & _color, const float & _bold, ID2D1StrokeStyle* _style)
{
	ID2D1SolidColorBrush* pSolidBrush = nullptr;

	//브러쉬 생성
	D2D_RENDERTARGET->CreateSolidColorBrush(_color, &pSolidBrush);

	D2D_RENDERTARGET->DrawRectangle(rc, pSolidBrush, _bold, _style);

	SAFE_RELEASE(pSolidBrush);
}

inline void D2DPrimitives::DrawRectCenter(const float & x, const float & y, const float & width, const float & height, const D2D1::ColorF & _color, const float & _bold, ID2D1StrokeStyle * _style)
{
	D2D_RECT_F rc = { x - (width * 0.5f), y - (height * 0.5f), x + (width * 0.5f), y + (height* 0.5f) };
	
	ID2D1SolidColorBrush* pSolidBrush = nullptr;

	//브러쉬 생성
	D2D_RENDERTARGET->CreateSolidColorBrush(_color, &pSolidBrush);

	D2D_RENDERTARGET->DrawRectangle(rc, pSolidBrush, _bold, _style);

	SAFE_RELEASE(pSolidBrush);
}

inline void D2DPrimitives::DrawLine(const D2D1_POINT_2F & _startPos, const D2D1_POINT_2F & _endPos, const D2D1::ColorF & _color, const float & _bold, ID2D1StrokeStyle* _style)
{
	ID2D1SolidColorBrush* pSolidBrush = nullptr;

	//브러쉬 생성
	D2D_RENDERTARGET->CreateSolidColorBrush(_color, &pSolidBrush);

	//선 그려줌
	D2D_RENDERTARGET->DrawLine(_startPos, _endPos, pSolidBrush, _bold, _style);

	SAFE_RELEASE(pSolidBrush);
}

inline void D2DPrimitives::DrawEllipse(const float & x, const float & y, const float & radiusX, const float & radiusY, const D2D1::ColorF & _color)
{
	D2D1_ELLIPSE ellipse;
	ellipse.point = { x, y };
	ellipse.radiusX = radiusX;
	ellipse.radiusY = radiusY;

	ID2D1SolidColorBrush* pSolidBrush = nullptr;

	//브러쉬 생성
	D2D_RENDERTARGET->CreateSolidColorBrush(_color, &pSolidBrush);

	//타원 그려줌
	D2D_RENDERTARGET->FillEllipse(ellipse, pSolidBrush);

	SAFE_RELEASE(pSolidBrush);
}

inline void D2DPrimitives::DrawEllipse(const D2D1_ELLIPSE & _elipse, const D2D1::ColorF & _color)
{
	ID2D1SolidColorBrush* pSolidBrush = nullptr;

	//브러쉬 생성
	D2D_RENDERTARGET->CreateSolidColorBrush(_color, &pSolidBrush);

	//타원 그려줌
	D2D_RENDERTARGET->FillEllipse(_elipse, pSolidBrush);

	SAFE_RELEASE(pSolidBrush);
}
