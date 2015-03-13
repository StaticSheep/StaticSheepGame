/******************************************************************************
Filename: CSprite.cpp
Project:
Author(s): Scott Nelson (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "pch/precompiled.h"
#include "CSprite.h"
#include "systems/graphics/SheepGraphics.h"

namespace Framework
{

  Sprite::Sprite()
    :Color(1, 1, 1, 1), Size(1, 1), MaxUV(1, 1), m_uvScale(false), m_origin(0,0)
  {
    transform = NULL;
  }

  Sprite::~Sprite()
  {
  }

  void Sprite::Initialize()
  {
    transform = this->GetOwner()->GetComponentHandle(eTransform);

    if (m_spriteName.length() == 0)
      m_spriteName = "Default.png";

    if(m_texture.GetType() == DirectSheep::NONE)
    {
      SetTexture(m_spriteName.c_str());
    }
    else
      SetTexture(m_spriteName.c_str());
    
    //TODO not sure if we have a GetOwner()->has working

    if (m_hooked)
    {
      space->hooks.Remove("PreDraw", self);
      space->hooks.Remove("Draw", self);
      space->hooks.Remove("PostDraw", self);
    }

    switch (m_layer)
    {
    case 2:
      space->hooks.Add("PostDraw", self, BUILD_FUNCTION(Sprite::Draw));
      break;
    case 0:
      space->hooks.Add("PreDraw", self, BUILD_FUNCTION(Sprite::Draw));
      break;
    case 1:
    default:
      space->hooks.Add("Draw", self, BUILD_FUNCTION(Sprite::Draw));
      break;
    }

    m_hooked = true;
    
  }

  void Sprite::SetTexture(const char * Texture)
  {
    m_spriteName = Texture;
    m_texture = GRAPHICS->LoadTexture(Texture);
    TextureSize = GRAPHICS->GetTextureDim(m_texture);
  }

  void Sprite::TweakSetTexture(const void * Texture)
  {
    SetTexture(((std::string *)Texture)->c_str());
  }

  DirectSheep::Handle& Sprite::GetTexture()
  {
    return m_texture;
  }

  void Sprite::SetFlipX(bool isFlipped)
  {
    m_flipX = isFlipped;
  }

  void Sprite::SetFlipY(bool isFlipped)
  {
    m_flipY = isFlipped;
  }

  bool Sprite::GetFlipX(void)
  {
    return m_flipX;
  }

  bool Sprite::GetFlipY(void)
  {
    return m_flipY;
  }

  void Sprite::Remove()
  {

    switch (m_layer)
    {
    case 2:
      space->hooks.Remove("PostDraw", self);
      break;
    case 0:
      space->hooks.Remove("PreDraw", self);
      break;
    case 1:
    default:
      space->hooks.Remove("Draw", self);
      break;
    }
    
  }

  void Sprite::Draw()
  {
    Transform* trans = space->GetHandles().GetAs<Transform>(transform);

    GRAPHICS->SetPosition(trans->GetTranslation().X,
      trans->GetTranslation().Y, trans->GetTranslation().Z);

    GRAPHICS->SetRotation(trans->GetRotation());

    GRAPHICS->SetSize(trans->GetScale().X * Size.X,
      trans->GetScale().Y * Size.Y);

    GRAPHICS->SetColor(Color);

    if (m_uvScale)
    {
      GRAPHICS->SetSize(Size.X,
        Size.Y);

      GRAPHICS->SetObjectOrigin(m_origin.x * trans->GetScale().X,
        m_origin.y * trans->GetScale().Y);

      GRAPHICS->SetUV(MinUV, Vec2(MaxUV.x * trans->GetScale().X,
        MaxUV.y * trans->GetScale().y));
    }
    else
    {
      GRAPHICS->SetSize(trans->GetScale().X * Size.X,
        trans->GetScale().Y * Size.Y);

      GRAPHICS->SetObjectOrigin(m_origin.x, m_origin.y);

      GRAPHICS->SetUV(MinUV, MaxUV);
    }

    GRAPHICS->SetSpriteFlip(m_flipX, m_flipY);
    GRAPHICS->SetCamState(0);

    GRAPHICS->DrawBatched(this->m_texture);

    GRAPHICS->SetObjectOrigin(0, 0);
    
  }
}