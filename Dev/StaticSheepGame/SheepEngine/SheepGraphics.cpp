/*****************************************************************
Filename: GraphicsInterface.cpp
Project: 
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "SheepGraphics.h"
#include "graphics\sheep_graphics.h"

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
    GFX_Release_D3D();
	}

  void SheepGraphics::SetWindwProperties(HWND hWnd, int screenWidth, int screenHeight)
  {
    _HWnd = hWnd;
    _ScreenWidth = screenWidth;
    _ScreenHeight = screenHeight;
  }

	void SheepGraphics::Initialize()
	{
		
		// Create DirectX object
		// Initialize graphics system
    GFX_Init_D3D(_HWnd, _ScreenWidth, _ScreenHeight);
	}

	void SheepGraphics::Update(float dt)
	{
		// Draw stuff

		GFX_Draw();
	}

	void SheepGraphics::Draw()
	{

	}
}