/*****************************************************************
Filename: DrawLib.cpp
Project: 
Author(s): Scott Nelson(primary)
           Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "SheepGraphics.h"
#include <math.h>

namespace Framework
{
  bool Draw::m_useCamera = true;
  Vec4 Draw::m_Color;
  Vec2 Draw::m_TextPos;
  UINT Draw::m_TextureID;

  void Draw::SetUseCamera(bool useCam)
  {
    Draw::m_useCamera = useCam;
    GRAPHICS->SetUseCamera(useCam);
  }

  void Draw::SetTexture(UINT texID)
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
    GRAPHICS->SetPosition(x, y);
  }

  void Draw::SetColor(float r, float g, float b, float a)
  {
    new (&Draw::m_Color) Vec4(r, g, b, a);
    GRAPHICS->SetColor(Draw::m_Color);
  }

  void Draw::DrawRect(float x, float y, float width, float height)
  {
    GRAPHICS->SetSize(width, height);
    GRAPHICS->SetPosition(x, y);

  }

  void Draw::DrawTexturedRect(float x, float y, float width, float height)
  {
    GRAPHICS->SetSize(width, height);

    GRAPHICS->SetPosition(x, y);

  }

  void Draw::DrawLine(float sX, float sY, float eX, float eY, float width/* =1 */)
  {
    float diffX = eX - sX;
    float diffY = eY - sY;

    GRAPHICS->SetPosition(sX + diffX / 2, sY + diffY / 2);
    GRAPHICS->SetSize(sqrt(diffX * diffX + diffY * diffY), width);

    GRAPHICS->SetRotation(-atan(diffY / diffX));

  }

  void Draw::DrawString(const char* text, float size, const char* font)
  {
    GRAPHICS->DrawSpriteText(text, size, font);
  }
}