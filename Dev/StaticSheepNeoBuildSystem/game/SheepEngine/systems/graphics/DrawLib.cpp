/*****************************************************************
Filename: DrawLib.cpp
Project: 
Author(s): Scott Nelson(primary)
           Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "SheepGraphics.h"
#include <math.h>

namespace Framework
{
  bool Draw::m_useCamera = true;
  Vec4 Draw::m_Color;
  Vec2 Draw::m_TextPos;
  unsigned Draw::m_TextureID;

  void Draw::SetUseCamera(bool useCam)
  {
    Draw::m_useCamera = useCam;
    GRAPHICS->SetUseCamera(useCam);
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

  void Draw::SetRotation(float theta)
  {
    GRAPHICS->SetRotation(theta);
  }

  void Draw::SetPosition(float x, float y)
  {
    GRAPHICS->SetPosition(x, y, 0.0f);
  }

  void Draw::SetColor(float r, float g, float b, float a)
  {
    new (&Draw::m_Color) Vec4(r, g, b, a);
    GRAPHICS->SetColor(Draw::m_Color);
  }

  void Draw::DrawRect(float x, float y, float width, float height)
  {
    GRAPHICS->UpdateCamera();
    GRAPHICS->SetSize(width, height);
    GRAPHICS->SetPosition(x, y, 0.0f);
    GRAPHICS->BindTexture(GRAPHICS->GetTextureID("White.png"));
    GRAPHICS->RawDraw();
  }

  void Draw::DrawTexturedRect(float x, float y, float width, float height)
  {
    GRAPHICS->UpdateCamera();
    GRAPHICS->SetSize(width, height);

    GRAPHICS->SetPosition(x, y, 0.0f);

    GRAPHICS->BindTexture(m_TextureID);

    GRAPHICS->RawDraw();

  }

  void Draw::DrawLine(float sX, float sY, float eX, float eY, float width/* =1 */)
  {
    float diffX = eX - sX;
    float diffY = eY - sY;
    GRAPHICS->UpdateCamera();
    GRAPHICS->SetSize(sqrt(diffX * diffX + diffY * diffY), width);
    GRAPHICS->BindTexture(GRAPHICS->GetTextureID("White.png"));

    float rotation = (float)Framework::PI / 2.0f;
    if (diffX != 0)
      rotation = atan(diffY / diffX);
     
    float offsetX = diffX * cos(rotation) - diffY * sin(rotation);
    float offxetY = diffX * sin(rotation) - diffY * cos(rotation);

    GRAPHICS->SetRotation(rotation);

    GRAPHICS->SetPosition(sX + diffX / 2, sY + diffY / 2, 0.0f);

    GRAPHICS->RawDraw();

  }

  void Draw::DrawCircle(float x, float y, float radius)
  {
    const int numLines = 128;

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
    GRAPHICS->UpdateCamera();
    GRAPHICS->DrawSpriteText(text, size, font);
  }
}