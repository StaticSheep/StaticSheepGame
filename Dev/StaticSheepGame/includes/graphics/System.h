#pragma once

#include <windef.h>

namespace DirectSheep
{
  GFX_API void Init(HWND hWnd, int ScreenWidth, int ScreenHeight, void(*callback)(void));
  GFX_API void Release(void);
  GFX_API void* GetDevice(void);

  GFX_API void LoadShader(const std::string& name);

  GFX_API void LoadTexture(const std::string& filename);
  GFX_API int GetTextureID(std::string& texture);

}
