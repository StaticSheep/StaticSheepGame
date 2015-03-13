/******************************************************************************
Filename: CAniSprite.cpp
Project:
Author(s): Scott Nelson (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "pch/precompiled.h"
#include "CAniSprite.h"

#include "systems/graphics/SheepGraphics.h"
#include "Handle.h"

#include "systems\anttweak\TweakHelper.h"

namespace Framework
{

  /*****************************************************************************/
  /*!
      \brief
      Default constructor for AniSprite Component
  */
  /*****************************************************************************/
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

  /*****************************************************************************/
  /*!
      \brief
      Initializes AniSprite with texture
  */
  /*****************************************************************************/
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
    
    UpdateFramePosition();

    if (m_hooked)
    {
      space->hooks.Remove("PreDraw", self);
      space->hooks.Remove("Draw", self);
      space->hooks.Remove("PostDraw", self);

      space->hooks.Remove("LogicUpdate", self);
      space->hooks.Remove("FrameUpdate", self);
    }

    switch (m_layer)
    {
    case 2:
      space->hooks.Add("PostDraw", self, BUILD_FUNCTION(AniSprite::Draw));
      break;
    case 0:
      space->hooks.Add("PreDraw", self, BUILD_FUNCTION(AniSprite::Draw));
      break;
    case 1:
    default:
      space->hooks.Add("Draw", self, BUILD_FUNCTION(AniSprite::Draw));
      break;
    }

    switch (m_frameUpdate)
    {
    case true:
      space->hooks.Add("FrameUpdate", self, BUILD_FUNCTION(AniSprite::CheckNextFrame));
      break;
    case false:
    default:
      space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(AniSprite::CheckNextFrame));
      break;
    }

    m_hooked = true;
  }

  /*****************************************************************************/
  /*!
      \brief
        Binds texture to AniSprite
  */
  /*****************************************************************************/
  DirectSheep::Handle& AniSprite::SetTexture(const std::string& Texture)
  {
    m_spriteName = Texture;
    m_texture = GRAPHICS->LoadTexture(Texture);
    TextureSize = GRAPHICS->GetTextureDim(m_texture);

    m_time = 0;

    m_framePos = Vec2(0, 0);
    
    return m_texture;
  }

  /*****************************************************************************/
  /*!
      \brief
        Sets row and column count for AniSprite
  */
  /*****************************************************************************/
  void AniSprite::SetFrames(Vec2 frames)
  {
    m_frames = frames;

    m_frameWidth = 1 / m_frames.X;
    m_frameHeight = 1 / m_frames.Y;
  }

  /*****************************************************************************/
  /*!
      \brief
        Pauses AniSprites sequence
  */
  /*****************************************************************************/
  void AniSprite::Pause(bool ispaused)
  {
    m_paused = ispaused;
  }

  bool AniSprite::Paused()
  {
    return m_paused;
  }

  /*****************************************************************************/
  /*!
      \brief
        Check to see if AniSprite is in stopped state
  */
  /*****************************************************************************/
  bool AniSprite::Stopped(void)
  {
    return (m_currFrame == m_startFrame && m_paused);
  }

  /*****************************************************************************/
  /*!
      \brief
        Sets start frame and end frame range for AniSprite
  */
  /*****************************************************************************/
  void AniSprite::SetRange(unsigned start, unsigned end)
  {
    m_startFrame = start;
    m_endFrame = end;
    UpdateFramePosition();
  }

  /*****************************************************************************/
  /*!
      \brief
        Sets start frame and end frame range for AniSprite
  */
  /*****************************************************************************/
  void AniSprite::SetRange(Vec2 range)
  {
    m_startFrame = (unsigned)range.X;
    m_endFrame = (unsigned)range.Y;
    UpdateFramePosition();
  }

  void AniSprite::TweakStartFrame(const void* start)
  {
    m_startFrame = *(int*)start;
    UpdateFramePosition();
  }
  void AniSprite::TweakEndFrame(const void* end)
  {
    m_endFrame = *(int*)end;
    UpdateFramePosition();
  }

  /*****************************************************************************/
  /*!
      \brief
        Plays anisprite "loops" number of time, if loops < 0 then loop forever
  */
  /*****************************************************************************/
  void AniSprite::Play(float loops)
  {
    m_paused = false;
    m_loop = (unsigned)loops;
  }

  /*****************************************************************************/
  /*!
      \brief
        Adds onto play with setting range of frames and framerate
  */
  /*****************************************************************************/
  void AniSprite::PlayEx(unsigned start, unsigned end, int loops, float framerate)
  {
    m_paused = false;
    m_startFrame = start;
    m_endFrame = end;
    SetFrameRate(framerate);
    m_loop = loops;
  }

  /*****************************************************************************/
  /*!
      \brief
        Sets framerate for Anisprite, cannot be <= 0
  */
  /*****************************************************************************/
  void AniSprite::SetFrameRate(float framerate)
  {
    if (framerate <= 0)
      framerate = 1;

    m_frameRate = framerate;
  }

  /*****************************************************************************/
  /*!
      \brief
        Updates UV coordinates to be passed to graphics engine
  */
  /*****************************************************************************/
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

  void AniSprite::UpdateFramePosition()
  {
    m_currFrame = m_startFrame;
    m_startFramePos = Vec2(float(m_currFrame % (int)m_frames.x), float(m_currFrame / (int)m_frames.x));
    m_framePos = m_startFramePos;
  }

  /*****************************************************************************/
  /*!
      \brief
        Removes AniSrite from listening to draw message
  */
  /*****************************************************************************/
  void AniSprite::Remove(void)
  {
    switch (m_layer)
    {
    case 2:
      space->hooks.Remove("PostDraw", self);
      break;
    case 0:
      space->hooks.Remove("PreDraw", self);
      break;
    case 1:
    default:
      space->hooks.Remove("Draw", self);
      break;
    }

    if (m_frameUpdate)
      space->hooks.Remove("FrameUpdate", self);
    else
      space->hooks.Remove("LogicUpdate", self);


  }

  /*****************************************************************************/
  /*!
      \brief
        AniSprite update checks for change in frame
  */
  /*****************************************************************************/
  void AniSprite::CheckNextFrame(float dt)
  {
    // If we are paused or stopped, don't update frames
    if (m_paused || Stopped())
      return;

    // update frame dt
    m_time += dt;

    // if we have exceeded our framerate
    if (m_time > 1 / m_frameRate)
    {
      // reset time to count up to next frame
      m_time -= 1 / m_frameRate;

      // increment currframe
      m_currFrame += 1;

      // If we have reached our endframe
      if (m_currFrame > m_endFrame)
      {
        // loop back to beginning of animation
        m_currFrame = m_startFrame;
        m_framePos = m_startFramePos;

        // If we are finitely looping decrement loops
        if (m_loop > 1)
          --m_loop;

        // If loop is -1 we loop infinitley, else pause and stop
        else if (m_loop != -1)
        {
          Pause(true);
          space->GetGameObject(owner)->hooks.Call("AnimEnd");
        }

        // our job is done
        return;
      }

      // Else we continue to next frame
      m_framePos.X += 1;

      // If we have exceeded number of columns drop down a row
      if (m_framePos.X > m_frames.X)
      {
        m_framePos.X = 0;
        m_framePos.Y += 1;
      }
    } 
  }

  /*****************************************************************************/
  /*!
      \brief
        AniSprite Draw Call
  */
  /*****************************************************************************/
  void AniSprite::Draw()
  {

    if (m_hidden)
      return;

    Transform* trans = space->GetHandles().GetAs<Transform>(transform);

    UpdateUV();
    GRAPHICS->SetPosition(trans->GetTranslation().X, trans->GetTranslation().Y, trans->GetTranslation().Z);
    GRAPHICS->SetRotation(trans->GetRotation());
    GRAPHICS->SetSize(trans->GetScale().X * Size.X, trans->GetScale().Y * Size.Y);
    GRAPHICS->SetColor(Color);
    GRAPHICS->SetUV(uvBegin, uvEnd);
    GRAPHICS->SetCamState(0);
    GRAPHICS->SetSpriteFlip(m_flipX, m_flipY);
    GRAPHICS->DrawBatched(m_texture);

    //CheckNextFrame();
  }

  /*****************************************************************************/
  /*!
      \brief
        AniSprite Deconstructor
  */
  /*****************************************************************************/
  AniSprite::~AniSprite()
  {
  }

  void AniSprite::Test(void* value)
  {
    *(int*)((char*)this + AntTweak::Tweaker::currentGeneric->realOffset) = *(int*)value;
  }

}