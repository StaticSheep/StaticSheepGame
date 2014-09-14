#pragma once

#define USE_ANTTWEAKBAR 1

namespace DirectSheep
{
  GFX_API void Init(HWND hWnd, int ScreenWidth, int ScreenHeight);
  GFX_API void Release(void);
  GFX_API void* GetDevice(void);

  GFX_API void LoadShader(const std::string& name);

  GFX_API void LoadTexture(const std::string& filename);
  GFX_API int GetTextureID(std::string& texture);


  GFX_API bool AntTweakBarLoaded();

}
