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

  class SheepAudio;

  extern SheepAudio* AUDIO;

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
  
    // Plays the requested sound
    bool Play(const std::string& name, SoundInstance* instance);
  
    bool Stop(SoundInstance* instance);
    void StopAll();

    // Pauses the instance. Pass true for pause, false for unpause
    void Pause(SoundInstance* instance, bool status);

    // Pause everything. Pass true for pause, false for unpause
    void PauseAll(bool paused);

    static void SetMasterVolume(float volume);
    void SetMasterPitch(float pitch);
    static void SetMusicVolume(float volume);
    static void SetSFXVolume(float volume);

    static float GetMasterVolume();
    static float GetMusicVolume();
    static float GetSFXVolume();

    static SheepAudio* GetAudioSystem()
    {
      return AUDIO;
    }

    
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

    FMOD::ChannelGroup* musicChannel;
    FMOD::ChannelGroup* sfxChannel;

    float masterVolume;

    float musicVolume;
    float sfxVolume;

    float masterPitch;
    
  };

	
}