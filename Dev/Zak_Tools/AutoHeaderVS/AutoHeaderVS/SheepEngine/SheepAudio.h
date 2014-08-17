/*****************************************************************
Filename: SheepAudio.h
Project: 
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "System.h"

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

	private:
	};

	extern SheepAudio* AUDIO;
}