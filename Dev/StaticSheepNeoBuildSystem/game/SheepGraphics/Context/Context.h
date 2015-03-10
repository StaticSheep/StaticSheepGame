/*****************************************************************
Filename: Context.h
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include <map>
#include <unordered_map>
#include "Vertices.h"
#include "DataTypes.h"
#include "Vec2D.h"
#include "Texture/Tex2d.h"
#include "model.h"
#include "Light.h"
#include "SafeRelease.h"
#include "Handle.h"
#include "Matrix4D.h"
#include <stack>
#include "CommonStates.h"
#include "Atlas\SpineAtlas.h"

namespace DirectSheep
{
  class Handle;
} // namespace Graphics


namespace DirectSheep
{
  typedef std::pair<DirectX::VertexPositionColor,
    DirectX::VertexPositionColor > DebugLine;

  typedef std::tuple < DirectX::VertexPositionColor,
    DirectX::VertexPositionColor,
    DirectX::VertexPositionColor > DebugTriangle;

  typedef std::tuple < DirectX::VertexPositionColor,
    DirectX::VertexPositionColor, DirectX::VertexPositionColor,
    DirectX::VertexPositionColor > DebugQuad;

  typedef std::vector<DirectX::VertexPositionColor> DebugPoly;

  extern float SCREEN_WIDTH;
  extern float SCREEN_HEIGHT;

  class Camera;
  class GenEffect;
  class PointLight;

    struct Viewport
    {
      Dimension dim;
      float offsetX;
      float offsetY;
    };

class RenderContext
{
  public:
    RenderContext(void);
    ~RenderContext(void);

    static RenderContext * Allocate(void);

    //Returns true if the RenderContext is Initialized, else false
    bool IsInitialized(void) const;
    //Initializes the RenderContext
    //Returns true if successful, else false
    bool Initialize(HWND hwnd, float height, float width);
    //Uninitializes the RenderContext
    void Uninitialize(RenderContext * rCon);

    void PurgeTextures();

    /////////////////////////////////////////////////////////////
    //                     DRAW FUNCTIONS                      //
    /////////////////////////////////////////////////////////////

    /* --- General Draw Functions --- */
   void Draw(unsigned vertexCount, unsigned vertexStart = 0);
   void DrawSpriteText(const char * text, int index, Framework::Vec2D& scale);
   void DrawBatched(DirectSheep::Handle texture);
   void DrawLightBatched(DirectSheep::Handle texture);
   // Layer: 0 = Background, 1 = Foreground, 2 = Front
   void BatchPointLight(Framework::Vec3D position,
     Framework::Vec4D brightness, Framework::Vec3D attenuation);

   void BatchPointLight(Framework::Vec3D position,
     Framework::Vec4D brightness, Framework::Vec3D attenuation, unsigned layer);
   
   void SetLayer(unsigned layer)
   {
     m_curLayer = layer;
   }

   /* --- Primitive Draw Functions --- */
   void DrawLine(Vec3 start, Vec3 end, Color startColor, Color endColor);
   void DrawLine(Vec3 start, Vec3 end);
   void DrawQuad(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4);
   void DrawTriangle(Vec3 v1, Vec3 v2, Vec3 v3);

   void StartBatch(unsigned maxSpriteLayers, bool fullBright);
   void EndBatch();

   void FrameStart();
   void FrameEnd();

   void Present(void);


   //////////////////////////////////////////////////////////////
   //                     CAMERA FUNCTIONS                     //
   //////////////////////////////////////////////////////////////
   Handle NewCamera();

   void SetCamState(int camState);
   /*void SetCamPosition(Handle camera, float x, float y);
   void SetCamPosition(Handle camera, float x, float y, float z);
   void SetCamScale(Handle camera, float width, float height);
   void SetCamFOV(Handle camera, float FOV);*/
   void SetActiveCamera(Handle camera);
   
   Handle GetActiveCamera();
   static inline DirectSheep::Camera* RetrieveCamera(Handle camera)
   {
     return (DirectSheep::Camera*)camera.ptr;
   }

   Framework::Mat4D GetCamViewProj(Handle camera);

   void ActivateDefaultCamera();

    /////////////////////////////////////////////////////////////
    //                    CREATE FUNCTIONS                     //
    /////////////////////////////////////////////////////////////
    
   bool CreateTexture(Handle& handle, const std::string& filename);
   bool CreateVertexBuffer(Handle& handle, size_t size);
   bool CreateIndexBuffer(Handle& handle, size_t size);
   bool CreateConstantBuffer(Handle& handle, size_t size);
   bool CreateRenderTarget(Handle& handle, const DXGI_FORMAT format,
     const Dimension& dim = Dimension(), bool depthbuffer = false);
   bool CreateDepthBuffer(void);
   int AddFont(const char* fontname,const char* filename);

   bool CreateAtlas(Handle& handle, const std::string& filename);

    /////////////////////////////////////////////////////////////
    //                    BIND FUNCTIONS                       //
    /////////////////////////////////////////////////////////////

   void BindVertexShader(const Handle& vsHandle);
   void BindPixelShader(const Handle& psHandle);
   void BindTexture(unsigned slot, const Handle& texHandle);
   void BindTextures(unsigned count, const Handle texHandles[], unsigned startSlot = 0);
   void BindVertexBuffer(const Handle& vbHandle, size_t stride, size_t offset = 0);
   void BindVertexBuffers(unsigned count, const Handle vertexBuffers[], size_t strides[], size_t offsets[]);
   void BindIndexBuffer(const Handle& ibHandle);
   void BindConstantBuffer(unsigned slot, const Handle& cbHandle, ObjectType shaderType =VERTEX_SHADER);
   void BindRenderTarget(const Handle& rtHandle);
   void BindRenderTargets(unsigned count, const Handle rtHandle, ...);
   void BindDepthBuffer(void);

    /////////////////////////////////////////////////////////////
    //                    SETTER FUNCTIONS                     //
    /////////////////////////////////////////////////////////////

   void SetWireFrame(bool isWired);
   void SetSpriteFlip(bool xFlip, bool yFlip);
   void Resize(float width, float height);
   void SetClearColor(const float r, const float g, const float b, const float a);
   void SetTargetWindow(const HWND& hwnd);
   void SetFullscreen(const bool fullscreen);
   void SetPrimitiveTopology(const PrimitiveTopology primitiveTopology);
   void SetBlendMode(const BlendMode blendMode);
   void SetViewport(int xOffset, int yOffset, Dimension dim);
   void SetViewport(const Viewport& viewport);
   void SetDisplayMode(unsigned modeIndex);
   void SetDisplayMode(const Dimension& resolution);
   void SetVSync(bool vsync);

   void SetUV(float x1, float y1, float x2, float y2);
   void SetPosition(const float x, const float y, const float z);
   void SetRotation(const float theta);
   void SetObjectOrigin(const float x, const float y);
   void SetDimensions(const float w, const float h);
   void SetBlendCol(const float r, const float g, const float b, const float a);

    /////////////////////////////////////////////////////////////
    //                    GETTER FUNCTIONS                     //
    /////////////////////////////////////////////////////////////

   Handle GetBackBuffer(void) const;
   bool GetFullscreen(void) const;
   void* ExternalGetDevice(void) const;
   ID3D11Device* GetDevice(void) const;
   ID3D11DeviceContext* GetDeviceContext(void) const;
   const Dimension& GetNativeResolution(void) const;
   const Viewport& GetViewport(void) const;
   int GetCurrentDisplayModeIndex(void) const;
   const std::string& GetGraphicsCardInfo(void) const;
   const Dimension GetTextureSize(const Handle& texHandle) const;

    /////////////////////////////////////////////////////////////
    //                    UTILITY FUNCTIONS                    //
    /////////////////////////////////////////////////////////////

   void CopyData(const Handle& handle, const void *data, size_t size = 0);
   void ClearRenderTarget(const Handle& handle,
     float r, float g, float b, float a);
   void ClearBackBuffer(void);
   void ClearDepthBuffer(void);
   Framework::Vec2D MeasureString(const char* text, Framework::Vec2D scale,
     int fontIndex);

    /////////////////////////////////////////////////////////////
    //                 PUBLIC RELEASE FUCNTION                 //
    /////////////////////////////////////////////////////////////

    void Release(const Handle &handle);


    static RenderContext* Context;
  private:

    /////////////////////////////////////////////////////////////
    //              PRIVATE INITIALIZE FUNCTIONS               //
    /////////////////////////////////////////////////////////////

    void InitializeDXGI(void);
    void InitializeDisplayModes(void);
    void InitializeDeviceAndSwapChain(void);
    void InitializeRasterizerState(void);
    void InitializeBackBuffer(void);
    void InitializeBlendModes(void);
    void InitializeSamplerState(void);
    void InitializeDepthState(void);

    /////////////////////////////////////////////////////////////
    //               INTERNAL RELEASE FUNCTIONS                //
    /////////////////////////////////////////////////////////////

    void ReleaseTextureIntern(const Handle& texture);
    void ReleaseVertexShaderIntern(const Handle& vertexShader);
    void ReleasePixelShaderIntern(const Handle& pixelShader);
    void ReleaseVertexBufferIntern(const Handle& vertexBuffer);
    void ReleaseIndexBufferIntern(const Handle& indexBuffer);
    void ReleaseConstantBufferIntern(const Handle& constantBuffer);
    void ReleaseRenderTargetIntern(const Handle& renderTarget);

    void DrawPointLights(bool isLight, unsigned layer);
    

    void UpdatePrimativeEffect();


#if SHEEPGRAPHICS
    //---------//
    // Structs //
    //---------//

    enum RastStates
    {
      Fill = 0,
      Wire,
      NumStates,
    };

    struct DepthBuffer
    {
      DepthBuffer() : m_depthBuffer(NULL), m_depthState(NULL), texture2D(NULL) {}
      void Release() { SafeRelease(texture2D); SafeRelease(m_depthState); SafeRelease(m_depthBuffer); }
      ID3D11DepthStencilView      *m_depthBuffer;
      ID3D11DepthStencilState     *m_depthState;
      ID3D11Texture2D             *texture2D;
    };

    struct VertexShader
    {
      VertexShader() : vShader(NULL), inputLayout(NULL) {}
      void Release() { SafeRelease(inputLayout); SafeRelease(vShader); }
      ID3D11VertexShader *vShader;
      ID3D11InputLayout  *inputLayout;
    };

    struct RenderTarget
    {
      RenderTarget() : renderTargetView(NULL), shaderResourceView(NULL), texture2D(NULL) {}
      void Release() { SafeRelease(texture2D); SafeRelease(shaderResourceView); SafeRelease(renderTargetView); }
      ID3D11RenderTargetView   *renderTargetView;
      ID3D11ShaderResourceView *shaderResourceView;
      ID3D11Texture2D          *texture2D;
      DepthBuffer              depthBuffer;
      Format                   format;

      float            downsamplePercentage;
      RenderTargetMode mode;
      Dimension        size;
      bool             hasDepthBuffer;
    };

    struct Transform
    {
      Transform() : x(0), y(0), w(64), h(64), theta(0), uvBegin(0,0), uvEnd(1,1) {}
      Transform(float x, float y, float w, float h, float theta) : x(x), y(y), w(w), h(h), theta(theta) {}
      float x;
      float y;
      float z;
      float w;
      float h;
      Vec2 uvBegin;
      Vec2 uvEnd;
      float theta;
    };

    struct Font
    {
      Font() : m_spriteFont(nullptr) {}
      Font(DirectX::SpriteFont* newFont) : m_spriteFont(newFont) {}
      DirectX::SpriteFont* m_spriteFont;
    };

    //////////////
    //  System  //
    //////////////
    bool        m_initialized;
    HWND        m_hwnd;
    Dimension   m_resolution;
    Dimension   m_nativeResolution;
    bool        m_fullscreen;
    bool        m_vsync;
    std::string m_contentPath = "content\\";

    /////////////
    // DirectX //
    /////////////
    IDXGISwapChain              *m_swapChain;
    IDXGIFactory2                *m_factory;
    ID3D11Device                *m_device;
    ID3D11DeviceContext         *m_deviceContext;
    //std::unordered_map<std::string, std::unique_ptr<DirectX::SpriteFont>> m_font;
    std::vector<Font> m_font;
    int                         m_displayModeIndex;
                                
    ID3D11RenderTargetView      *m_backBuffer;
    Dimension                    m_backBufferSize;
    Viewport                     m_viewport;
    Color                        m_clearColor;
    PrimitiveTopology            m_primative;

    Transform                    m_spriteTrans;
    Vec4                         m_spriteBlend;
    Vec2                         m_spriteOrigin;

    /////////////////////////////////
    // Other render configurations //
    /////////////////////////////////
    ID3D11SamplerState                      *m_sampleStates[2];
    ID3D11RasterizerState                   *m_rastState[RastStates::NumStates];
    std::map<BlendMode, ID3D11BlendState *>  m_blendStateMap;
    DepthBuffer                              m_depthBuffer;

    ////////////
    // Camera //
    ////////////
    Handle                                   m_camera;
    Handle                                   m_Perspective;
    Handle                                   m_Ortho;
    Handle                                   m_orthoScreen;
    Handle                                   m_editor;

    Handle                                   m_postEffects;
    std::vector<DirectSheep::Camera*>         m_CameraPool;
    bool                                     m_camUse;

    RastStates                               m_currentRast = RastStates::Fill;

    ///////////////
    // Resources //
    ///////////////
    std::vector<Tex2D>                       m_textureRes;
    std::vector<ID3D11Buffer*>               m_vertexBufferRes;
    std::vector<ID3D11Buffer*>               m_indexBufferRes;
    std::vector<ID3D11Buffer*>               m_constBufferRes;
    std::vector<RenderTarget>                m_renderTargetRes;
    std::vector<SpineAtlas>                  m_atlasRes;

    Handle m_canvasTarget;
    Handle m_lightTarget;

    /////////////
    // Batcher //
    /////////////

    static const int MAX_LAYERS = 3;

    unsigned m_curLayer = 1;
    unsigned m_curMaxLayers = 3;
    bool m_fullbright = false;
    
    std::vector<Light>                    m_PointLights[MAX_LAYERS];

    DirectX::SpriteBatch* m_batcher[MAX_LAYERS];
    DirectX::SpriteBatch* m_lightBatcher[MAX_LAYERS];

    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>
      m_primitiveBatch;
    std::unique_ptr<DirectX::BasicEffect> m_primativeEffect;
    ID3D11InputLayout* m_primativeLayout;

    std::unique_ptr<DirectX::CommonStates> m_states;

    std::stack <DebugLine> m_lineList;
    std::stack <DebugTriangle> m_triangleList;
    std::stack <DebugQuad> m_quadList;
    std::stack <DebugPoly> m_polyList;

    DirectX::SpriteEffects                m_flip = DirectX::SpriteEffects_None;

    /////////////
    // Effects //
    /////////////

    GenEffect*                               m_genericEffect;
    PointLight*                              m_PointLight;

    Model<PositionVertex>*                   m_PLightModel;
    Model<PositionTextureVertex>*                   m_quad;
    

    ///////////
    //cleanup//
    ///////////

    std::vector<DirectSheep::Handle>         m_handles;
#endif
};

} //namespace Graphics
