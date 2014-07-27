#pragma once

namespace DirectSheep
{
  GFX_API void GFX_Init_D3D(HWND hWnd, int ScreenWidth, int ScreenHeight);
  GFX_API void GFX_Release_D3D(void);
  GFX_API void GFX_Draw(void);
}