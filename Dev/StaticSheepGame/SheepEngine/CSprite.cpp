#include "CSprite.h"
#include "SheepGraphics.h"

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

    GRAPHICS->SetPosition(trans->Translation.X, trans->Translation.Y);

    GRAPHICS->SetRotation(trans->Rotation);
    GRAPHICS->SetSize(Size.X * trans->Scale.X, Size.Y * trans->Scale.Y);
    GRAPHICS->SetTexture(SpriteID);
    GRAPHICS->SetColor(Color);
    GRAPHICS->DrawSprite();
  }

  void Sprite::SetTexture(std::string texture)
  {
    SpriteName = texture;
    SpriteID = GRAPHICS->GetTextureID(texture);
  }
}