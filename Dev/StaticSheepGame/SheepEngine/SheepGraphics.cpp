/*****************************************************************
Filename: GraphicsInterface.cpp
Project: 
Author(s): Zachary Nawar

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
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

    ErrorIf(GRAPHICS != NULL, "SheepGraphics", "Graphics already initialized");
		GRAPHICS = this;
	}

	SheepGraphics::~SheepGraphics()
	{
		// Free anything that was allocated
    DirectSheep::GFX_Release_D3D();
	}

	void SheepGraphics::Initialize()
	{
		
		// Create DirectX object
		// Initialize graphics system

    _HWnd = ENGINE->Window.GetHandle();
    _ScreenWidth = ENGINE->Window.width;
    _ScreenHeight = ENGINE->Window.height;

    DirectSheep::GFX_Init_D3D(_HWnd, _ScreenWidth, _ScreenHeight);
	}

	void SheepGraphics::Update(float dt)
	{
		// Draw stuff

		DirectSheep::GFX_Update(dt);

    Draw();

    Draw();
	}

	void SheepGraphics::Draw()
	{
    // Draw Hooks
    GameSpace* space;
    SetupMatrices();

    // Regular Draw
    for (auto it = ENGINE->Spaces().begin(); it != ENGINE->Spaces().end(); ++it)
    {
      space = *it;
      if (!space->Hidden())
        space->hooks.Call("Draw", dt);
    }
    Lua::CallFunc(ENGINE->Lua(), "hook.Call", "Draw", dt);

    // Post Draw
    for (auto it = ENGINE->Spaces().begin(); it != ENGINE->Spaces().end(); ++it)
    {
      space = *it;
      if (!space->Hidden())
        space->hooks.Call("PostDraw", dt);
    }
    Lua::CallFunc(ENGINE->Lua(), "hook.Call", "PostDraw", dt);
	}

  void SheepGraphics::SetupMatrices()
  {
    DirectSheep::SetupMatrices();
  }
}