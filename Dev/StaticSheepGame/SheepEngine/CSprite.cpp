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

  void Sprite::Draw()
  {
    time += 0.001f;

    Transform* trans = space->GetHandles().GetAs<Transform>(transform);

    GRAPHICS->SetPosition(trans->position.x, trans->position.y);
    GRAPHICS->SetRotation(time);
    GRAPHICS->SetSize(128.0f * 1.6f, 100.0f * 1.6f);
    GRAPHICS->SetTexture("content/test.png");
    GRAPHICS->DrawSprite();

    /*GRAPHICS->SetPosition(transform., y);
    GRAPHICS->SetRotation(p, y);
    GRAPHICS->SetSize(x, y);
    GRAPHICS->SetTexture(textureID/something);
    GRAPHICS->DrawSprite();*/
  }
}