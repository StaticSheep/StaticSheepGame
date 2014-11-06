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
  
    virtual std::string GetName() {return "SheepAudio";};
  
    void Update(float dt);
    void Initialize();
  
    void RegisterComponents();
    const void* GetDebugData();
  
    bool Play(const std::string& name, SoundInstance* instance);
  
    bool Stop(SoundInstance* instance);
    void StopAll();
    void Pause(SoundInstance* instance){};
    void PauseAll(){};

    bool GetLoadState() const;

    DebugAudio* debug;
  
  private:

    const std::string GUID;
    std::unordered_map<std::string, Sound *> soundMap;
    std::vector<SOUND::Bank *> banks;
    SOUND::System* system;
    FMOD::System* lowLevelSystem;

    
  };



  /*
	class SheepAudio : public ISystem
	{
	public:
		// Sheep graphics interface constructor
		SheepAudio();
		// Sheep graphics interface deconstructor
		~SheepAudio();

		virtual std::string GetName() {return "SheepAudio";};

		// Initialization call
		void Initialize(void);

    void RegisterComponents(void);

		// Update per frame
		void Update(float dt);

    // Play the things! Stop the things!
    SOUND::EventInstance* Play(const std::string &event_name, PlayMode mode, float volume, float pitch);

    const void* GetDebugData();

    void Stop(const std::string &event_name, FadeOut mode);
    void StopAll(FadeOut mode);
    bool GetLoadState() const;

	private:

    // name of GUID file for FMOD
    const std::string _GUID;
    // pointer to the FMOD system
    SOUND::System* _system;

    FMOD::System* _lowLevelSystem;
    // container for all sound events
    std::unordered_map<std::string, SoundEvent> _events;
    // vector for all sound banks
    std::vector<SOUND::Bank *> _banks;
    //vector for all of the instances...
    std::vector<SOUND::EventInstance* > _instances;


    float _MasterVolume;
    float _SFXVolume;
    float _MusicVolume;

    DebugAudio* debug;

    void** _ninjaPointer; // just in case

	};*/

	extern SheepAudio* AUDIO;
}