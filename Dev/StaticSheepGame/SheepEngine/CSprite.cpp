#include "CSprite.h"
#include "SheepGraphics.h"

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

  void Sprite::Remove()
  {
    if (space != nullptr)
    {
      space->hooks.Remove("Draw", self);
    }
    
    SpriteName.~basic_string();
  }

  void Sprite::Draw()
  {
    time += 0.001f;

    Transform* trans = space->GetHandles().GetAs<Transform>(transform);

    GRAPHICS->SetPosition(trans->translation.X, trans->translation.Y);

    GRAPHICS->SetRotation(-time);
    GRAPHICS->SetSize(200, 200);
    GRAPHICS->SetTexture("content/bricks.png");
    GRAPHICS->DrawSprite();
  }
}