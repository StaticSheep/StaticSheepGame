#pragma once

#include "DataTypes.h"
#include "Vec2D.h"

namespace DirectSheep
{
  class Handle;
} // namespace Graphics


namespace DirectSheep
{

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

    GFX_API static RenderContext * Allocate(void);

    //Returns true if the RenderContext is Initialized, else false
    GFX_API bool IsInitialized(void) const;
    //Initializes the RenderContext
    //Returns true if successful, else false
    GFX_API bool Initialize(HWND hwnd, float height, float width);
    //Uninitializes the RenderContext
    GFX_API void Uninitialize(RenderContext * rCon);

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


   //////////////////////////////////////////////////////////////
   //                     CAMERA FUNCTIONS                     //
   //////////////////////////////////////////////////////////////
   GFX_API Handle NewCamera();
   GFX_API void SetCamState(int camState);
   GFX_API void SetCamPosition(Handle Camera, float x, float y);
   GFX_API void SetCamPosition(Handle Camera, float x, float y, float z);
   GFX_API void SetCamScale(Handle Camera, float width, float height);
   GFX_API void SetCamFOV(Handle Camera, float FOV);
   GFX_API void SetCamActive(Handle Camera);
   GFX_API void SetCamDefault();
   GFX_API Handle GetCamActive();

    /////////////////////////////////////////////////////////////
    //                    CREATE FUNCTIONS                     //
    /////////////////////////////////////////////////////////////
    
   GFX_API bool CreateTexture(Handle& handle, const std::string& filename);
   GFX_API bool CreateVertexBuffer(Handle& handle, size_t size);
           bool CreateIndexBuffer(Handle& handle, size_t size);
   GFX_API bool CreateConstantBuffer(Handle& handle, size_t size);
           bool CreateRenderTarget(Handle& handle, const RenderTargetMode mode, const Format format, const float downsamplePercentage = 1.0f, const Dimension& dim = Dimension());
           bool CreateDepthBuffer(void);
   GFX_API bool CreateFontWrapper(void);

    /////////////////////////////////////////////////////////////
    //                    BIND FUNCTIONS                       //
    /////////////////////////////////////////////////////////////

   GFX_API void BindVertexShader(const Handle& vsHandle);
   GFX_API void BindPixelShader(const Handle& psHandle);
   GFX_API void BindTexture(unsigned slot, const Handle& texHandle);
   GFX_API void BindTextures(unsigned count, const Handle texHandles[], unsigned startSlot = 0);
   GFX_API void BindVertexBuffer(const Handle& vbHandle, size_t stride, size_t offset = 0);
   GFX_API void BindVertexBuffers(unsigned count, const Handle vertexBuffers[], size_t strides[], size_t offsets[]);
   GFX_API void BindIndexBuffer(const Handle& ibHandle);
   GFX_API void BindConstantBuffer(unsigned slot, const Handle& cbHandle, ObjectType shaderType =VERTEX_SHADER);
   GFX_API void BindRenderTarget(const Handle& rtHandle);
   GFX_API void BindRenderTargets(unsigned count, const Handle rtHandle, ...);
   GFX_API void BindDepthBuffer(void);

    /////////////////////////////////////////////////////////////
    //                    SETTER FUNCTIONS                     //
    /////////////////////////////////////////////////////////////

   GFX_API void setWireFrame(bool isWired);
   GFX_API void SetSpriteFlip(bool xFlip, bool yFlip);
   GFX_API void Resize(float width, float height);
   GFX_API void SetClearColor(const float r, const float g, const float b, const float a);
   GFX_API void SetTargetWindow(const HWND& hwnd);
   GFX_API void SetFullscreen(const bool fullscreen);
   GFX_API void SetPrimitiveTopology(const PrimitiveTopology primitiveTopology);
   GFX_API void SetBlendMode(const BlendMode blendMode);
   GFX_API void SetViewport(int xOffset, int yOffset, Dimension dim);
   GFX_API void SetViewport(const Viewport& viewport);
   GFX_API void SetDisplayMode(unsigned modeIndex);
   GFX_API void SetDisplayMode(const Dimension& resolution);
   GFX_API void SetVSync(bool vsync);

   GFX_API void SetUV(float x1, float y1, float x2, float y2);
   GFX_API void SetPosition(const float x, const float y, const float z);
   GFX_API void SetRotation(const float theta);
   GFX_API void SetDimensions(const float w, const float h);
   GFX_API void SetBlendCol(const float r, const float g, const float b, const float a);

    /////////////////////////////////////////////////////////////
    //                    GETTER FUNCTIONS                     //
    /////////////////////////////////////////////////////////////

   GFX_API Handle GetBackBuffer(void) const;
   GFX_API bool GetFullscreen(void) const;
   GFX_API void* ExternalGetDevice(void) const;
    ID3D11Device* GetDevice(void) const;
    ID3D11DeviceContext* GetDeviceContext(void) const;
   GFX_API const Dimension& GetNativeResolution(void) const;
   GFX_API const Viewport& GetViewport(void) const;
    int GetCurrentDisplayModeIndex(void) const;
    const std::string& GetGraphicsCardInfo(void) const;
   GFX_API const Dimension GetTextureSize(const Handle& texHandle) const;

    /////////////////////////////////////////////////////////////
    //                    UTILITY FUNCTIONS                    //
    /////////////////////////////////////////////////////////////

    void CopyData(const Handle& handle, const void *data, size_t size = 0);
   GFX_API void ClearRenderTarget(const Handle& handle,
     float r, float g, float b, float a);
   GFX_API void ClearBackBuffer(void);
   GFX_API void ClearDepthBuffer(void);
   GFX_API Framework::Vec2D MeasureString(const char* text, float size,
     const char* font);

    /////////////////////////////////////////////////////////////
    //                 PUBLIC RELEASE FUCNTION                 //
    /////////////////////////////////////////////////////////////

    void Release(const Handle &handle);

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
      Format                   format;

      float            downsamplePercentage;
      RenderTargetMode mode;
      Dimension        size;
    };

    struct Font
    {
      Font() : m_fontFactory(NULL), m_fontWrapper(NULL) {}
      void Release() { SafeRelease(m_fontWrapper); SafeRelease(m_fontFactory); }
      IFW1Factory     *m_fontFactory;
      IFW1FontWrapper *m_fontWrapper;
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
    ID3D11Device                *m_device;   
    ID3D11DeviceContext         *m_deviceContext;
    Font                         m_font;               
    int                         m_displayModeIndex;
                                
    ID3D11RenderTargetView      *m_backBuffer;
    Dimension                    m_backBufferSize;
    Viewport                     m_viewport;
    Color                        m_clearColor;
    PrimitiveTopology            m_primative;

    Transform                    m_spriteTrans;
    Vec4                         m_spriteBlend;

    /////////////////////////////////
    // Other render configurations //
    /////////////////////////////////
    ID3D11SamplerState                      *m_sampleStates[2];
    ID3D11RasterizerState                   *m_rastState[RastStates::NumStates];
    std::map<BlendMode, ID3D11BlendState *>  m_blendStateMap;
    DepthBuffer                              m_depthBuffer;
    Handle                                   m_camera;
    Handle                                   m_Perspective;
    Handle                                   m_Ortho;
    Handle                                   m_orthoScreen;
    Handle                                   m_editor;
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

    /////////////
    // Batcher //
    /////////////
    
    std::unique_ptr<DirectX::SpriteBatch> m_batcher;
    DirectX::SpriteEffects                m_flip = DirectX::SpriteEffects_None;

    /////////////
    // Effects //
    /////////////

    GenEffect*                               m_genericEffect;
    PointLight*                              m_PointLight;


    ///////////
    //cleanup//
    ///////////

    std::vector<DirectSheep::Handle>         m_handles;
#endif
};

} //namespace Graphics
