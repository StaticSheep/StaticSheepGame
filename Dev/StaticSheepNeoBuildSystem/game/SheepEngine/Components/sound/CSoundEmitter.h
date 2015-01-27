/*****************************************************************
Filename: SoundEmitter.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once
#include "systems/audio/SheepAudio.h"
#include <string>

namespace Framework
{
  
  // Used for playing short SFX. Does not keep track of sounds it plays
  class SoundEmitter : public GameComponent
  {
  public:
    SoundEmitter() { _soundSystem = AUDIO; _pitch = 1.0f; _volume = 1.0f;};
    ~SoundEmitter() {};

    void Initialize();
    
    // Plays the requested sound with the parameters in instance. instance is defaulted if not provided.
    void Play(std::string name, SoundInstance* instance = &SoundInstance());
    
    void SetPitch(float pitch);
    
  private:
    
    SheepAudio* _soundSystem;
    
    float _pitch;
    float _volume;

  };
}

