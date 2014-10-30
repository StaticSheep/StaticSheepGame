/******************************************************************************
Filename: SoundPlayer.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#pragma once
#include "systems/audio/SheepAudio.h"
#include <string>

#define MAX_SOUNDS 4

namespace Framework
{
  class SoundPlayer : public GameComponent
  {
    struct InstanceData
    {
      SOUND::EventInstance* instance;
      std::string name;
    };
    
    
  public:
    SoundPlayer();
    ~SoundPlayer() {};
    
    void Play(const std::string &name, PlayMode mode);
    void PlayEx(const std::string &name, PlayMode mode, float volume);
    
    void Stop(std::string name, FadeOut mode);
    void Pause(std::string name, bool flag);
    
    void SetPitch(float pitch);
    void SetVolume(float volume);
    
   private:
    
    SheepAudio* _soundSystem;
    
    // keeps track of the currently playing instances
    InstanceData _instanceList[MAX_SOUNDS];
    
    float _pitch;
    float _volume;
    PlayMode _mode;
    
  };
}

