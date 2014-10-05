#include "CAniSprite.h"
#include "SheepGraphics.h"
#include "graphics\api.h"
#include "graphics\Handle.h"

namespace Framework
{

  AniSprite::AniSprite() : uvBegin(0,0), uvEnd(1,1), frames(4,3), currFrame(0,0)
  {
    transform = NULL;
  }

  void AniSprite::Initialize()
  {
    transform = this->GetOwner()->GetComponentHandle(eTransform);

    if(m_texture.GetType() == DirectSheep::NONE)
    {
      SetTexture("content/thiefRun.png");
      m_spriteName = "content/thiefRun.png";
    }
    else
      SetTexture(m_spriteName.c_str());
    
    //TODO not sure if we have a GetOwner()->has working
    space->hooks.Add("Draw", self, BUILD_FUNCTION(AniSprite::Draw));
  }

  DirectSheep::Handle& AniSprite::SetTexture(const std::string& Texture)
  {
    m_texture = GRAPHICS->SetTexture(Texture);
    TextureSize = GRAPHICS->GetTextureDim(m_texture);
    return m_texture;
  }

  void AniSprite::UpdateUV(void)
  {
    float width;
    float height;
    float offsetX;
    float offsetY;

    width = (1 / frames.X);
    height = (1 / frames.Y);
    offsetX = width * (int)currFrame.X;
    offsetY = height * (int)currFrame.Y;

    if(currFrame.x_ < 4)
      currFrame.x_ += .1f;
    else
    {
      currFrame.y_++;
      currFrame.x_ = 0;
    }
    uvBegin = Vec2(offsetX, offsetY);
    uvEnd = Vec2(offsetX + width, offsetY + height);
    
  }

  void AniSprite::Remove(void)
  {
    space->hooks.Remove("Draw", self);
  }

  void AniSprite::Draw()
  {
    Transform* trans = space->GetHandles().GetAs<Transform>(transform);

    UpdateUV();
    GRAPHICS->SetPosition(trans->GetTranslation().X, trans->GetTranslation().Y);
    GRAPHICS->SetRotation(trans->GetRotation());
    GRAPHICS->SetSize(trans->GetScale().X * Size.X * (TextureSize.X / frames.X), trans->GetScale().Y * Size.Y * (TextureSize.Y / frames.Y));
    GRAPHICS->SetColor(Color);
    GRAPHICS->SetUV(uvBegin, uvEnd);
    GRAPHICS->SetUseCamera(true);

    GRAPHICS->DrawSprite(this);
    
  }

  AniSprite::~AniSprite()
  {
  }

}