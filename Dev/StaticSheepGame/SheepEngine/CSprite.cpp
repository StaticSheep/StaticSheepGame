#include "CSprite.h"
#include "SheepGraphics.h"
#include "graphics\api.h"

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

    if(m_texture.GetType() == DirectSheep::NONE)
    {
      SetTexture("content/circle.png");
      SpriteName = "content/circle.png";
    }
    else
      SetTexture(SpriteName);
    
    //TODO not sure if we have a GetOwner()->has working
    space->hooks.Add("Draw", self, BUILD_FUNCTION(Sprite::Draw));
  }

  DirectSheep::Handle& Sprite::SetTexture(const std::string& Texture)
  {
    m_texture = GRAPHICS->SetTexture(Texture);
    TextureSize = GRAPHICS->GetTextureDim(m_texture);
    return m_texture;
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
    GRAPHICS->SetSize(trans->GetScale().X * Size.X * TextureSize.X, trans->GetScale().Y * Size.Y * TextureSize.Y);
    GRAPHICS->SetColor(Color);
    GRAPHICS->SetUV(Vec2(0,0), Vec2(1,1));

    GRAPHICS->DrawSprite(this);
    
  }
}