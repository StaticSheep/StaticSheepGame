/******************************************************************************
Filename: SoundEmitter.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#pragma once
#include "systems/audio/SheepAudio.h"
#include <string>

namespace Framework
{
  
  // plays only one type of sound... one off sounds
  class SoundEmitter : public GameComponent
  {
  public:
    SoundEmitter() { _soundSystem = AUDIO; _pitch = 1.0f; _volume = 1.0f;};
    ~SoundEmitter() {};

    void Initialize()
    {
      Play("space_brawl");
    };
    
    void Play(std::string name);
    void PlayEx(std::string name, float volume);
    
    void SetPitch(float pitch);
    
  private:
    
    SheepAudio* _soundSystem;
    
    float _pitch;
    float _volume;
    
  };
}

