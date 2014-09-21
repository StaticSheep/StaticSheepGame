/*****************************************************************
Filename: GraphicsInterface.h
Project: 
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "System.h"
#include <wtypes.h>
#include <unordered_map>

namespace DirectSheep
{
  class RenderContext;
};

namespace Framework
{
  class Camera;
	class SheepGraphics : public ISystem
	{
	public:
    // Update per frame
		void Update(float dt);

    virtual void RegisterComponents();

    virtual std::string GetName() {return "SheepGraphics";};

		// Sheep graphics interface constructor
		SheepGraphics();
		// Sheep graphics interface deconstructor
		~SheepGraphics();

    void SetPosition(float x, float y);
    void SetRotation(float theta);
    void SetSize(float x, float y);
    void SetColor(Vec4 Color);

    DirectSheep::Handle SetTexture(const std::string& Texture);
    DirectSheep::Handle SetVShader(const std::string& Shader);
    DirectSheep::Handle SetPShader(const std::string& Shader);

    void RawDraw(void);

    void BindTexture(int ID);

    void SetUseCamera(bool useCam);
    void DrawSprite(Sprite *sprite);
    int GetTextureID(const std::string& texture);
    void DrawSpriteText(const char * text, float size, const char * font);

	private:

    // Initialization call
		void Initialize(void);

    void InitGeometry();

    void LoadAssets();

    void StartFrame(void);

    void FinishFrame(void);

		void Draw(void);

    DirectSheep::RenderContext *m_renderContext;

    std::unordered_map<std::string, DirectSheep::Handle> m_textureMap;

    std::unordered_map<std::string, DirectSheep::Handle> m_vshaderMap;

    std::unordered_map<std::string, DirectSheep::Handle> m_pshaderMap;

    DirectSheep::Handle spritepShader;

    DirectSheep::Handle spritevShader;

    DirectSheep::Handle spriteQuad;

    DirectSheep::Handle spriteContext;

  public:

    Camera* CurrentCamera;
    HWND _HWnd;
    int _ScreenWidth;
    int _ScreenHeight;


	};

	extern SheepGraphics* GRAPHICS;
}

