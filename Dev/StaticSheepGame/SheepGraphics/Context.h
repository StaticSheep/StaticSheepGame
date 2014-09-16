#pragma once

namespace DirectSheep
{
  class Handle;
} // namespace Graphics

#include "DataTypes.h"
//#include "Window.h"

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

    //Returns true if the RenderContext is Initialized, else false
    bool IsInitialized(void) const;
    //Initializes the RenderContext
    //Returns true if successful, else false
    bool Initialize(HWND hwnd, float height, float width);
    //Uninitializes the RenderContext
    void Uninitialize(void);

    /////////////////////////////////////////////////////////////
    //                     DRAW FUNCTIONS                      //
    /////////////////////////////////////////////////////////////

    void Draw(unsigned vertexCount, unsigned vertexStart = 0);
    void DrawIndexed(unsigned indexCount, unsigned indexStart = 0, unsigned vertexStart = 0);
    void DrawInstanced(unsigned vertexCount, unsigned instanceCount, unsigned vertexStart = 0, unsigned instanceStart = 0);
    void DrawIndexInstanced(unsigned indexCountPerInstance, unsigned instanceCount, unsigned indexStart = 0, unsigned vertexStart = 0, unsigned instanceStart = 0);
    void Present(void);

    /////////////////////////////////////////////////////////////
    //                    CREATE FUNCTIONS                     //
    /////////////////////////////////////////////////////////////
    
    bool CreateVertexShader(Handle& handle, const std::string& filename, const InputLayout& inputLayout, const std::string& entryFunc = "VShader");
    bool CreatePixelShader(Handle& handle, const std::string& filename, const std::string& entryFunc = "PShader");
    bool CreateTexture(Handle& handle, const std::string& filename);
    bool CreateTexture(Handle& handle, const void *data, const Dimension& dim, const Format format);
    bool CreateVertexBuffer(Handle& handle, size_t size);
    bool CreateIndexBuffer(Handle& handle, size_t size);
    bool CreateConstantBuffer(Handle& handle, size_t size);
    bool CreateRenderTarget(Handle& handle, const RenderTargetMode mode, const Format format, const float downsamplePercentage = 1.0f, const Dimension& dim = Dimension());

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

    /////////////////////////////////////////////////////////////
    //                    SETTER FUNCTIONS                     //
    /////////////////////////////////////////////////////////////

    void SetClearColor(const float r, const float g, const float b, const float a);
    void SetClearColor(const Color& color);
    void SetTargetWindow(const HWND& hwnd);
    void SetFullscreen(const bool fullscreen);
    void SetPrimitiveTopology(const PrimitiveTopology primitiveTopology);
    void SetBlendMode(const BlendMode blendMode);
    void SetViewport(int xOffset, int yOffset, Dimension dim);
    void SetViewport(const Viewport& viewport);
    void SetDisplayMode(unsigned modeIndex);
    void SetDisplayMode(const Dimension& resolution);
    void SetVSync(bool vsync);

    /////////////////////////////////////////////////////////////
    //                    GETTER FUNCTIONS                     //
    /////////////////////////////////////////////////////////////

    Handle GetBackBuffer(void) const;
    bool GetFullscreen(void) const;
    ID3D11Device* GetDevice(void) const;
    ID3D11DeviceContext* GetDeviceContext(void) const;
    const Dimension& GetNativeResolution(void) const;
    const Viewport& GetViewport(void) const;
    DisplayMode GetCurrentDisplayMode(void) const;
    int GetCurrentDisplayModeIndex(void) const;
    const std::vector<DisplayMode>& GetDisplayModes(void) const;
    const std::string& GetGraphicsCardInfo(void) const;
    const Dimension& GetTextureSize(const Handle& texHandle) const;

    /////////////////////////////////////////////////////////////
    //                    UTILITY FUNCTIONS                    //
    /////////////////////////////////////////////////////////////

    void CopyData(const Handle& handle, const void *data, size_t size = 0);
    void ClearRenderTarget(const Handle& handle, float r, float g, float b, float a);
    void ClearRenderTarget(const Handle& handle, Color clearColor);
    void ClearBackBuffer(void);

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

  private:

    //---------//
    // Structs //
    //---------//
    struct Texture
    {
      ID3D11ShaderResourceView *shaderResourceView;
      ID3D11Texture2D          *texture;
      Dimension                 size;
    };

    struct VertexShader
    {
      ID3D11VertexShader *vShader;
      ID3D11InputLayout  *inputLayout;
    };

    struct RenderTarget
    {
      ID3D11RenderTargetView   *renderTargetView;
      ID3D11ShaderResourceView *shaderResourceView;
      ID3D11Texture2D          *texture2D;
      Format                   format;

      float            downsamplePercentage;
      RenderTargetMode mode;
      Dimension        size;
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

    /////////////
    // DirectX //
    /////////////
    IDXGISwapChain              *m_swapChain;
    ID3D11Device                *m_device;   
    ID3D11DeviceContext         *m_deviceContext;
    IDXGIFactory                *m_factory;
    IDXGIAdapter                *m_adapter;
    IDXGIOutput                 *m_output;
                                
    int                         m_displayModeIndex;
    std::vector<DisplayMode>    m_displayModes;
    std::vector<DXGI_MODE_DESC> m_displayModeDescs;
                                
    ID3D11RenderTargetView      *m_backBuffer;
    ID3D11DepthStencilView      *m_depthBuffer;
    Dimension                    m_backBufferSize;
    Viewport                     m_viewport;
    Color                        m_clearColor;
    PrimitiveTopology            m_primative;

    /////////////////////////////////
    // Other render configurations //
    /////////////////////////////////
    ID3D11SamplerState                      *m_sampleStates[2];
    ID3D11RasterizerState                   *m_rastState;
    std::map<BlendMode, ID3D11BlendState *>  m_blendStateMap;

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
};

} //namespace Graphics
