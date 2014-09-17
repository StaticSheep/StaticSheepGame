/*****************************************************************
Filename: SheepGraphics.cpp
Project: 
Author(s): Scott Nelson(Primary)
           Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include <Windows.h>
#include "SheepGraphics.h"

#include "graphics\api.h"
#include "graphics\Handle.h"
#include "graphics\Context.h"


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
    //DirectSheep::Release();
    delete m_renderContext;
	}

	void SheepGraphics::Initialize()
	{
		
		// Create DirectX object
		// Initialize graphics system

    _HWnd = ENGINE->Window.GetHandle();
    _ScreenWidth = ENGINE->Window.width;
    _ScreenHeight = ENGINE->Window.height;

    m_renderContext = DirectSheep::RenderContext::Allocate();

    m_renderContext->Initialize(_HWnd,_ScreenWidth,_ScreenHeight);

	}

	void SheepGraphics::Update(float dt)
	{
    Draw();
	}

	void SheepGraphics::Draw()
	{
    m_renderContext->SetClearColor(1, 0, 1, 1);
    m_renderContext->ClearBackBuffer();
    m_renderContext->DrawSpriteText("Damnit!", 64);
    m_renderContext->Present();
    //// Draw Hooks
    //GameSpace* space;

    //// Regular Draw
    //for (auto it = ENGINE->Spaces().begin(); it != ENGINE->Spaces().end(); ++it)
    //{
    //  space = *it;
    //  if (!space->Hidden())
    //    space->hooks.Call("Draw");
    //}
    //Lua::CallFunc(ENGINE->Lua(), "hook.Call", "Draw");

    //// Post Draw
    //for (auto it = ENGINE->Spaces().begin(); it != ENGINE->Spaces().end(); ++it)
    //{
    //  space = *it;
    //  if (!space->Hidden())
    //    space->hooks.Call("PostDraw");
    //}
    //Lua::CallFunc(ENGINE->Lua(), "hook.Call", "PostDraw");
    
	}

  void SheepGraphics::SetPosition(float x, float y)
  {
  }

  void SheepGraphics::SetRotation(float theta)
  {
  }

  void SheepGraphics::SetSize(float x, float y)
  {
  }

  void SheepGraphics::SetTexture(int ID)
  {
  }

  void SheepGraphics::SetColor(Vec4 Color)
  {
  }

  void SheepGraphics::DrawSprite(void)
  {
  }

  void SheepGraphics::FinishFrame()
  {
  }

  int SheepGraphics::GetTextureID(std::string& texture)
  {
    return 0;
  }

  void SheepGraphics::SetUseCamera(bool useCam)
  {
  }

  void SheepGraphics::DrawSpriteText(const char * text, float size, const char * font)
  {
  }
}