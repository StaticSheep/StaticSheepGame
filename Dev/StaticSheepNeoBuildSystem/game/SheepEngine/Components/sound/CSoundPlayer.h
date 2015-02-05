/*****************************************************************
Filename: SoundPlayer.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once
#include "systems/audio/SheepAudio.h"
#include <string>

namespace Framework
{

  class SoundPlayer : public GameComponent
  {
    
    
  public:
    SoundPlayer();
    ~SoundPlayer();
    
    // Plays the requested sound with the parameters in instance. instance is defaulted if not provided
    void Play(const std::string &name, SoundInstance* instance = &SoundInstance());
    void LuaPlay(std::string name);
    
    // Stops the requested sound. FadeOut does nothing currently.
    void Stop(std::string name, FadeOut mode);
    void LuaStop(std::string name);

    // Pauses the requested sound. Pause = true, Unpause = false
    void Pause(std::string name, bool flag);
    
    void SetPitch(float pitch);
    void SetVolume(float volume);
    
   private:
    
    SheepAudio* _soundSystem;
    
    // keeps track of the currently playing instances
    std::unordered_map<std::string, SoundInstance> instanceList;
    
    float _pitch;
    float _volume;
    PlayMode _mode;
  };
}

