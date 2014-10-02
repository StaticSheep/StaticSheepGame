#pragma once

#include "CTransform.h"
#include "CSprite.h"

namespace Framework
{
  class Transform;

  class AniSprite : public Sprite
  {
  public:

    AniSprite();
    ~AniSprite();

    virtual void Initialize();
    virtual void Remove();

    DirectSheep::Handle& SetTexture(const std::string& Texture);
    void Draw(void);
    void UpdateUV(void);

    Vec2 frames;
    Vec2 currFrame;

  private:
    Vec2 uvBegin;
    Vec2 uvEnd;

    
  };
}