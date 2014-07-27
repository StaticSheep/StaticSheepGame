#pragma once

#include "graphics/api.h"
#include <wtypes.h>

namespace Sheep_GFX
{
  GFX_API void GFX_Init_D3D(HWND hWnd, int ScreenWidth, int ScreenHeight);
  GFX_API void GFX_Release_D3D(void);
  GFX_API void GFX_Draw(void);
  class DirectX_Core;
  GFX_API extern DirectX_Core* CORE;
}