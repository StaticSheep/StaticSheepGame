/*****************************************************************
Filename: SheepAudio.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "systems/System.h"
#include <unordered_map>
#include "SheepSoundEvent.h"

namespace Framework
{
  // debug data for audio
  struct DebugAudio
  {
    FMOD_STUDIO_CPU_USAGE cpuLoad;
    FMOD_STUDIO_BUFFER_USAGE bufferInfo;
    int RAM;
    int channels;
    void* data; 
    unsigned int block;
  };

  class SheepAudio : public ISystem
  {
  public:
    SheepAudio();
    ~SheepAudio();

    void Shutdown();
  
    virtual std::string GetName() {return "SheepAudio";};
    
    void Update(float dt);
    void Initialize();
  
    void RegisterComponents();
    const void* GetDebugData();
  
    // Looks for if the sound name exists and plays it based on the instance parameters
    bool Play(const std::string& name, SoundInstance* instance);
  
    // Stops playing a specific sound instance
    bool Stop(SoundInstance* instance);

    // Tells Master Channel to stop playing everything
    void StopAll();

    // Pauses/Unpauses a specific instance. Pass true to pause, false to unpause.
    void Pause(SoundInstance* instance, bool status);

    // Pauses everything in the master channel group.
    void PauseAll(bool status);

    void SetMasterVolume(float volume);
    float GetMasterVolume();

    void SetMasterPitch(float pitch);
    float GetMasterPitch();

    // Gets the loading state of FMOD. Returns true if done loading, false if not done.
    bool GetLoadState() const;

    DebugAudio* debug;
  
  private:

    /*---------- FMOD System -----------*/

    SOUND::System* system;
    FMOD::System* lowLevelSystem;
    const std::string GUID;
    std::unordered_map<std::string, Sound *> soundMap;
    std::vector<SOUND::Bank *> banks;
    
    /*---------- Audio Parameters -----------*/

    float masterVolume;
    float masterPitch;
    
  };

	extern SheepAudio* AUDIO;
}