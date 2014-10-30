#pragma once

#include "DataTypes.h"

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

    GFX_API static RenderContext * Allocate(void);
    GFX_API void UpdateCamera(float x, float y, float fov);

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
   GFX_API void DrawIndexed(unsigned indexCount, unsigned indexStart = 0, unsigned vertexStart = 0);
   GFX_API void DrawInstanced(unsigned vertexCount, unsigned instanceCount, unsigned vertexStart = 0, unsigned instanceStart = 0);
   GFX_API void DrawIndexInstanced(unsigned indexCountPerInstance, unsigned instanceCount, unsigned indexStart = 0, unsigned vertexStart = 0, unsigned instanceStart = 0);
   GFX_API void DrawBatched(DirectSheep::Handle texture);
   GFX_API void StartBatch();
   GFX_API void EndBatch();
   GFX_API void frameStart();
   GFX_API void frameEnd();
   GFX_API void Present(void);

    /////////////////////////////////////////////////////////////
    //                    CREATE FUNCTIONS                     //
    /////////////////////////////////////////////////////////////
    
   GFX_API bool CreateVertexShader(Handle& handle, const std::string& filename, const InputLayout& inputLayout, const std::string& entryFunc = "VShader");
   GFX_API bool CreatePixelShader(Handle& handle, const std::string& filename, const std::string& entryFunc = "PShader");
   GFX_API bool CreateTexture(Handle& handle, const std::string& filename);
   GFX_API bool CreateTexture(Handle& handle, const void *data, const Dimension& dim, const Format format);
   GFX_API bool CreateVertexBuffer(Handle& handle, size_t size);
   GFX_API bool CreateIndexBuffer(Handle& handle, size_t size);
   GFX_API bool CreateConstantBuffer(Handle& handle, size_t size);
   GFX_API bool CreateRenderTarget(Handle& handle, const RenderTargetMode mode, const Format format, const float downsamplePercentage = 1.0f, const Dimension& dim = Dimension());
   GFX_API bool CreateDepthBuffer(void);
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

   GFX_API void SetClearColor(const float r, const float g, const float b, const float a);
   GFX_API void SetClearColor(const Color& color);
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
   GFX_API void SetUseCam(bool camUse);

    /////////////////////////////////////////////////////////////
    //                    GETTER FUNCTIONS                     //
    /////////////////////////////////////////////////////////////

   GFX_API Handle GetBackBuffer(void) const;
   GFX_API bool GetFullscreen(void) const;
   GFX_API void* ExternalGetDevice(void) const;
   GFX_API const Dimension& GetNativeResolution(void) const;
   GFX_API const Viewport& GetViewport(void) const;
   GFX_API const Dimension& GetTextureSize(const Handle& texHandle) const;

    /////////////////////////////////////////////////////////////
    //                    UTILITY FUNCTIONS                    //
    /////////////////////////////////////////////////////////////

   GFX_API void ClearRenderTarget(const Handle& handle, float r, float g, float b, float a);
   GFX_API void ClearRenderTarget(const Handle& handle, Color clearColor);
   GFX_API void ClearBackBuffer(void);
   GFX_API void ClearDepthBuffer(void);

    /////////////////////////////////////////////////////////////
    //                 PUBLIC RELEASE FUCNTION                 //
    /////////////////////////////////////////////////////////////


  private:

    /////////////////////////////////////////////////////////////
    //              PRIVATE INITIALIZE FUNCTIONS               //
    /////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////
    //               INTERNAL RELEASE FUNCTIONS                //
    /////////////////////////////////////////////////////////////



#if SHEEPGRAPHICS
    //---------//
    // Structs //
    //---------//
    struct Texture
      ID3D11ShaderResourceView *shaderResourceView;
      ID3D11Texture2D          *texture;
      Dimension                 size;
    };

    struct DepthBuffer
      ID3D11DepthStencilView      *m_depthBuffer;
      ID3D11DepthStencilState     *m_depthState;
      ID3D11Texture2D             *texture2D;
    };

    struct VertexShader
      ID3D11VertexShader *vShader;
      ID3D11InputLayout  *inputLayout;
    };

    struct RenderTarget
      ID3D11RenderTargetView   *renderTargetView;
      ID3D11ShaderResourceView *shaderResourceView;
      ID3D11Texture2D          *texture2D;
      Format                   format;

      float            downsamplePercentage;
      RenderTargetMode mode;
      Dimension        size;
    };

    struct Font
      IFW1Factory     *m_fontFactory;
      IFW1FontWrapper *m_fontWrapper;
    };

    struct Camera
    {
      iMat4         view;
      iMat4         proj;
      iMat4         viewProj;
      bool         used;
    };

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
    std::string m_graphicsCardInfo;
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
    IDXGIFactory                *m_factory;
    IDXGIAdapter                *m_adapter;
    IDXGIOutput                 *m_output;
    Font                         m_font;               
    int                         m_displayModeIndex;
    std::vector<DisplayMode>    m_displayModes;
    std::vector<DXGI_MODE_DESC> m_displayModeDescs;
                                
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
    ID3D11RasterizerState                   *m_rastState;
    std::map<BlendMode, ID3D11BlendState *>  m_blendStateMap;
    DepthBuffer                              m_depthBuffer;
    Camera                                   m_camera;

    ///////////////
    // Resources //
    ///////////////
    std::vector<VertexShader>                m_vertexShaderRes;
    std::vector<ID3D11PixelShader*>          m_pixelShaderRes;
    std::vector<Texture>                     m_textureRes;
    std::vector<ID3D11Buffer*>               m_vertexBufferRes;
    std::vector<ID3D11Buffer*>               m_indexBufferRes;
    std::vector<ID3D11Buffer*>               m_constBufferRes;
    std::vector<RenderTarget>                m_renderTargetRes;

    /////////////
    // Batcher //
    /////////////
    
    std::unique_ptr<DirectX::SpriteBatch> m_batcher;


    ///////////
    //cleanup//
    ///////////

    std::vector<DirectSheep::Handle>         m_handles;
#endif
};

} //namespace Graphics

