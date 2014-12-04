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

    void ReceiveMessage(Message& msg);
  
    bool Play(const std::string& name, SoundInstance* instance);
  
    bool Stop(SoundInstance* instance);
    void StopAll();
    void Pause(SoundInstance* instance, bool status);
    void PauseAll(bool paused);

    void SetMasterVolume(float volume);
    float GetMasterVolume();

    void SetMasterPitch(float pitch);
    float GetMasterPitch();

    bool GetLoadState() const;

    /* ----- Lua Binds ----- */
    static void LuaPauseAll(bool paused);


    DebugAudio* debug;
  
  private:

    const std::string GUID;
    std::unordered_map<std::string, Sound *> soundMap;
    std::vector<SOUND::Bank *> banks;
    SOUND::System* system;
    FMOD::System* lowLevelSystem;

    float masterVolume;
    float masterPitch;
    
  };

	extern SheepAudio* AUDIO;
}