/*****************************************************************
Filename: SheepAudio.cpp
Project: 
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "SheepAudio.h"

namespace Framework
{
	// Global pointer
	SheepAudio* AUDIO = NULL;

	SheepAudio::SheepAudio()
	{
		// This should load any required materials, set constants
		// Pre-initialization logic

		AUDIO = this;
	}

	SheepAudio::~SheepAudio()
	{
		// Free anything that was allocated
	}

	void SheepAudio::Initialize()
	{

	}

	void SheepAudio::Update(float dt)
	{

	}

}