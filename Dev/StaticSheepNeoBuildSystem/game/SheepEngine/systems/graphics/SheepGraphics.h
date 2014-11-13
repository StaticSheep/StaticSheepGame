/*****************************************************************
Filename: GraphicsInterface.h
Project: 
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once


#include <wtypes.h>
#include <unordered_map>

#include "components/sprites/CSprite.h"
#include "components/camera/CCamera.h"

#include "systems/System.h"

#include "SheepGraphics/Handle.h"

#include "types/vectors/Vec2.h"
#include "types/vectors/Vec4.h"

namespace DirectSheep
{
  class RenderContext;
};


namespace Framework
{

  namespace DebugData
  {
    struct SheepGraphics
    {
      unsigned numTextures;
      unsigned numShaders;

      unsigned numDrawCalls;
      unsigned numBatchedCalls;
      unsigned numTextDraws;
    };
  }

	class SheepGraphics : public ISystem
	{
	public:
    // Update per frame
		void Update(float dt);

    virtual void RegisterComponents();

    virtual std::string GetName() {return "SheepGraphics";};
		// Sheep graphics interface constructor
    SheepGraphics(void* rc = nullptr);
		// Sheep graphics interface deconstructor
		~SheepGraphics();

    void SetPosition(float x, float y, float Z);
    void SetRotation(float theta);
    void SetSize(float x, float y);
    void SetColor(Vec4 Color);
    void SetUV(Vec2 uvBegin, Vec2 uvEnd);

    DirectSheep::Handle SetTexture(const std::string& Texture);
    DirectSheep::Handle SetVShader(const std::string& Shader);
    DirectSheep::Handle SetPShader(const std::string& Shader);

    void RawDraw(void);

    void BindTexture(int ID);

    void SetWireframe(bool iswired);
    void SetUseCamera(bool useCam);
    void DrawSprite(Sprite *sprite);
    int GetTextureID(const std::string& texture);
    Vec2 GetTextureDim(DirectSheep::Handle texture);
    void DrawSpriteText(const char * text, float size, const char * font);


    bool LoadAssets(std::string& filepath);
    void ReceiveMessage(Message& msg);
    void* GetDevice(void);

	private:

    // Initialization call
		void Initialize(void);

    void InitGeometry();

    void LoadAssets();

    void StartFrame(void);

    void FinishFrame(void);

		void Draw(void);

    std::unordered_map<std::string, DirectSheep::Handle> m_textureMap;

    std::unordered_map<std::string, DirectSheep::Handle> m_vshaderMap;

    std::unordered_map<std::string, DirectSheep::Handle> m_pshaderMap;

    DirectSheep::Handle spritepShader;

    DirectSheep::Handle spritevShader;

    DirectSheep::Handle spriteQuad;

    DirectSheep::Handle spriteContext;

    DebugData::SheepGraphics m_debugData;


  public:

    Handle CurrentCamera;
    HWND _HWnd;
    int _ScreenWidth;
    int _ScreenHeight;


    DirectSheep::RenderContext *m_renderContext;

	};

	extern SheepGraphics* GRAPHICS;
}

