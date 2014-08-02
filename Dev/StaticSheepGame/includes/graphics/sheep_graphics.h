#pragma once

#include "graphics/api.h"
#include <wtypes.h>

namespace DirectSheep
{
  GFX_API void InitD3D(HWND hWnd, int ScreenWidth, int ScreenHeight);
  GFX_API void ReleaseD3D(void);
  GFX_API void DrawSprite();
  GFX_API void FinishFrame();
  GFX_API void InitGeometry(void);
  GFX_API void StartFrame(float dt);
  GFX_API void SetupMatrices(void);
  GFX_API void LoadTexture(const std::string& filename);

  GFX_API void SetPosition(float x, float y);
  GFX_API void SetRotation(float theta);
  GFX_API void SetSize(float x, float y);
  GFX_API void SetTexture(std::string& filepath);
  GFX_API void SetUseCamera(bool useCam);
}