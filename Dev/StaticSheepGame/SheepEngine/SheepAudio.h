/*****************************************************************
Filename: SheepAudio.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "System.h"
#include <Windows.h>
#include <unordered_map>
#include "SheepSoundEvent.h"

namespace Framework
{
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

		// Update per frame
		void Update(float dt);

    // Play the things! Stop the things!
    void Play(const std::string &event_name, PlayMode mode);
    void Stop(const std::string &event_name, FadeOut mode);
    void StopAll(FadeOut mode);
    bool GetLoadState() const;

	private:

    // name of GUID file for FMOD
    const std::string _GUID;
    // pointer to the FMOD system
    SOUND::System* _system;
    // container for all sound events
    std::unordered_map<std::string, SoundEvent> _events;
    // vector for all sound banks
    std::vector<SOUND::Bank *> _banks;

    void** _ninjaPointer; // just in case

	};

	extern SheepAudio* AUDIO;
}