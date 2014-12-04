/******************************************************************************
Filename: CAniSprite.h
Project:
Author(s): Scott Nelson (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
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

    // sets row and column count
    void SetFrames(Vec2 frames);

    // Plays animation in current range of frames for number of loops, if negative loop forever
    void Play(float loops = -1);

    // Define Start and end frames along with loops and framerate
    void PlayEx(unsigned start, unsigned end, int loops = -1, float framerate = 20);

    // Check for if animation is in a stopped state
    bool Stopped(void);

    // pause/unpause animation
    void Pause(bool ispaused);

    // checks if ani is paused
    bool Paused();

    // Sets range for start and end frames
    void SetRange(unsigned start, unsigned end);

    // Sets range for start and end frames
    void SetRange(Vec2 range);

    // Sets frame rate of AniSprite
    void SetFrameRate(float framerate);

    // AniSprite Draw Call
    void Draw(void);

    void Test(void* value);

    void UpdateFramePosition();

    void TweakStartFrame(const void* start);
    void TweakEndFrame(const void* end);

    Vec2 m_frames;

    float m_frameRate;

    unsigned m_currFrame;
    unsigned m_startFrame;
    unsigned m_endFrame;
    int m_loop;
    bool m_paused;

    Vec2 uvBegin;
    Vec2 uvEnd;

  private:
    float m_time;

    int m_numFramesX;
    int m_numFramesY;

    float m_frameWidth;
    float m_frameHeight;

    Vec2 m_framePos;
    Vec2 m_startFramePos;

    void CheckNextFrame();

    void UpdateUV(void);
    
  };
}