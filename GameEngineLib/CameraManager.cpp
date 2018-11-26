#include "stdafx.h"
#include "CameraManager.h"

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
}

HRESULT CameraManager::Init(void)
{
	_camera = D2D_PRIMITEVS->RectFMake(0.0f, 0.0f, 800.0f, 600.0f);
	_miniCamera = D2D_PRIMITEVS->RectFMake(0.0f, 0.0f, 100.0f, 100.0f);
	_miniWidth = 100.0f;
	_miniHeight = 100.0f;
	_miniX = 0.0f;
	_miniY = 0.0f;

	return S_OK;
}

void CameraManager::Release(void)
{
}

void CameraManager::setMiniCamera(float distanceX, float distanceY, float x, float y, int miniWidth, int miniHieght)
{
	_miniX = distanceX;
	_miniY = distanceY;
	_miniCamera = D2D_PRIMITEVS->RectFMake(x, y, (float)miniWidth, (float)miniHieght);
	_miniWidth = (float)miniWidth;
	_miniHeight = (float)miniHieght;
}

void CameraManager::setCamera(float x, float y, float winWidth, float winHeight, float realWidth, float realHeight)
{
	//미니 카메라 안쓸때
	if (!_miniCameraSwitch)
	{
		if (winWidth * 0.5f > x)
			x = winWidth * 0.5f;
		if (realWidth - winWidth * 0.5f < x)
			x = realWidth - winWidth * 0.5f;
		if (winHeight * 0.5f > y)
			y = winHeight * 0.5f;
		if (realHeight - winHeight * 0.5f < y)
			y = realHeight - winHeight * 0.5f;
		_camera = D2D_PRIMITEVS->RectFMake(x, y, winWidth, winHeight);
	}
	//미니카메라 쓸때
	else
	{
		//미니카메라의 왼족보다 x가 작으면
		if (x < _miniCamera.left)
		{
			//미니카메라의 왼쪽을 x로 맞춰준다.
			_miniCamera.left = x;
			_miniCamera.right = _miniCamera.left + _miniWidth;

			//근데 만약 x가 윈도우 창의 왼쪽 끝부분에 있을경우
			if (winWidth *0.5f - _miniWidth * 0.5f + _miniX > x)
			{
				//_x 에 윈도우창의 반 크기를 넣는다.
				float _x;
				_x = winWidth * 0.5f + _miniX;
				//미니카메라를 재조정 해준다.
				_miniCamera.left = _x - _miniWidth * 0.5f;
				_miniCamera.right = _miniCamera.left + _miniWidth;
			}
			//미니카메라 재조정 거쳤건 안거쳤건 미니카메라를 이용하여 카메라를 조정해준다.
			_camera = D2D_PRIMITEVS->RectFMake(_miniCamera.left + (_miniCamera.right - _miniCamera.left) *0.5f - _miniX,
				_miniCamera.top + (_miniCamera.bottom - _miniCamera.top) *0.5f - _miniY, winWidth, winHeight);
		}
		//아래도 같은 원리이다.
		if (x > _miniCamera.right)
		{
			_miniCamera.right = x;
			_miniCamera.left = _miniCamera.right - _miniWidth;
			if (realWidth - (winWidth - ((winWidth *0.5f) + _miniX + (_miniWidth *0.5f))) < x)
			{
				_miniCamera.right = realWidth - (winWidth - ((winWidth *0.5f) + _miniX + (_miniWidth *0.5f)));
				_miniCamera.left = _miniCamera.right - _miniWidth;
				float distance = _miniX;
			}
			_camera = D2D_PRIMITEVS->RectFMake(_miniCamera.left + (_miniCamera.right - _miniCamera.left) *0.5f - _miniX,
				_miniCamera.top + (_miniCamera.bottom - _miniCamera.top) *0.5f - _miniY, winWidth, winHeight);
		}
		if (y < _miniCamera.top)
		{
			_miniCamera.top = y;
			_miniCamera.bottom = _miniCamera.top + _miniHeight;
			if (winHeight *0.5f - _miniHeight * 0.5f + _miniY > y)
			{
				float _y;
				_y = winHeight * 0.5f + _miniY;
				_miniCamera.top = _y - _miniHeight * 0.5f;
				_miniCamera.bottom = _miniCamera.top + _miniHeight;

			}
			_camera = D2D_PRIMITEVS->RectFMake(_miniCamera.left + (_miniCamera.right - _miniCamera.left) *0.5f - _miniX,
				_miniCamera.top + (_miniCamera.bottom - _miniCamera.top) *0.5f - _miniY, winWidth, winHeight);
		}
		if (y > _miniCamera.bottom)
		{
			_miniCamera.bottom = y;
			_miniCamera.top = _miniCamera.bottom - _miniHeight;
			if (realHeight - (winHeight - ((winHeight *0.5f) + _miniY + (_miniHeight *0.5f))) < y)
			{
				_miniCamera.bottom = realHeight - (winHeight - ((winHeight *0.5f) + _miniY + (_miniHeight *0.5f)));
				_miniCamera.top = _miniCamera.bottom - _miniHeight;
			}
			_camera = D2D_PRIMITEVS->RectFMake(_miniCamera.left + (_miniCamera.right - _miniCamera.left) *0.5f - _miniX,
				_miniCamera.top + (_miniCamera.bottom - _miniCamera.top) *0.5f - _miniY, winWidth, winHeight);
		}
	}
}
