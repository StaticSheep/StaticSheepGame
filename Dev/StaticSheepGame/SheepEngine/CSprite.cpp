#include "CSprite.h"
#include "SheepGraphics.h"
#include "graphics\api.h"
#include "graphics\Handle.h"

namespace Framework
{

  Sprite::Sprite()
    :Color(1, 1, 1, 1), Size(32, 32)
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
      SetTexture("content/maverick.jpg");
      SpriteName = "content/maverick.jpg";
    }
    else
      SetTexture(SpriteName);
    
    //TODO not sure if we have a GetOwner()->has working
    space->hooks.Add("Draw", self, BUILD_FUNCTION(Sprite::Draw));
  }

  DirectSheep::Handle& Sprite::SetTexture(const std::string& Texture)
  {
   return m_texture = GRAPHICS->SetTexture(Texture);
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

    GRAPHICS->SetPosition(trans->Translation.X, trans->Translation.Y);
    GRAPHICS->SetRotation(trans->Rotation);
    GRAPHICS->SetSize(trans->Scale.X, trans->Scale.Y);
    GRAPHICS->SetColor(Color);

    GRAPHICS->DrawSprite(this);
    
  }
}