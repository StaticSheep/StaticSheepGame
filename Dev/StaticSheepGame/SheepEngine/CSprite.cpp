#include "CSprite.h"
#include "SheepGraphics.h"

namespace Framework
{

  Sprite::Sprite()
    :m_Color(1, 1, 1, 1)
  {
    transform = NULL;
  }

  Sprite::~Sprite()
  {
  }

  void Sprite::Initialize()
  {
    transform = this->GetOwner()->GetComponentHandle(eTransform);

    if (this->SpriteName.length() == 0)
      this->SpriteName = "content/bricks.png";

    SetTexture(this->SpriteName);
    //TODO not sure if we have a GetOwner()->has working
    space->hooks.Add("Draw", self, BUILD_FUNCTION(Sprite::Draw));
  }

  void Sprite::Remove()
  {
    space->hooks.Remove("Draw", self);
  }

  void Sprite::Draw()
  {
    Transform* trans = space->GetHandles().GetAs<Transform>(transform);

    GRAPHICS->SetPosition(trans->translation.X, trans->translation.Y);

    GRAPHICS->SetRotation(trans->rotation);
    GRAPHICS->SetSize(m_Size.X, m_Size.Y);
    GRAPHICS->SetTexture(SpriteID);
    GRAPHICS->DrawSprite();
  }

  void Sprite::SetTexture(std::string texture)
  {
    SpriteName = texture;
    SpriteID = GRAPHICS->GetTextureID(texture);
  }
}