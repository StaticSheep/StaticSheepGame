/******************************************************************************
Filename: SoundEmitter.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "pch/precompiled.h"
#include "CSoundEmitter.h"

namespace Framework
{
/*****************************************************************************/
/*!
  \brief
    Plays an event! Creates a temporary instance, and immediately releases
    it. Good for short sfx.

  \param name
    Name of the event to play
*/
/*****************************************************************************/
  void SoundEmitter::Play(std::string name)
  {
    SOUND::EventInstance* instance = _soundSystem->Play(name, PLAY_ONCE, _volume, _pitch);
  }

  void SoundEmitter::Initialize()
  {

  }
  
/*****************************************************************************/
/*!
  \brief
    Plays an event! Creates a temporary instance, and immediately releases
    it. Good for short sfx. Has volume control

  \param name
    Name of the event to play

  \param volume
    What to set the volume to. Normalized values. 0 - 1
*/
/*****************************************************************************/
  void SoundEmitter::PlayEx(std::string name, float volume)
  {
    SOUND::EventInstance* instance = _soundSystem->Play(name, PLAY_ONCE, volume, _pitch);
  }


/*!
\brief  Sets the pitch of the emitter. Doesn't affect currently playing instances.
\param  pitch What to set the pitch to
*/
  void SoundEmitter::SetPitch(float pitch)
  {
    _pitch = pitch;
  }

/*****************************************************************************/
  
}// end namespace
