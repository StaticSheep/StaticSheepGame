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

  void SheepGraphics::RegisterComponents()
  {
    REGISTER_COMPONENT(Sprite);
  }

	SheepGraphics::~SheepGraphics()
	{
		// Free anything that was allocated
    DirectSheep::ReleaseD3D();
	}

	void SheepGraphics::Initialize()
	{
		
		// Create DirectX object
		// Initialize graphics system

    _HWnd = ENGINE->Window.GetHandle();
    _ScreenWidth = ENGINE->Window.width;
    _ScreenHeight = ENGINE->Window.height;

    DirectSheep::InitD3D(_HWnd, _ScreenWidth, _ScreenHeight);
	}

	void SheepGraphics::Update(float dt)
	{
		// Draw stuff

		DirectSheep::StartFrame(dt);

    Draw();

    FinishFrame();
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
    DirectSheep::SetPosition(x, y);
  }
  void SheepGraphics::SetRotation(float theta)
  {
    DirectSheep::SetRotation(theta);
  }
  void SheepGraphics::SetSize(float x, float y)
  {
    DirectSheep::SetSize(x, y);
  }
  void SheepGraphics::SetTexture(int ID)
  {
    DirectSheep::SetTexture(ID);
  }
  void SheepGraphics::DrawSprite(void)
  {
    DirectSheep::Draw();
  }
  void SheepGraphics::FinishFrame()
  {
    DirectSheep::FinishFrame();
  }

  int SheepGraphics::GetTextureID(std::string& texture)
  {
    return DirectSheep::GetTextureID(texture);
  }
}