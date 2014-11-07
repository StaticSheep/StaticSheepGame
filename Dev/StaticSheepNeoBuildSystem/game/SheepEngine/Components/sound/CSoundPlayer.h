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
    
    
  public:
    SoundPlayer();
    ~SoundPlayer();
    
    void Play(const std::string &name, SoundInstance* instance = &SoundInstance());
    
    void Stop(std::string name, FadeOut mode);
    void Pause(std::string name, bool flag);
    
    void SetPitch(float pitch);
    
   private:
    
    SheepAudio* _soundSystem;
    
    // keeps track of the currently playing instances
    std::unordered_map<std::string, SoundInstance> instanceList;
    
    float _pitch;
    float _volume;
    PlayMode _mode;
    
  };
}

