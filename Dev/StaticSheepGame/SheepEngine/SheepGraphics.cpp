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
        space->hooks.Call("Draw");
    }
    Lua::CallFunc(ENGINE->Lua(), "hook.Call", "Draw");

    // Post Draw
    for (auto it = ENGINE->Spaces().begin(); it != ENGINE->Spaces().end(); ++it)
    {
      space = *it;
      if (!space->Hidden())
        space->hooks.Call("PostDraw");
    }
    Lua::CallFunc(ENGINE->Lua(), "hook.Call", "PostDraw");
	}

  void SheepGraphics::SetupMatrices()
  {
    DirectSheep::SetupMatrices();
  }

  void SheepGraphics::SetPosition(float x, float y)
  {
    DirectSheep::GFX_SetPosition(x, y);
  }
  void SheepGraphics::SetRotation(float theta)
  {
    DirectSheep::GFX_SetRotation(theta);
  }
  void SheepGraphics::SetSize(float x, float y)
  {
    DirectSheep::GFX_SetSize(x, y);
  }
  void SheepGraphics::SetTexture(std::string filepath)
  {
    DirectSheep::GFX_SetTexture(filepath);
  }
  void SheepGraphics::DrawSprite(void)
  {
    DirectSheep::GFX_Draw();
  }
}