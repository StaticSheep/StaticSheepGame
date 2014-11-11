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
  void SoundEmitter::Play(std::string name, SoundInstance* instance)
  {
    _soundSystem->Play(name, instance);
  }

  void SoundEmitter::Initialize()
  {

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
