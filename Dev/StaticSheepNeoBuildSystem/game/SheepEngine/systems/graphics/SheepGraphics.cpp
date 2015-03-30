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

#include "systems/graphics/Debug.h"

#include "engine/window/Window32.h"

#include "components/sprites/CAniSprite.h"
#include "components/particles/CParticleSystem.h"
#include "components/particles/CParticleCircleEmitter.h"
#include "components/particles/CParticleBoxEmitter.h"
#include "components/lights/CSpriteLight.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include "../input/Input.h"
#include "components/sprites/CSpineSprite.h"

#include "components/basicps/CBasicPSystem.h"

using namespace boost::filesystem;


namespace Framework
{
  bool SheepGraphics::m_FullScreen = false;
  DirectSheep::RenderContext* SheepGraphics::m_renderContext = nullptr;
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
    REGISTER_COMPONENT(PointLight);
    REGISTER_COMPONENT(SpriteLight);
    REGISTER_COMPONENT(BasicParticleSystem);
    REGISTER_COMPONENT(ParticleSystem);
    REGISTER_COMPONENT(ParticleCircleEmitter);
    REGISTER_COMPONENT(ParticleBoxEmitter);
    REGISTER_COMPONENT(SpineSprite);
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
      m_renderContext->Initialize(ENGINE->Window->GetHandle(),
        (float)_ScreenWidth, (float)_ScreenHeight);
    }

    Message m(Message::GFXDeviceInit);
    ENGINE->SystemMessage(m);

    m_renderContext->CreateConstantBuffer(spriteContext, 160);

    m_renderContext->CreateVertexBuffer(spriteQuad, 120);

    m_renderContext->m_letterbox = ENABLE_LETTERBOXING;


    ErrorIf(!LoadAssets(std::string("content")), "AssetLoad", "SheepGraphics.cpp");

    ErrorIf(!LoadAtlas(std::string("content")), "AtlasLoad", "SheepGraphics.cpp");
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

#if SHEEP_DEBUG
    if (SHEEPINPUT->KeyIsPressed(VK_OEM_3) && SHEEPINPUT->KeyIsDown(VK_CONTROL))
    {
      m_renderContext->PurgeTextures();
    }
#endif
	}
  
  void SheepGraphics::ActivateDefaultCamera(void)
  {
    m_renderContext->ActivateDefaultCamera();
  }

  static void GetDesktopResolution(int& horizontal, int& vertical)
  {
    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    // The top left corner will have coordinates (0,0)
    // and the bottom right corner will have coordinates
    // (horizontal, vertical)
    horizontal = desktop.right;
    vertical = desktop.bottom;
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

        Lua::CallFunc(ENGINE->Lua(), "WindowResize",
          RC()->GetViewport().dim.width,
          RC()->GetViewport().dim.height);
      }
    }

      if (msg.MessageId == Message::WindowMinimize && m_renderContext)
      {
        m_renderContext->SetFullscreen(false);
      }

      if (msg.MessageId == Message::WindowRestore && m_renderContext)
      {
          SetFullScreen(m_FullScreen);
      }

      if (msg.MessageId == Message::EngineReady)
      {
        ShowWindow(ENGINE->Window->GetHandle(), SW_SHOWNORMAL);
        SetFullScreen(m_FullScreen);
      }
      
    }

	void SheepGraphics::Draw()
	{
    // Draw Hooks
    GameSpace* space;
    Draw::SetCamState(0);

    if (SHEEPINPUT->KeyIsPressed('F'))
    {
      m_FullScreen = !m_FullScreen;
      SetFullScreen(m_FullScreen);
    }
    m_renderContext->StartBatch(3, ENGINE->m_editorActive ? !ENGINE->m_editorLights : false);
    // Regular Draw
    for (auto it = ENGINE->Spaces().begin(); it != ENGINE->Spaces().end(); ++it)
    {
      space = *it;

      if (!space->Ready())
        continue;

      if (!space->Hidden())
      {
        RC()->SetLayer(0);
        space->hooks.Call("PreDraw");

        RC()->SetLayer(1);
        space->hooks.Call("Draw");

        RC()->SetLayer(2);
        space->hooks.Call("PostDraw");
      }
        
    }

    RC()->SetLayer(1);
    Lua::CallFunc(ENGINE->Lua(), "hook.Call", "Draw");

    RC()->SetLayer(2);
    Lua::CallFunc(ENGINE->Lua(), "hook.Call", "PostDraw");
    ENGINE->SystemMessage(Message(Message::PostDraw));

    m_renderContext->EndBatch();


    Draw::SetCamState(2);

    m_renderContext->StartBatch(2, true);

    RC()->SetLayer(0);
    ENGINE->SystemMessage(Message(Message::GUIDraw));

    RC()->SetLayer(1);
    ENGINE->SystemMessage(Message(Message::PostGUIDraw));

    m_renderContext->EndBatch();

    Draw::SetCamState(2);
	}

  void SheepGraphics::StartFrame()
  {
    m_renderContext->FrameStart();
  }

  void SheepGraphics::FinishFrame()
  {
    m_renderContext->FrameEnd();
  }

  void SheepGraphics::SetPosition(float x, float y, float Z)
  {
    m_renderContext->SetPosition(x,y, Z);
  }

  void SheepGraphics::SetObjectOrigin(float x, float y)
  {
    m_renderContext->SetObjectOrigin(x, y);
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
    m_renderContext->SetWireFrame(iswired);
  }

  void SheepGraphics::SetSpriteFlip(bool x, bool y)
  {
    m_renderContext->SetSpriteFlip(x, y);
  }

  DirectSheep::Handle SheepGraphics::LoadTexture(const std::string& textureName)
  {
    for(auto it : m_textureMap)
    {
      if (it.first == textureName)
        return it.second;
    }

    std::string realTexture = textureName;

    DirectSheep::Handle temp;

    m_renderContext->CreateTexture(temp, textureName);
    
#if SHEEP_DEBUG
    ++(m_debugData.numTextures);
#endif

    m_textureMap[textureName] = temp;

    return m_textureMap[textureName];
  }

  void SheepGraphics::BindTexture(int ID)
  {
    m_renderContext->BindTexture(0,DirectSheep::Handle(DirectSheep::TEXTURE, ID));
  }

  void SheepGraphics::SetColor(Vec4 Color)
  {
    m_renderContext->SetBlendCol(Color.R, Color.G, Color.B, Color.A);
  }


  void SheepGraphics::DrawBatched(DirectSheep::Handle texture)
  {
    m_renderContext->DrawBatched(texture);

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
    return (LoadTexture(texture)).GetIndex();
  }

  void SheepGraphics::SetCamState(int camState)
  {
    m_renderContext->SetCamState(camState);
  }

  void SheepGraphics::DrawSpriteText(const char * text, int fontIndex, Vec2D scale)
  {
    m_renderContext->DrawSpriteText(text, fontIndex, 0.02f * scale);

#if SHEEP_DEBUG
    ++(m_debugData.numTextDraws);
#endif
  }
  void* SheepGraphics::GetDevice()
  {
    return m_renderContext->ExternalGetDevice();
  }

  int SheepGraphics::GetFontIndex(const char * fontName)
  {
    if (m_fontMap.count(fontName))
      return m_fontMap[fontName];
    else
      return m_fontMap["Arial"];
  }

  Vec2 SheepGraphics::GetTextureDim(DirectSheep::Handle texture)
  {
    DirectSheep::Dimension texSize = m_renderContext->GetTextureSize(texture);
    return Vec2((float)texSize.width, (float)texSize.height);
  }

  bool SheepGraphics::LoadAtlas(const std::string& filepath)
  {
    path p(filepath);

    if (exists(p))
    {
      if (is_directory(p))
      {
        for (directory_iterator it(p), end; it != end; ++it)
        {
          std::string foo = it->path().extension().generic_string();
          if (it->path().extension().generic_string() == ".atlas")
          {
            DirectSheep::Handle temp;

            m_renderContext->CreateAtlas(temp, it->path().filename().generic_string());

            m_atlasMap[it->path().filename().leaf().generic_string()] = temp;

          }
        }
      }

      return true;
    }
    return false;
  }

  DirectSheep::Handle SheepGraphics::GetAtlasHandle(const std::string& atlas)
  {
    std::string realName = atlas + ".atlas";

    for(auto it : m_atlasMap)
    {
      if (it.first == realName)
        return it.second;
    }

    return DirectSheep::Handle(); 
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
            LoadTexture(it->path().filename().generic_string());
        }
      }
      else
        return false;

      p = filepath + "/Fonts";
      if (exists(p))
      {
        if (is_directory(p))
        {
          for (directory_iterator it(p), end; it != end; ++it)
          {
            std::string foo = it->path().extension().generic_string();
            if (it->path().extension().generic_string() == ".spritefont")
              // This line is gross
              m_fontMap[it->path().stem().generic_string().c_str()] = 
              m_renderContext->AddFont(it->path().stem().generic_string().c_str(), it->path().generic_string().c_str());
          }
        }
        return true;
      }
      else
        return false;
    }
    return false;
  }
  

  Vec2 SheepGraphics::MeasureString(const char* text, Vec2D scale,
    int fontIndex)
  {
    return m_renderContext->MeasureString(text, 0.02f * scale, fontIndex);
  }

  bool SheepGraphics::IsFullScreen()
  {
    return m_FullScreen;
  }

  void SheepGraphics::SetFullScreen(bool isFull)
  {
    m_FullScreen = isFull;

    m_renderContext->SetFullscreen(isFull);
  }

  DirectSheep::Camera* SheepGraphics::RetrieveCamera(DirectSheep::Handle camHandle)
  {
    return DirectSheep::RenderContext::RetrieveCamera(camHandle);
  }



  void SheepGraphics::SetActiveCamera(DirectSheep::Handle camHandle)
  {
    m_renderContext->SetActiveCamera(camHandle);
  }

}