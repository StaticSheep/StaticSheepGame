


#include "pch/precompiled.h"
#include "CSprite.h"
#include "systems/graphics/SheepGraphics.h"
#include "SheepGraphics\api.h"

namespace Framework
{

  Sprite::Sprite()
    :Color(1, 1, 1, 1), Size(1, 1)
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
      m_spriteName = "content/Default.png";

    if(m_texture.GetType() == DirectSheep::NONE)
    {
      SetTexture(m_spriteName.c_str());
    }
    else
      SetTexture(m_spriteName.c_str());
    
    //TODO not sure if we have a GetOwner()->has working
    space->hooks.Add("Draw", self, BUILD_FUNCTION(Sprite::Draw));
  }

  void Sprite::SetTexture(const char * Texture)
  {
    m_spriteName = Texture;
    m_texture = GRAPHICS->SetTexture(Texture);
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

  void Sprite::Remove()
  {
    space->hooks.Remove("Draw", self);
  }

  void Sprite::Draw()
  {
    Transform* trans = space->GetHandles().GetAs<Transform>(transform);

    GRAPHICS->SetPosition(trans->GetTranslation().X, trans->GetTranslation().Y);
    GRAPHICS->SetRotation(trans->GetRotation());
    GRAPHICS->SetSize(trans->GetScale().X * Size.X, trans->GetScale().Y * Size.Y);
    GRAPHICS->SetColor(Color);
    GRAPHICS->SetUV(Vec2(0,0), Vec2(1,1));
    GRAPHICS->SetUseCamera(true);

    GRAPHICS->DrawSprite(this);
    
  }
}