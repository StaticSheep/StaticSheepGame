/*****************************************************************
Filename: DrawLib.h
Project: 
Author(s): Scott Nelson (primary)
           Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "types/vectors/Vec2.h"
#include "types/vectors/Vec4.h"
#include "Handle.h"

namespace Framework
{
  class Draw
  {
  public:
    
    static Vec2 RotateAroundPoint(Vec2 p, Vec2 o, float theta);
    
    static void SetCamState(int camState);
    static void SetTexture(unsigned texID);
    static int  GetTextureID(const char* texName);
    static Vec2 GetTextSize(const char* text, const char* font);
    static void SetRotation(float theta);
    static void SetPosition(float x, float y);
    static void SetPositionEx(float x, float y, float z);
    static void SetColor(float r, float g, float b, float a);

    static void DrawRect(float x, float y, float width, float height);
    static void DrawRectOutline(float x, float y, float width, float height,
      float theta = 0);
    static void DrawQuad(float x, float y, float width, float height, 
      float theta = 0);
    static void DrawTexturedRect(float x, float y, float width, float height);
    static void DrawTexturedRectRotated(float x, float y,
      float width, float height, float theta);
    static void DrawLine(float sX, float sY, float eX, float eY);
    static void DrawLineEx(float sX, float sY, float eX, float eY,
      Vec4 startColor, Vec4 endColor);
    static void DrawCircle(float x, float y, float radius);

    static void DrawString(const char* text, Vec2D scale, int fontIndex);
    static void LuaDrawString(const char* text, float scale, int fontIndex);

    static int GetFontIndex(const char* fontName);

    static void DrawTriangle(float x0, float y0, float x1, float y1,
      float x2, float y2);

    static void ForceZ(bool use, float z);


    static Vec3 ToWorld(Vec2 screenPos);
    static Vec2 ToScreen(Vec3 worldPos);

    static int ScreenWidth(void);
    static int ScreenHeight(void);

    static Vec2 MeasureString(const char* text, Vec2D scale, int fontIndex);
    static Vec2 LuaMeasureString(const char* text, float scale, int fontIndex);
  private:
    static unsigned m_TextureID;
    static Vec4 m_Color;
    static Vec2 m_TextPos;
    static bool m_useCamera;
    static int m_whiteTextureID;
    static DirectSheep::Handle m_whiteHandle;

    static float m_forceZ;
    static bool m_useForcedZ;
    
  };
}