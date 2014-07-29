#pragma once

#include "graphics/api.h"
#include <wtypes.h>

namespace DirectSheep
{
  GFX_API void GFX_Init_D3D(HWND hWnd, int ScreenWidth, int ScreenHeight);
  GFX_API void GFX_Release_D3D(void);
  GFX_API void GFX_Draw(Vec2 scale, Vec2 position, float rotation);
  GFX_API void InitGeometry(void);
  GFX_API void GFX_Update(float dt);
  GFX_API void SetupMatrices(void);
}