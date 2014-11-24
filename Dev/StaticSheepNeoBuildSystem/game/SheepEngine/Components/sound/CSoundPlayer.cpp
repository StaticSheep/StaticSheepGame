/*****************************************************************
Filename: SoundPlayer.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "CSoundPlayer.h"


namespace Framework
{

  SoundPlayer::SoundPlayer()
  {
    // connect to the FMOD system
    _soundSystem = AUDIO;
    _volume = 1.0f;
    _pitch = 1.0f;
  }

  SoundPlayer::~SoundPlayer()
  {
    // iterate through all of the instances
    for(auto it = instanceList.begin(); it != instanceList.end(); ++it)
    {
      // and stop them
      if(it->second.active)
      {
        _soundSystem->Stop(&it->second);
        it->second.active = false;
      }
    }
  }
  
  // Plays the requested sound with the parameters in instance. instance is defaulted if not provided
  void SoundPlayer::Play(const std::string &name, SoundInstance* instance)
  {
    // if the instance exists, and is active
    if(instanceList.find(name) != instanceList.end() && instanceList[name].active)
      return; // don't try and play it again

    // otherwise, make a new instance
    instanceList[name] = *instance;
    instanceList[name].active = true;

    // and play it
    _soundSystem->Play(name, &instanceList[name]);
  }
  
  // Stops a specific instance. FadeOut currently does nothing.
  void SoundPlayer::Stop(std::string name, FadeOut mode)
  {
    // if the instance exists
    if(instanceList.find(name) != instanceList.end())
    {
      // tell it to stop
      _soundSystem->Stop(&instanceList[name]);
      // and set the active flag to false
      instanceList[name].active = false;
    }
  }

  // Pauses a specific instance. Pause = true, Unpause = false
  void SoundPlayer::Pause(std::string name, bool flag)
  {
    // make sure the instance exists first
    if(instanceList.find(name) != instanceList.end())
      _soundSystem->Pause(&instanceList[name], flag);
  }
  
  void SoundPlayer::SetPitch(float pitch)
  {
    _pitch = pitch;
  }

  void SoundPlayer::SetVolume(float volume)
  {
    _volume = volume;
  }

}// end namespace
