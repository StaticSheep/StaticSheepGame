/*****************************************************************
Filename: SheepGraphics.cpp
Project: 
Author(s): Scott Nelson(Primary)
           Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "SheepGraphics.h"
#include "graphics\api.h"
#include "graphics\System.h"
#include "graphics\RenderMain.h"
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
    DirectSheep::Release();
	}

	void SheepGraphics::Initialize()
	{
		
		// Create DirectX object
		// Initialize graphics system

    _HWnd = ENGINE->Window.GetHandle();
    _ScreenWidth = ENGINE->Window.width;
    _ScreenHeight = ENGINE->Window.height;

    DirectSheep::Init(_HWnd, _ScreenWidth, _ScreenHeight);
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

  void SheepGraphics::SetColor(Vec4 Color)
  {
    DirectSheep::SetBlendColor(Color.R, Color.G, Color.B, Color.A);
  }

  void SheepGraphics::DrawSprite(void)
  {
    DirectSheep::DrawSprite();
  }

  void SheepGraphics::FinishFrame()
  {
    DirectSheep::FinishFrame();
  }

  int SheepGraphics::GetTextureID(std::string& texture)
  {
    return DirectSheep::GetTextureID(texture);
  }

  void SheepGraphics::SetUseCamera(bool useCam)
  {
    DirectSheep::SetUseCamera(useCam);
  }

  void SheepGraphics::DrawSpriteText(const char * text, float size, const char * font)
  {
    DirectSheep::DrawSpriteText(text, size, font);
  }
}