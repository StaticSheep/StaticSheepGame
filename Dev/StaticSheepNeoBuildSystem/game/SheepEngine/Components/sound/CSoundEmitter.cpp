/*****************************************************************
Filename: SoundEmitter.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "CSoundEmitter.h"

namespace Framework
{
  // Plays the requested sound. Instance is defaulted if not given.
  void SoundEmitter::Play(std::string name, SoundInstance* instance)
  {
    _soundSystem->Play(name, instance);
  }

  void SoundEmitter::Initialize()
  {

  }
  
  void SoundEmitter::SetPitch(float pitch)
  {
    _pitch = pitch;
  }
  
}
