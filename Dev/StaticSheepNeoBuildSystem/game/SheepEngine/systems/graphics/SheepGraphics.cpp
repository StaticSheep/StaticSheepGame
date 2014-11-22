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
#include "SheepGraphics\Context.h"

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

    m_renderContext = (DirectSheep::RenderContext*)rc;
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
      m_renderContext = DirectSheep::RenderContext::Allocate();
      m_renderContext->Initialize(ENGINE->Window->GetHandle(), (float)_ScreenWidth, (float)_ScreenHeight);
    }
    
    Message m(Message::GFXDeviceInit);
    ENGINE->SystemMessage(m);

    m_renderContext->CreateConstantBuffer(spriteContext, 160);

    m_renderContext->CreateVertexBuffer(spriteQuad, 120);

    ErrorIf(!LoadAssets(std::string("content")), "AssetLoad", "SheepGraphics.cpp");
	}

	void SheepGraphics::Update(float dt)
	{
#if SHEEP_DEBUG
    m_debugData.numDrawCalls = 0;
    m_debugData.numBatchedCalls = 0;
    m_debugData.numTextDraws = 0;
#endif

    StartFrame();
    Draw();
    FinishFrame();
	}
  
  void SheepGraphics::SetDefaultCam(void)
  {
    m_renderContext->SetCamDefault();
  }

  void SheepGraphics::ReceiveMessage(Message& msg)
  {
    if (msg.MessageId == Message::WindowResize)
    {
      if (m_renderContext)
      {
        ResizeMessage* rMsg = (ResizeMessage*)&msg;
        _ScreenWidth = (int)rMsg->width;
        _ScreenHeight = (int)rMsg->height;

        m_renderContext->Resize((float)_ScreenWidth, (float)_ScreenHeight);
      }

      if (msg.MessageId == Message::WindowMinimize)
      {

      }

      if (msg.MessageId == Message::WindowRestore)
      {

      }
      
    }
  }

	void SheepGraphics::Draw()
	{
    // Draw Hooks
    GameSpace* space;
    

    //Vec2 size = m_renderContext->MeasureString("TEST THIS\nSHIT BOOM", 32, "Arial");

    //Draw::SetCamState(0);
    //Draw::SetColor(1, 0, 0, 1);
    //Draw::DrawRect(0, -size.y / 2, size.x, size.y);
    //Draw::SetColor(1, 1, 1, 1);
    //Draw::SetPosition(0 - size.x / 2, 0);
    //Draw::SetRotation(0);
    //Draw::DrawString("TEST THIS\nSHIT BOOM", 32, "Arial");
    //Draw::SetCamState(0);

    m_renderContext->StartBatch();
    // Regular Draw
    for (auto it = ENGINE->Spaces().begin(); it != ENGINE->Spaces().end(); ++it)
    {
      space = *it;

      if (!space->Ready())
        continue;

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

      if (!space->Ready())
        continue;

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
    m_renderContext->frameStart();
  }

  void SheepGraphics::FinishFrame()
  {
    m_renderContext->frameEnd();
  }

  void SheepGraphics::SetPosition(float x, float y, float Z)
  {
    m_renderContext->SetPosition(x,y, Z);
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

  void SheepGraphics::SetWireframe(bool iswired)
  {
    m_renderContext->setWireFrame(iswired);
  }

  void SheepGraphics::FlipSprite(bool x, bool y)
  {
    m_renderContext->SetSpriteFlip(x, y);
  }

  DirectSheep::Handle SheepGraphics::SetTexture(const std::string& Texture)
  {
    for(auto it : m_textureMap)
    {
      if(it.first == Texture)
        return it.second;
    }

    std::string realTexture = Texture;

    DirectSheep::Handle temp;

    m_renderContext->CreateTexture(temp, Texture);
    
#if SHEEP_DEBUG
    ++(m_debugData.numTextures);
#endif

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

#if SHEEP_DEBUG
    ++(m_debugData.numBatchedCalls);
#endif
  }

  void SheepGraphics::RawDraw(void)
  {
    m_renderContext->BindVertexBuffer(spriteQuad,20,0);

    m_renderContext->Draw(6,0);


#if SHEEP_DEBUG
    ++(m_debugData.numDrawCalls);
#endif
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

  void SheepGraphics::SetCamState(int camState)
  {
    m_renderContext->SetCamState(camState);
  }

  void SheepGraphics::DrawSpriteText(const char * text, float size, const char * font)
  {
    m_renderContext->DrawSpriteText(text, size, font);

#if SHEEP_DEBUG
    ++(m_debugData.numTextDraws);
#endif
  }
  void* SheepGraphics::GetDevice()
  {
    return m_renderContext->ExternalGetDevice();
  }

  Vec2 SheepGraphics::GetTextureDim(DirectSheep::Handle texture)
  {
    DirectSheep::Dimension texSize = m_renderContext->GetTextureSize(texture);
    return Vec2((float)texSize.width, (float)texSize.height);
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
            SetTexture(it->path().filename().generic_string());
        }
      }
      return true;
    }
    return false;
  }
  

  Vec2 SheepGraphics::MeasureString(const char* text, float size,
    const char* font)
  {
    return m_renderContext->MeasureString(text, size, font);
  }

}