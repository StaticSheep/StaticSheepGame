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
    //GRAPHICS->SetTexture(texID);
  }

  void Draw::SetColor(Vec4& color)
  {
    Draw::m_Color = color;
    //GRAPHICS->SetColor(color);
  }

  void Draw::SetColor(float r, float g, float b, float a/* =255 */)
  {
    new (&Draw::m_Color) Vec4(r, g, b, a);
    //GRAPHICS->SetColor(Draw::m_Color);
  }

  void Draw::DrawRect(float x, float y, float width, float height)
  {
    if (Draw::m_useCamera)
      GRAPHICS->SetPosition(x + width / 2, y - height / 2);
    else
      GRAPHICS->SetPosition(x + width / 2, y + height / 2);

    GRAPHICS->SetSize(width, height);
    GRAPHICS->SetRotation(0.0f);

    //GRAPHICS->SetUseCamera(m_useCamera);

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
}