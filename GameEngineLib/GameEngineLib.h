#pragma once

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "DWrite.lib")
#pragma comment(lib, "WindowsCodecs.lib")
#pragma comment(lib, "Gdiplus.lib")

#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <tchar.h>
#include <assert.h>
#include <vector>
#include <map>
#include <time.h>
#include <Commdlg.h>

using namespace std;

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

//D2D관련된 include
#include <d2d1.h>
#include <d2d1helper.h>
#include <d2dbasetypes.h>
#include <D2DErr.h>
#include <d2d1effects.h>
#include <d2d1effecthelpers.h>
#include <wincodec.h>
#include <Dwrite.h>

#include "EngineSingleton.h"
#include "Util.h"
#include "GameEngine.h"
#include "Grapics.h"
#include "Grapics2D.h"
#include "Direct2D.h"
#include "D2DPrimitives.h"
#include "ResourceManager.h"
#include "GameResource.h"
#include "Image.h"
#include "ImageManager.h"
#include "FrameImage.h"
#include "TextManager.h"
#include "BasicNode.h"
#include "GameNode.h"

//엔진 접근 매크로
#define GAME_ENGINE GameEngine::GetInstance()

#define D2D_FACTORY GameEngine::GetInstance()->GetGrapics()->GetGrapics2D()->GetDirect2D()->GetFactory()
#define DIRECT2D GameEngine::GetInstance()->GetGrapics()->GetGrapics2D()->GetDirect2D()
#define D2D_RENDERTARGET GameEngine::GetInstance()->GetGrapics()->GetGrapics2D()->GetDirect2D()->GetRenderTarget()
#define D2D_PRIMITEVS GameEngine::GetInstance()->GetGrapics()->GetGrapics2D()->Get2DPrimitives()
