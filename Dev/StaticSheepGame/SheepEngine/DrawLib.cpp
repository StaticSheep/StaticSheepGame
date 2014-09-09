/*****************************************************************
Filename: DrawLib.cpp
Project: 
Author(s): Zachary Nawar

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
    GRAPHICS->SetTexture(texID);
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

  //void Draw::SetColor(Vec4& color)
  //{
  //  Draw::m_Color = color;
  //  //GRAPHICS->SetColor(color);
  //}

  void Draw::SetColor(float r, float g, float b, float a)
  {
    new (&Draw::m_Color) Vec4(r, g, b, a);
    GRAPHICS->SetColor(Draw::m_Color);
  }

  //void Draw::SetTextPos(Vec2& pos)
  //{
  //  Draw::m_TextPos = pos;
  //}

  void Draw::SetTextPos(float x, float y)
  {
    new (&Draw::m_TextPos) Vec2(x, y);
  }



  void Draw::DrawRect(float x, float y, float width, float height)
  {
    GRAPHICS->SetSize(width, height);
    GRAPHICS->SetTexture(GRAPHICS->GetTextureID(std::string("content/White.png")));
    GRAPHICS->SetPosition(x, y);

    GRAPHICS->DrawSprite();
  }

  void Draw::DrawTexturedRect(float x, float y, float width, float height)
  {
    GRAPHICS->SetSize(width, height);

    GRAPHICS->SetPosition(x, y);

    GRAPHICS->DrawSprite();
  }

  void Draw::DrawLine(float sX, float sY, float eX, float eY, float width/* =1 */)
  {
    float diffX = eX - sX;
    float diffY = eY - sY;

    GRAPHICS->SetPosition(sX + diffX / 2, sY + diffY / 2);
    GRAPHICS->SetSize(sqrt(diffX * diffX + diffY * diffY), width);

    GRAPHICS->SetRotation(-atan(diffY / diffX));

    GRAPHICS->DrawSprite();
  }

  void Draw::DrawString(const char* text, float size, const char* font)
  {
    GRAPHICS->DrawSpriteText(text, size, font);
  }
}