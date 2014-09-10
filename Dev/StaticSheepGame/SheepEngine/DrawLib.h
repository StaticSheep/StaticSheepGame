/*****************************************************************
Filename: DrawLib.h
Project: 
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  class Draw
  {
  public:
    static void SetUseCamera(bool useCam);
    static void SetTexture(UINT texID);
    static int  GetTextureID(const char* texName);
    //static void SetColor(Vec4& color);
    static void SetRotation(float theta);
    static void SetPosition(float x, float y);
    static void SetColor(float r, float g, float b, float a);
    //static void SetTextPos(Vec2& pos);

    static void DrawRect(float x, float y, float width, float height);
    static void DrawTexturedRect(float x, float y, float width, float height);
    static void DrawLine(float sX, float sY, float eX, float eY, float width=1);
    static void DrawString(const char* text, float size, const char* font);
  private:
    static UINT m_TextureID;
    static Vec4 m_Color;
    static Vec2 m_TextPos;
    static bool m_useCamera;
  };
}