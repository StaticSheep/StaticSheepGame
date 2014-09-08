#pragma once

namespace DirectSheep
{
  GFX_API void StartFrame(float dt);
  GFX_API void FinishFrame();

  GFX_API void SetShader(const std::string& name);

  GFX_API void SetPosition(float x, float y);
  GFX_API void SetRotation(float theta);
  GFX_API void SetSize(float x, float y);
  GFX_API void SetTexture(int ID);
  GFX_API void SetUseCamera(bool useCam);

  GFX_API void DrawSprite();

  GFX_API void DrawSpriteText(const char * text, float size, const char * font);

}