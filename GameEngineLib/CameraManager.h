#pragma once

class CameraManager
{
private:
	D2D1_RECT_F _camera;
	D2D1_RECT_F _miniCamera;
	bool _miniCameraSwitch;
	float _miniWidth;
	float _miniHeight;
	float _miniX;
	float _miniY;

public:
	HRESULT Init(void);
	void Release(void);

	void onOffMiniCamera(bool _miniSwitch) { _miniCameraSwitch = _miniSwitch; }
	void setMiniCamera(float distanceX, float distanceY, float x, float y, int miniWidth, int miniHieght);
	void setCamera(float x, float y, float winWidth, float winHeight, float realWidth, float realHeight);

	float getCameraTop() { return _camera.top; }
	float getCameraBottom() { return _camera.bottom; }
	float getCameraLeft() { return _camera.left; }
	float getCameraRight() { return _camera.right; }
	float getCameraCenterX() { return _camera.left + (_camera.right - _camera.left) *0.5f; }
	float getCameraCenterY() { return _camera.top + (_camera.bottom - _camera.top) *0.5f; }

	D2D1_RECT_F getCamera() { return _camera; }

	float getMiniWidth() { return _miniWidth; }
	float getMiniHeight() { return _miniHeight; }

	D2D1_RECT_F getMiniCamera() { return _miniCamera; }
	float getMiniX() { return _miniX; }
	float getMiniY() { return _miniY; }

	CameraManager();
	~CameraManager();
};

