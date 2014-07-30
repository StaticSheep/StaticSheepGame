#include "CSprite.h"
#include "graphics\sheep_graphics.h"

namespace Framework
{

  Sprite::Sprite()
  {
    transform = NULL;
  }

  Sprite::~Sprite()
  {
  }

  void Sprite::Initialize()
  {
    transform = this->GetOwner()->GetComponentHandle(eTransform);

    //TODO not sure if we have a GetOwner()->has working
    space->hooks.Add("Draw", self, BUILD_FUNCTION(Sprite::Draw));
  }

  void Sprite::Draw()
  {
    // GRAPHICS->SetPosition(x, y)
    // GRAPHICS->SetRotation(p, y)
    // GRAPHICS->SetSize(x, y)
    // GRAPHICS->SetTexture(textureID/something)
    // GRAPHICS->DrawSprite()

    //DirectSheep::GFX_Draw(Size, transform->position, transform->rotation);
  }
}