#pragma once

#include "components/transform/CTransform.h"
#include "components/sprites/CSprite.h"

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

    Vec2 m_frames;

    float m_frameRate;

    int m_currFrame;
    int m_startFrame;
    int m_endFrame;

  private:
    Vec2 uvBegin;
    Vec2 uvEnd;

    float m_time;

    int m_numFramesX;
    int m_numFramesY;

    float m_frameWidth;
    float m_frameHeight;

    Vec2 m_framePos;
    Vec2 m_startFramePos;

    void CheckNextFrame();
    
  };
}