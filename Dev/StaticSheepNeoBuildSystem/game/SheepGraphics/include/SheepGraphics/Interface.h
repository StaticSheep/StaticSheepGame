#pragma once

#include <string>
#include <vector>
#include <Windows.h>

#if SHEEPGRAPHICS
#include "Vertices.h"
#include "DirectXIncludes.h"
#include "Tex2d.h"
#include "RenderContext.h"
#include "Effects.h"
#include "Effects\plight_effect.h"
#include "Graphics.h"
#include "model.h"
#include "vertex_types.h"
#include "include/SheepGraphics/api.h"
#else
#include "SheepGraphics/api.h"
#endif

namespace DirectSheep
{
  class Handle;
} // namespace Graphics


namespace DirectSheep
{

class Interface
{
  public:

    GFX_API static Interface * Allocate(void);
    GFX_API void UpdateCamera(float x, float y, float fov);

    //Returns true if the RenderContext is Initialized, else false
    GFX_API bool IsInitialized(void) const;
    //Initializes the RenderContext
    //Returns true if successful, else false
    GFX_API bool Initialize(HWND hwnd, float height, float width);
    //Uninitializes the RenderContext
    GFX_API void Uninitialize(Interface * rCon);

    /////////////////////////////////////////////////////////////
    //                     DRAW FUNCTIONS                      //
    /////////////////////////////////////////////////////////////

   GFX_API void Draw(unsigned vertexCount, unsigned vertexStart = 0);
   GFX_API void DrawSpriteText(const char * text, float size = 32, const char * font = "Arial");
   GFX_API void DrawBatched(DirectSheep::Handle texture);
   GFX_API void StartBatch();
   GFX_API void EndBatch();
   GFX_API void frameStart();
   GFX_API void frameEnd();
   GFX_API void Present(void);

    /////////////////////////////////////////////////////////////
    //                    BIND FUNCTIONS                       //
    /////////////////////////////////////////////////////////////

   GFX_API void BindTexture(unsigned slot, const Handle& texHandle);

   /////////////////////////////////////////////////////////////
   //                    CREATE FUNCTIONS                     //
   /////////////////////////////////////////////////////////////

   GFX_API void CreateTexture(Handle& handle, const std::string& filename);

   GFX_API float* GetTextureSize(DirectSheep::Handle texture);
   GFX_API void* ExternalGetDevice(void);

    /////////////////////////////////////////////////////////////
    //                    SETTER FUNCTIONS                     //
    /////////////////////////////////////////////////////////////

   GFX_API void SetClearColor(const float r, const float g, const float b, const float a);
   GFX_API void SetVSync(bool vsync);

   GFX_API void SetUV(float x1, float y1, float x2, float y2);
   GFX_API void SetPosition(const float x, const float y);
   GFX_API void SetRotation(const float theta);
   GFX_API void SetDimensions(const float w, const float h);
   GFX_API void SetBlendCol(const float r, const float g, const float b, const float a);
   GFX_API void SetUseCam(bool camUse);
   GFX_API void ClearBackBuffer(void);
   GFX_API void ClearDepthBuffer(void);

   GFX_API void changeResolution(void)
   {

   };

    /////////////////////////////////////////////////////////////
    //                 PUBLIC RELEASE FUCNTION                 //
    /////////////////////////////////////////////////////////////


#if SHEEPGRAPHICS
  private:


    struct Camera
    {
      iMat4         view;
      iMat4         proj;
      iMat4         viewProj;
      bool          used;
    };


    //////////////
    //  System  //
    //////////////
    bool        m_initialized;
    bool        m_vsync;

    /////////////
    // DirectX //
    /////////////
    Color                        m_clearColor;

    Vec4                         m_spriteBlend;

    Graphics*                     m_graphics;
    RenderContext*                m_Context;

    /////////////////////////////////
    // Other render configurations //
    /////////////////////////////////
    Camera                                   m_camera;

    Vec4                                     m_UV;
    Vec2                                     m_position;
    Vec2                                     m_scale;
    Color                                    m_BlendCol;
    float                                    m_theta;
    Tex2D*                                   m_currTex;

    Mat4          m_world;
    Mat4 m_view;
    Mat4 m_projection;
    Light m_cursorLight;

    ///////////////
    // Resources //
    ///////////////
    std::vector<Tex2D> m_TextureRes;
    std::vector<PointLight*> m_EffectRes;
    Model<PositionVertex>*             m_posModel;
    IFW1FontWrapper*                  m_FontWrapper;
    IFW1Factory*                      m_FontFactory;

    std::vector<DirectSheep::Handle>         m_handles;
#endif
};

} //namespace Graphics

