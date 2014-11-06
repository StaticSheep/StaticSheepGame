#include "pch/precompiled.h"
#include "CAniSprite.h"

#include "systems/graphics/SheepGraphics.h"
#include "SheepGraphics\api.h"
#include "SheepGraphics\Handle.h"

namespace Framework
{

  AniSprite::AniSprite() : uvBegin(0, 0), uvEnd(1, 1), m_frames(1, 1), m_currFrame(0),
    m_endFrame(0),
    m_startFrame(0),
    m_startFramePos(0, 0),
    m_frameRate(20),
    m_paused(false),
    m_loop(-1),
    m_time(0),
    m_frameWidth(1 / m_frames.X),
    m_frameHeight(1 / m_frames.Y)
  {
    transform = NULL;
  }

  void AniSprite::Initialize()
  {
    transform = this->GetOwner()->GetComponentHandle(eTransform);

    if (m_spriteName.length() == 0)
      m_spriteName = "spritesheet.png";

    if(m_texture.GetType() == DirectSheep::NONE)
    {
      SetTexture(m_spriteName.c_str());
    }
    else
      SetTexture(m_spriteName.c_str());
    
    //TODO not sure if we have a GetOwner()->has working
    space->hooks.Add("Draw", self, BUILD_FUNCTION(AniSprite::Draw));
  }

  DirectSheep::Handle& AniSprite::SetTexture(const std::string& Texture)
  {
    m_spriteName = Texture;
    m_texture = GRAPHICS->SetTexture(Texture);
    TextureSize = GRAPHICS->GetTextureDim(m_texture);

    m_time = 0;

    m_framePos = Vec2(0, 0);
    
    return m_texture;
  }

  void AniSprite::SetFrames(Vec2 frames)
  {
    m_frames = frames;

    m_frameWidth = 1 / m_frames.X;
    m_frameHeight = 1 / m_frames.Y;
  }

  void AniSprite::Paused(bool ispaused)
  {
    m_paused = ispaused;
  }

  void AniSprite::Play(unsigned start, unsigned end, int loop, unsigned framerate)
  {
    m_paused = false;
    m_startFrame = start;
    m_endFrame = end;
    m_frameRate = framerate;
    m_loop = loop;
  }

  void AniSprite::UpdateUV(void)
  {
    float offsetX;
    float offsetY;
    m_frameWidth = 1 / m_frames.X;
    m_frameHeight = 1 / m_frames.Y;
    offsetX = m_frameWidth * m_framePos.X;
    offsetY = m_frameHeight * m_framePos.Y;

    uvBegin = Vec2(offsetX, offsetY);
    uvEnd = Vec2(offsetX + m_frameWidth, offsetY + m_frameHeight);
  }

  void AniSprite::Remove(void)
  {
    space->hooks.Remove("Draw", self);
  }

  void AniSprite::CheckNextFrame()
  {
    if (m_paused || m_loop == 0)
      return;

    float dt = ENGINE->Framerate.GetDT();

    m_time += dt;

    if (m_time > 1 / m_frameRate)
    {
      m_time -= 1 / m_frameRate;
      m_currFrame += 1;

      if (m_currFrame > m_endFrame)
      {
        m_currFrame = m_startFrame;
        m_framePos = m_startFramePos;
        if (m_loop)
        --m_loop;
        return;
      }

      m_framePos.X += 1;

      if (m_framePos.X > m_frames.X)
      {
        m_framePos.X = 0;
        m_framePos.Y += 1;
      }
      
    }
    
  }

  void AniSprite::Draw()
  {
    if (m_loop == 0)
      return;

    Transform* trans = space->GetHandles().GetAs<Transform>(transform);

    UpdateUV();
    GRAPHICS->SetPosition(trans->GetTranslation().X, trans->GetTranslation().Y, trans->GetTranslation().Z);
    GRAPHICS->SetRotation(trans->GetRotation());
    GRAPHICS->SetSize(trans->GetScale().X * Size.X, trans->GetScale().Y * Size.Y);
    GRAPHICS->SetColor(Color);
    GRAPHICS->SetUV(uvBegin, uvEnd);
    GRAPHICS->SetUseCamera(true);

    GRAPHICS->DrawSprite(this);

    CheckNextFrame();
  }

  AniSprite::~AniSprite()
  {
  }

}