/*****************************************************************
Filename: SheepGraphics.cpp
Project: 
Author(s): Scott Nelson(Primary)
           Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include <Windows.h>
#include "SheepGraphics.h"

#include "SheepGraphics\api.h"
#include "SheepGraphics\Handle.h"
#include "SheepGraphics\Interface.h"

#include "engine/window/Window32.h"

#include "components/sprites/CAniSprite.h"
#include <iostream>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;


namespace Framework
{
	// Global pointer
	SheepGraphics* GRAPHICS = NULL;

	SheepGraphics::SheepGraphics(void* rc)
	{
		// This should load any required materials, set constants
		// Pre-initialization logic

    ErrorIf(GRAPHICS != NULL, "SheepGraphics", "Graphics already initialized");
		GRAPHICS = this;

    m_renderContext = (DirectSheep::Interface*)rc;

    CurrentCamera = NULL;
	}

  void SheepGraphics::RegisterComponents()
  {
    REGISTER_COMPONENT(Sprite);
    REGISTER_COMPONENT(Camera);
    REGISTER_COMPONENT(AniSprite);
  }

	SheepGraphics::~SheepGraphics()
	{
		// Free anything that was allocated
    //DirectSheep::Release();
    m_renderContext->Uninitialize(m_renderContext);
    m_renderContext = NULL;
	}

	void SheepGraphics::Initialize()
	{
		
		// Create DirectX object
		// Initialize graphics system

#if USE_EDITOR
#else
    _ScreenWidth = ENGINE->Window->GetWidth();
    _ScreenHeight = ENGINE->Window->GetHeight();
#endif

    if (m_renderContext == nullptr)
    {
      m_renderContext = DirectSheep::Interface::Allocate();
      m_renderContext->Initialize(ENGINE->Window->GetHandle(), (float)_ScreenWidth, (float)_ScreenHeight);
    }
    
    Message m(Message::GFXDeviceInit);
    ENGINE->SystemMessage(m);

    ErrorIf(!LoadAssets(std::string("content")), "AssetLoad", "SheepGraphics.cpp");

    
	}

	void SheepGraphics::Update(float dt)
	{
    StartFrame();
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
    m_renderContext->EndBatch();
    m_renderContext->StartBatch();
    // Post Draw
    for (auto it = ENGINE->Spaces().begin(); it != ENGINE->Spaces().end(); ++it)
    {
      space = *it;
      if (!space->Hidden())
        space->hooks.Call("PostDraw");
    }
    Lua::CallFunc(ENGINE->Lua(), "hook.Call", "PostDraw");
    m_renderContext->EndBatch();
    Message m(Message::PostDraw);
    ENGINE->SystemMessage(m);
    
	}

  void SheepGraphics::StartFrame()
  {
    UpdateCamera();
    m_renderContext->frameStart();
    m_renderContext->StartBatch();
  }

  void SheepGraphics::FinishFrame()
  {
    m_renderContext->frameEnd();
  }

  void SheepGraphics::SetPosition(float x, float y)
  {
    m_renderContext->SetPosition(x,y);
  }

  void SheepGraphics::SetRotation(float theta)
  {
    m_renderContext->SetRotation(theta);
  }

  void SheepGraphics::SetSize(float x, float y)
  {
    m_renderContext->SetDimensions(x,y);
  }

  void SheepGraphics::SetUV(Vec2 uvBegin, Vec2 uvEnd)
  {
    m_renderContext->SetUV(uvBegin.X, uvBegin.Y, uvEnd.X, uvEnd.Y);
  }

  DirectSheep::Handle SheepGraphics::SetTexture(const std::string& Texture)
  {
    for(auto it : m_textureMap)
    {
      if(it.first == Texture)
        return it.second;
    }

    DirectSheep::Handle temp;

    m_renderContext->CreateTexture(temp, Texture);

    m_textureMap[Texture] = temp;

    return m_textureMap[Texture];
  }

  void SheepGraphics::BindTexture(int ID)
  {
    m_renderContext->BindTexture(0,DirectSheep::Handle(DirectSheep::TEXTURE, ID));
  }

  void SheepGraphics::SetColor(Vec4 Color)
  {
    m_renderContext->SetBlendCol(Color.R, Color.G, Color.B, Color.A);
  }

  void SheepGraphics::DrawSprite(Sprite *sprite)
  {
    m_renderContext->DrawBatched(sprite->GetTexture());
  }

  void SheepGraphics::RawDraw(void)
  {
    m_renderContext->Draw(6,0);
  }
  int SheepGraphics::GetTextureID(const std::string& texture)
  {
    for(auto it : m_textureMap)
    {
      if(it.first == texture)
         return m_textureMap[texture].GetIndex();
    }
     return (SetTexture(texture)).GetIndex();
  }

  void SheepGraphics::SetUseCamera(bool useCam)
  {
    m_renderContext->SetUseCam(useCam);
  }

  void SheepGraphics::DrawSpriteText(const char * text, float size, const char * font)
  {
    UpdateCamera();
    m_renderContext->DrawSpriteText(text, size, font);
  }

  void SheepGraphics::UpdateCamera(void)
  {
      m_renderContext->UpdateCamera(0, 0, 75);
  }
  void* SheepGraphics::GetDevice()
  {
    return m_renderContext->ExternalGetDevice();
  }

  Vec2 SheepGraphics::GetTextureDim(DirectSheep::Handle texture)
  {
    float* temp = m_renderContext->GetTextureSize(texture);
    return Vec2(temp[0], temp[1]);
  }

  bool SheepGraphics::LoadAssets(std::string& filepath)
  {
    path p(filepath);

    if (exists(p))
    {
      if (is_directory(p))
      {
        for (directory_iterator it(p), end; it != end; ++it)
        {
          std::string foo = it->path().extension().generic_string();
          if (it->path().extension().generic_string() == ".png" || it->path().extension().generic_string() == ".jpg")
            SetTexture(it->path().generic_string());
        }
      }
      return true;
    }
    return false;
  }
  
}