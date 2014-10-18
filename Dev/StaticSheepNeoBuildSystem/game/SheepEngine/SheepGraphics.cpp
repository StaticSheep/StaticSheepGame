/*****************************************************************
Filename: SheepGraphics.cpp
Project: 
Author(s): Scott Nelson(Primary)
           Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "precompiled.h"
#include <Windows.h>
#include "SheepGraphics.h"

#include "SheepGraphics\api.h"
#include "SheepGraphics\Handle.h"
#include "SheepGraphics\Context.h"

#include "Window32.h"

#include "CAniSprite.h"


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

    _ScreenWidth = ENGINE->Window->GetWidth();
    _ScreenHeight = ENGINE->Window->GetHeight();

    m_renderContext = DirectSheep::RenderContext::Allocate();

    m_renderContext->Initialize(ENGINE->Window->GetHandle(), (float)_ScreenWidth, (float)_ScreenHeight);

    Message m(Message::GFXDeviceInit);
    ENGINE->SystemMessage(m);

    spritepShader = SetPShader("Generic.hlsl");

    spritevShader = SetVShader("Generic.hlsl");

    m_renderContext->CreateConstantBuffer(spriteContext, 160);

    m_renderContext->CreateVertexBuffer(spriteQuad, 120);

    
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

    // Post Draw
    for (auto it = ENGINE->Spaces().begin(); it != ENGINE->Spaces().end(); ++it)
    {
      space = *it;
      if (!space->Hidden())
        space->hooks.Call("PostDraw");
    }
    Lua::CallFunc(ENGINE->Lua(), "hook.Call", "PostDraw");

    Message m(Message::PostDraw);
    ENGINE->SystemMessage(m);
    
	}

  void SheepGraphics::StartFrame()
  {
    m_renderContext->ClearBackBuffer();
    m_renderContext->ClearDepthBuffer();
    UpdateCamera();
  }

  void SheepGraphics::FinishFrame()
  {
    m_renderContext->Present();
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

  DirectSheep::Handle SheepGraphics::SetPShader(const std::string& Shader)
  {
    for(auto it : m_pshaderMap)
    {
      if(it.first == Shader)
        return it.second;
    }
      DirectSheep::Handle temp;

      m_renderContext->CreatePixelShader(temp, Shader);

      m_pshaderMap[Shader] = temp;

      return m_pshaderMap[Shader];
    }

  DirectSheep::Handle SheepGraphics::SetVShader(const std::string& Shader)
  {
    for(auto it : m_vshaderMap)
    {
      if(it.first == Shader)
        return it.second;
    }

    DirectSheep::InputLayout layout;
    layout.push_back(DirectSheep::InputElement("POSITION",(DirectSheep::Format)DXGI_FORMAT_R32G32B32_FLOAT));
    layout.push_back(DirectSheep::InputElement("TEXCOORD",(DirectSheep::Format)DXGI_FORMAT_R32G32_FLOAT));
    DirectSheep::Handle temp;
    m_renderContext->CreateVertexShader(temp, Shader, layout);
    
    m_vshaderMap[Shader] = temp;
    return m_vshaderMap[Shader];
  }

  void SheepGraphics::SetColor(Vec4 Color)
  {
    m_renderContext->SetBlendCol(Color.R, Color.G, Color.B, Color.A);
  }

  void SheepGraphics::DrawSprite(Sprite *sprite)
  {

    m_renderContext->BindVertexShader(spritevShader);

    m_renderContext->BindPixelShader(spritepShader);

    m_renderContext->BindVertexBuffer(spriteQuad,20,0);

    m_renderContext->BindTexture(0,sprite->GetTexture());

    m_renderContext->BindConstantBuffer(0, spriteContext, DirectSheep::VERTEX_SHADER);
    m_renderContext->BindConstantBuffer(0, spriteContext, DirectSheep::PIXEL_SHADER);

    m_renderContext->Draw(6,0);
  }

  void SheepGraphics::RawDraw(void)
  {
    m_renderContext->BindVertexShader(spritevShader);

    m_renderContext->BindPixelShader(spritepShader);

    m_renderContext->BindVertexBuffer(spriteQuad,20,0);

    m_renderContext->BindConstantBuffer(0, spriteContext, DirectSheep::VERTEX_SHADER);
    m_renderContext->BindConstantBuffer(0, spriteContext, DirectSheep::PIXEL_SHADER);

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
    DirectSheep::Dimension texSize = m_renderContext->GetTextureSize(texture);
    return Vec2((float)texSize.width, (float)texSize.height);
  }
}