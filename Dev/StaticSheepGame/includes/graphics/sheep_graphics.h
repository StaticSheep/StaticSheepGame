#pragma once

#include "graphics/api.h"
#include <wtypes.h>

namespace DirectSheep
{
  GFX_API void GFX_Init_D3D(HWND hWnd, int ScreenWidth, int ScreenHeight);
  GFX_API void GFX_Release_D3D(void);
  GFX_API void GFX_Draw();
  GFX_API void InitGeometry(void);
  GFX_API void GFX_Update(float dt);
  GFX_API void SetupMatrices(void);
  GFX_API void LoadTexture(const std::string& filename);
  GFX_API void GFX_SetPosition(float x, float y);
  GFX_API void GFX_SetRotation(float theta);
  GFX_API void GFX_SetSize(float x, float y);
  GFX_API void GFX_SetTexture(std::string& filepath);
}