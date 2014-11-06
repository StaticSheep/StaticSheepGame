#pragma once

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
    void SetFrames(Vec2 frames);
    void TweakSetFrames(const void* frames);
    void Draw(void);
    void UpdateUV(void);
    void Paused(bool ispaused);
    void Play(unsigned start, unsigned end, int loop = -1, unsigned framerate = 20);



    Vec2 m_frames;

    float m_frameRate;

    unsigned m_currFrame;
    unsigned m_startFrame;
    unsigned m_endFrame;

  
    Vec2 uvBegin;
    Vec2 uvEnd;

  private:
    float m_time;

    bool m_paused;

    unsigned m_loop;

    int m_numFramesX;
    int m_numFramesY;

    float m_frameWidth;
    float m_frameHeight;

    Vec2 m_framePos;
    Vec2 m_startFramePos;

    void CheckNextFrame();
    
  };
}