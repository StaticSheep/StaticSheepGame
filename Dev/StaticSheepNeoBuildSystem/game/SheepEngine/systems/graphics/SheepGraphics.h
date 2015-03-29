/*****************************************************************
Filename: SheepGraphics.h
Project: 
Author(s): Scott Nelson (primary)
           Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once


#include <wtypes.h>
#include <unordered_map>

#include "components/sprites/CSprite.h"
#include "components/camera/CCamera.h"
#include "components/lights/CPointLight.h"

#include "../SheepGraphics/Atlas/SpineAtlas.h"

#include "systems/System.h"

#include "Handle.h"

#include "types/vectors/Vec2.h"
#include "types/vectors/Vec4.h"

namespace DirectSheep
{
  class RenderContext;
  class Camera;
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

    // Sheep graphics interface constructor
    SheepGraphics(void* rc = nullptr);
    // Sheep graphics interface deconstruction
    ~SheepGraphics();

    /* =========== Core Engine Functions ============ */
    // Update per frame
		void Update(float dt);
    void RegisterComponents();
    void ReceiveMessage(Message& msg);
    std::string GetName() {return "SheepGraphics";};

    /* =========== Texture Loading / Retrieval Functions ============ */

    // Loads a specified texture into memory and returns a handle to said texture
    DirectSheep::Handle LoadTexture(const std::string& textureName);

    // Returns the index of the texture in the texture array
    int GetTextureID(const std::string& texture);

    // Returns the width/height of a texture
    Vec2 GetTextureDim(DirectSheep::Handle texture);

    bool LoadAtlas(const std::string& path);
    //DirectSheep::AnimationSheet* GetAnimationSheet(std::string& name);

    DirectSheep::Handle GetAtlasHandle(const std::string& atlas);
		
    /* =========== Draw State Functions ============ */

    void SetPosition(float x, float y, float Z);
    void SetRotation(float theta);
    void SetSize(float x, float y);
    void SetColor(Vec4 Color);
    void SetUV(Vec2 uvBegin, Vec2 uvEnd);
    void SetWireframe(bool iswired);
    void SetCamState(int camState);
    void SetSpriteFlip(bool x, bool y);

    // only affects batched draws
    void SetObjectOrigin(float x, float y);

    // Binds a texture to be used in the next draw call (Raw draw only)
    void BindTexture(int ID);
    
    /* =========== Draw Functions ============ */

    // Unbatched raw draw, uses draw state information
    void RawDraw(void);

    // Stores the current draw state information to be drawn with a corresponding texture
    void DrawBatched(DirectSheep::Handle texture);

    // Unbatched raw draw for sprite text
    void DrawSpriteText(const char * text, int fontIndex, Vec2D scale);
    int GetFontIndex(const char * fontName);

    void BatchPointLight(Vec3D position, Vec4D brightness, Vec3D attenuation);
    void DrawPointLights(bool isLight);

    /* =========== Graphics Engine Functions ============ */

    void* GetDevice(void);

    void ActivateDefaultCamera(void);

    /* =========== Utility Functions ============ */

    Vec2 MeasureString(const char* text, Vec2D scale, int fontIndex);

    static bool IsFullScreen();
    static void SetFullScreen(bool isFull);

    /* =========== Camera Functions ============ */

    // Returns the actual pointer to the camera
    DirectSheep::Camera* RetrieveCamera(DirectSheep::Handle camHandle);

    DirectSheep::Handle GetActiveCamera();
    void SetActiveCamera(DirectSheep::Handle camHandle);
    

	private:

    // Initialization call
		void Initialize(void);

    bool LoadAssets(std::string& filepath);

    void StartFrame(void);

    void FinishFrame(void);

		void Draw(void);

    std::unordered_map<std::string, DirectSheep::Handle> m_textureMap;
    std::unordered_map<std::string, DirectSheep::Handle> m_atlasMap;
    std::unordered_map<std::string, int> m_fontMap;

    DirectSheep::Handle spriteQuad;

    DirectSheep::Handle spriteContext;

    DebugData::SheepGraphics m_debugData;

    static DirectSheep::RenderContext *m_renderContext;

    static bool m_FullScreen;
  public:

    Handle currentCamera;
    HWND _HWnd;
    int _ScreenWidth;
    int _ScreenHeight;

    DirectSheep::RenderContext* RC() { return m_renderContext; }
    

	};

	extern SheepGraphics* GRAPHICS;
}

