/*****************************************************************
Filename: GraphicsInterface.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "SheepGraphics.h"

namespace Framework
{
	// Global pointer
	SheepGraphics* GRAPHICS = NULL;

	SheepGraphics::SheepGraphics()
	{
		// This should load any required materials, set constants
		// Pre-initialization logic

		GRAPHICS = this;
	}

	SheepGraphics::~SheepGraphics()
	{
		// Free anything that was allocated
	}

	void SheepGraphics::Initialize()
	{
		// Open window
		// Create DirectX object
		// Initialize graphics system
	}

	void SheepGraphics::Update(float dt)
	{
		// Draw stuff

		Draw();
	}

	void SheepGraphics::Draw()
	{

	}
}