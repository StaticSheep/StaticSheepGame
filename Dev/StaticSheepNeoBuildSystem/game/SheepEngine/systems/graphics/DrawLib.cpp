/*****************************************************************
Filename: DrawLib.cpp
Project: 
Author(s): Scott Nelson(primary)
           Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "SheepGraphics.h"
#include "systems/graphics/Debug.h"
#include <math.h>

namespace Framework
{
  bool Draw::m_useCamera = true;
  Vec4 Draw::m_Color;
  Vec2 Draw::m_TextPos;
  unsigned Draw::m_TextureID;
  int Draw::m_whiteTextureID = -1;
  DirectSheep::Handle Draw::m_whiteHandle(DirectSheep::TEXTURE, 0);
  bool Draw::m_useForcedZ = false;
  float Draw::m_forceZ = 0.0f;

  void Draw::SetCamState(int camState)
  {
    Draw::m_useCamera = camState == 0;
    GRAPHICS->SetCamState(camState);
  }

  void Draw::SetTexture(unsigned texID)
  {
    Draw::m_TextureID = texID;
  }

  int Draw::GetTextureID(const char* texName)
  {
    std::string texString = texName;
    return GRAPHICS->GetTextureID(texString);
  }

  Vec2 Draw::GetTextSize(const char* text, const char* font)
  {
    return Vec2(32, 32);
  }

  void Draw::SetRotation(float theta)
  {
    GRAPHICS->SetRotation(theta);
  }

  void Draw::SetPosition(float x, float y)
  {
    //if (m_useForcedZ)
    //  TRACELOG->Log(TraceLevel::DEBUG, "Forced Z was %f", m_forceZ);
    GRAPHICS->SetPosition(x, y, m_useForcedZ ? m_forceZ : 0.0f);
  }

  void Draw::SetPositionEx(float x, float y, float z)
  {
    GRAPHICS->SetPosition(x, y, z);
  }

  void Draw::ForceZ(bool use, float z)
  {
    m_useForcedZ = use;
    m_forceZ = z;
    //TRACELOG->Log(TraceLevel::DEBUG, "Forced Z to %f", z);
  }

  void Draw::SetColor(float r, float g, float b, float a)
  {
    new (&Draw::m_Color) Vec4(r, g, b, a);
    GRAPHICS->SetColor(Draw::m_Color);
  }

  void Draw::DrawRect(float x, float y, float width, float height)
  {
    GRAPHICS->SetSize(width, height);
    SetPosition(x, y);
    GRAPHICS->SetSpriteFlip(false, false);

    if (m_whiteTextureID == -1)
    {
      m_whiteTextureID = GRAPHICS->GetTextureID("White.png");
      new (&m_whiteHandle) DirectSheep::Handle(DirectSheep::TEXTURE, m_whiteTextureID);
    }
    
    GRAPHICS->SetUV(Vec2(0, 0), Vec2(1, 1));

    GRAPHICS->BindTexture(m_whiteTextureID);
    GRAPHICS->RawDraw();
    //GRAPHICS->DrawBatched(m_whiteHandle);
  }

  void Draw::DrawTexturedRect(float x, float y, float width, float height)
  {
    GRAPHICS->SetSize(width, height);
    SetPosition(x, y);
    GRAPHICS->SetSpriteFlip(false, false);
    
//     GRAPHICS->BindTexture(m_TextureID);
//     GRAPHICS->RawDraw();
    GRAPHICS->SetUV(Vec2(0, 0), Vec2(1, 1));
    GRAPHICS->DrawBatched(DirectSheep::Handle(DirectSheep::TEXTURE, m_TextureID));
  }

  void Draw::DrawTexturedRectRotated(float x, float y, float width, float height, float theta)
  {
    GRAPHICS->SetSize(width, height);
    SetPosition(x, y);
    GRAPHICS->SetSpriteFlip(false, false);
    //GRAPHICS->BindTexture(m_TextureID);


    GRAPHICS->SetUV(Vec2(0, 0), Vec2(1, 1));
    GRAPHICS->SetRotation(theta);
    GRAPHICS->DrawBatched(DirectSheep::Handle(DirectSheep::TEXTURE, m_TextureID));
    
    GRAPHICS->SetRotation(0);
  }

  void Draw::DrawLine(float sX, float sY, float eX, float eY, float width/* =1 */)
  {
    float diffX = eX - sX;
    float diffY = eY - sY;


    GRAPHICS->SetSize(sqrt(diffX * diffX + diffY * diffY), width);

    if (m_whiteTextureID == -1)
    {
      m_whiteTextureID = GRAPHICS->GetTextureID("White.png");
      new (&m_whiteHandle) DirectSheep::Handle(DirectSheep::TEXTURE, m_whiteTextureID);
    }

    //GRAPHICS->BindTexture(m_whiteTextureID);

    float rotation = (float)Framework::PI / 2.0f;
    if (diffX != 0)
      rotation = atan(diffY / diffX);
     
    float offsetX = diffX * cos(rotation) - diffY * sin(rotation);
    float offsetY = diffX * sin(rotation) - diffY * cos(rotation);

    GRAPHICS->SetRotation(rotation);

    if (m_useCamera)
      SetPosition(sX + diffX / 2, sY + diffY / 2);
    else
      SetPosition(sX, sY);

    GRAPHICS->SetUV(Vec2(0, 0), Vec2(1, 1));

    GRAPHICS->DrawBatched(m_whiteHandle);

  }

  void Draw::DrawCircle(float x, float y, float radius)
  {
    const int numLines = 32;

    float theta = 0;

    float lineAngle = (2.0f * (float)Framework::PI) / (float)numLines;

    Vec3D p2(cosf(theta), sinf(theta));

    p2 *= radius;
    p2 += Vec3D(x, y);

    Vec3D p1 = p2;
    for (int i = 0; i < numLines; ++i)
    {
      theta += lineAngle;

      p2 = Vec3D(cosf(theta), sinf(theta));

      p2 *= radius;
      p2 += Vec3D(x, y);

      DrawLine(p1.X, p1.Y, p2.X, p2.Y);

      p1 = p2;
    }
  }

  void Draw::DrawString(const char* text, float size, const char* font)
  {
    GRAPHICS->DrawSpriteText(text, size, font);
  }

  Vec3 Draw::ToWorld(Vec2 screenPos)
  {
    return Vec3((float*)&GRAPHICS->RetrieveCamera(GRAPHICS->GetActiveCamera())
      ->ToWorld(DirectSheep::Vec2((float*)&screenPos)));
  }

  Vec2 Draw::ToScreen(Vec3 worldPos)
  {
    return Vec2((float*)&GRAPHICS->RetrieveCamera(GRAPHICS->GetActiveCamera())
      ->ToScreen(DirectSheep::Vec3((float*)&worldPos)));
  }



  int Draw::ScreenHeight(void)
  {
    return GRAPHICS->_ScreenHeight;
  }

  int Draw::ScreenWidth(void)
  {
    return GRAPHICS->_ScreenWidth;
  }

  Vec2 Draw::MeasureString(const char* text, float size, const char* font)
  {
    return GRAPHICS->MeasureString(text, size, font);
  }
  
}