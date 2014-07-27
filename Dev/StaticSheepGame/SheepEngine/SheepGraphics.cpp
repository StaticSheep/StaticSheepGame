/*****************************************************************
Filename: GraphicsInterface.cpp
Project: 
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "SheepGraphics.h"
#include "graphics\sheep_graphics.h"
#include <Windows.h>

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
    Sheep_GFX::GFX_Release_D3D();
	}

	void SheepGraphics::Initialize()
	{
		
		// Create DirectX object
		// Initialize graphics system

    _HWnd = ENGINE->Window.GetHandle();
    _ScreenWidth = ENGINE->Window.width;
    _ScreenHeight = ENGINE->Window.height;

    Sheep_GFX::GFX_Init_D3D(_HWnd, _ScreenWidth, _ScreenHeight);
	}

	void SheepGraphics::Update(float dt)
	{
		// Draw stuff

		Sheep_GFX::GFX_Draw();
	}

	void SheepGraphics::Draw()
	{

	}
}