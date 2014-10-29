#include "precompiled.h"
#include "Context.h"
#include "Handle.h"

#include "WICTextureLoader.h"
#include <direct.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

namespace DirectSheep
{

  RenderContext * RenderContext::Allocate(void)
  {
    RenderContext* rc = (RenderContext*)_aligned_malloc(sizeof(RenderContext), 16);
    new (rc) RenderContext();
    return rc;
  }

  void RenderContext::UpdateCamera(float x, float y, float fov)
  {
    float cameraX = x;
    float cameraY = y;
    float cameraZ;

    if(m_camera.used)
      cameraZ = -400.0f;
    else
      cameraZ = -10.0f;

    Vec4 eyepoint(cameraX, cameraY, cameraZ, 0);

    Vec4 lootAtPoint(cameraX, cameraY, 0.0f, 1.0f);

    Vec4 upVector(0.0f, 1.0f, 0.0f, 0);

    iMat4 matView = XMMatrixLookAtLH(ToVector(&eyepoint), ToVector(&lootAtPoint), ToVector(&upVector));
    m_camera.view = matView;

    iMat4 matProj;
    if(m_camera.used)
      matProj = XMMatrixPerspectiveFovLH((FLOAT)XMConvertToRadians(fov), (float)m_viewport.dim.width / (float)m_viewport.dim.height, 1.0f, 1000.0f);
    else
      matProj = XMMatrixOrthographicLH((float)m_viewport.dim.width, (float)m_viewport.dim.height, 1.0f, 1000.0f);
    m_camera.proj = matProj;

    m_camera.viewProj = matView * matProj;
  }

  RenderContext::RenderContext(void) :
    m_initialized(false),
    m_hwnd(NULL),
    m_fullscreen(false),
    m_vsync(false),
    m_swapChain(NULL),
    m_device(NULL),
    m_deviceContext(NULL),
    m_factory(NULL),
    m_adapter(NULL),
    m_output(NULL),
    m_displayModeIndex(0),
    m_backBuffer(NULL),
    m_clearColor(Color(Colors::Black)),
    m_spriteBlend(Vec4(1, 1, 1, 1)),
    m_primative(PRIMITIVE_TOPOLOGY_TRIANGLELIST),
    m_rastState(NULL)
  {
    m_sampleStates[0] = NULL;
    m_sampleStates[1] = NULL;
  }

  RenderContext::~RenderContext(void)
  {
  }

  //Returns true if the RenderContext is Initialized, else false
  bool RenderContext::IsInitialized(void) const
  {
    return m_initialized;
  }

  //Initializes the RenderContext
  //Returns true if successful, else false
  bool RenderContext::Initialize(HWND hwnd, float width, float height)
  {
    m_viewport.dim = Dimension((unsigned)width, (unsigned)height);
    m_hwnd = hwnd;
    m_camera.used = true;
    InitializeDeviceAndSwapChain();
    m_batcher = std::unique_ptr<DirectX::SpriteBatch>(new SpriteBatch(m_deviceContext));
    m_batcher->SetRotation(DXGI_MODE_ROTATION_UNSPECIFIED);
    CreateDepthBuffer();
    InitializeBackBuffer();
    
    SetViewport(0, 0, Dimension((unsigned)width, (unsigned)height));

    CreateFontWrapper();
    
    InitializeRasterizerState();
    InitializeSamplerState();
    InitializeBlendModes();
    InitializeDepthState();

    m_initialized = true;
    return true;
  }

  //Uninitializes the RenderContext
  void RenderContext::Uninitialize(RenderContext * rCon)
  {
    Handle toRelease(VERTEX_SHADER, 0);

    m_swapChain->SetFullscreenState(FALSE, NULL);

    for (size_t i = 0; i < m_handles.size(); ++i)
    {
      Release(m_handles[i]);
    }

    SafeRelease(m_device);

    SafeRelease(m_deviceContext);

    SafeRelease(m_swapChain);

    m_font.Release();

    SafeRelease(m_backBuffer);

    m_depthBuffer.Release();

    SafeRelease(m_rastState);

    for (int i = 0; i < (sizeof(m_sampleStates) / sizeof(ID3D11SamplerState*)); ++i)
    {
      SafeRelease(m_sampleStates[i]);
    }

    for(auto it : m_blendStateMap)
    {
      SafeRelease(it.second);
    }

    rCon->~RenderContext();
    _aligned_free(rCon);
  }

    /////////////////////////////////////////////////////////////
    //                    SETTER FUNCTIONS                     //
    /////////////////////////////////////////////////////////////

    void RenderContext::SetClearColor(const float r, const float g, const float b, const float a)
    {
      m_clearColor = Color(r, g, b, a);
    }

    void RenderContext::SetClearColor(const Color& color)
    {
      m_clearColor = color;
    }

    void RenderContext::SetTargetWindow(const HWND& hwnd)
    {
      m_hwnd = hwnd;
    }

    void RenderContext::SetFullscreen(const bool fullscreen)
    {
      m_fullscreen = fullscreen;
    }

    void RenderContext::SetPrimitiveTopology(const PrimitiveTopology primitiveTopology)
    {
      m_primative = primitiveTopology;
    }

    void RenderContext::SetBlendMode(const BlendMode blendMode)
    {
      m_deviceContext->OMSetBlendState(m_blendStateMap[blendMode],0, 0xffffffff);
    }

    void RenderContext::SetViewport(int xOffset, int yOffset, Dimension dim)
    {
        // Init viewport
      D3D11_VIEWPORT viewport;
      ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

      viewport.TopLeftX = (float)xOffset;
      viewport.TopLeftY = (float)yOffset;
      viewport.Width = (float)dim.width;
      viewport.Height = (float)dim.height;
      viewport.MinDepth = 0.0f;    // Closest plane accounted by depth buffer (0 - 1 scale)
      viewport.MaxDepth = 1.0f;    // Furthest plane accounted by depth buffer (0 - 1 scale)

      // Register the viewport with device
      m_deviceContext->RSSetViewports(1, &viewport);
    }

    void RenderContext::SetViewport(const Viewport& viewport)
    {
      D3D11_VIEWPORT tempVP;
      ZeroMemory(&tempVP, sizeof(D3D11_VIEWPORT));

      tempVP.TopLeftX = (float)viewport.offsetX;
      tempVP.TopLeftY = (float)viewport.offsetY;
      tempVP.Width = (float)viewport.dim.width;
      tempVP.Height = (float)viewport.dim.height;
      tempVP.MinDepth = 0.0f;    // Closest plane accounted by depth buffer (0 - 1 scale)
      tempVP.MaxDepth = 1.0f;    // Furthest plane accounted by depth buffer (0 - 1 scale)

      // Register the viewport with device
      m_deviceContext->RSSetViewports(1, &tempVP);
    }

    void RenderContext::SetVSync(bool vsync)
    {
      m_vsync = vsync;
    }

   void RenderContext::SetPosition(const float x, const float y)
   {
     m_spriteTrans.x = x;
     m_spriteTrans.y = y;
   }
   void RenderContext::SetRotation(const float theta)
   {
     m_spriteTrans.theta = theta;
   }
   void RenderContext::SetDimensions(const float w, const float h)
   {
     m_spriteTrans.w = w;
     m_spriteTrans.h = h;
   }
   void RenderContext::SetBlendCol(const float r, const float g, const float b, const float a)
   {
     m_spriteBlend = Vec4(r,g,b,a);
   }

   void RenderContext::SetUV(float x1, float y1, float x2, float y2)
   {
     m_spriteTrans.uvBegin = Vec2(x1, y1);
     m_spriteTrans.uvEnd = Vec2(x2, y2);
   }
    /////////////////////////////////////////////////////////////
    //                    GETTER FUNCTIONS                     //
    /////////////////////////////////////////////////////////////

    Handle RenderContext::GetBackBuffer(void) const
    {
      return Handle();
    }

    bool RenderContext::GetFullscreen(void) const
    {
      return m_fullscreen;
    }

    void* RenderContext::ExternalGetDevice(void) const
    {
      return m_device;
    }

    ID3D11Device* RenderContext::GetDevice(void) const
    {
      return m_device;
    }

    ID3D11DeviceContext* RenderContext::GetDeviceContext(void) const
    {
      return m_deviceContext;
    }

    const Dimension& RenderContext::GetNativeResolution(void) const
    {
      return m_nativeResolution;
    }

    const Viewport& RenderContext::GetViewport(void) const
    {
      return m_viewport;
    }

    DisplayMode RenderContext::GetCurrentDisplayMode(void) const
    {
      return m_displayModes[m_displayModeIndex];
    }

    int RenderContext::GetCurrentDisplayModeIndex(void) const
    {
      return m_displayModeIndex;
    }

    const std::vector<DisplayMode>& RenderContext::GetDisplayModes(void) const
    {
      return m_displayModes;
    }

    const std::string& RenderContext::GetGraphicsCardInfo(void) const
    {
      return m_graphicsCardInfo;
    }

    const Dimension& RenderContext::GetTextureSize(const Handle& texHandle) const
    {
      return m_textureRes[texHandle.index].size;
    }

    void RenderContext::SetUseCam(bool camUse)
    {
      m_camera.used = camUse;
    }
    /////////////////////////////////////////////////////////////
    //                    UTILITY FUNCTIONS                    //
    /////////////////////////////////////////////////////////////

    void RenderContext::ClearRenderTarget(const Handle& handle, float r, float g, float b, float a)
    {
      if(handle.type == RENDER_TARGET)
        m_deviceContext->ClearRenderTargetView(m_renderTargetRes[handle.index].renderTargetView, (float*)&Vec4(r,g,b,a));
    }

    void RenderContext::ClearRenderTarget(const Handle& handle, Color clearColor)
    {
      if(handle.type == RENDER_TARGET)
        m_deviceContext->ClearRenderTargetView(m_renderTargetRes[handle.index].renderTargetView, (float*)&Vec4(clearColor.r, clearColor.g, clearColor.b, clearColor.a));
    }

    void RenderContext::ClearBackBuffer(void)
    {
      m_deviceContext->ClearRenderTargetView(m_backBuffer, (float*)&Vec4(m_clearColor.r, m_clearColor.g, m_clearColor.b, 1.0f));
    }

    void RenderContext::ClearDepthBuffer(void)
    {
      m_deviceContext->ClearDepthStencilView(m_depthBuffer.m_depthBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    /////////////////////////////////////////////////////////////
    //                 PUBLIC RELEASE FUCNTION                 //
    /////////////////////////////////////////////////////////////

    void RenderContext::Release(const Handle &handle)
    {
      switch (handle.type)
      {
      case VERTEX_SHADER    : ReleaseVertexShaderIntern(handle);
        break;

      case PIXEL_SHADER     : ReleasePixelShaderIntern(handle);
        break;

      case TEXTURE          : ReleaseTextureIntern(handle);
        break;

      case VERTEX_BUFFER    : ReleaseVertexBufferIntern(handle);
        break;

      case INDEX_BUFFER     : ReleaseIndexBufferIntern(handle);
        break;

      case CONSTANT_BUFFER  : ReleaseConstantBufferIntern(handle);
        break;

      case RENDER_TARGET    : ReleaseRenderTargetIntern(handle);
        break;

      default:
        break;
      }
    }

  /////////////////////////////////////////////////////////////
  //               INTERNAL RELEASE FUNCTIONS                //
  /////////////////////////////////////////////////////////////

  void RenderContext::ReleaseTextureIntern(const Handle& texture)
  {
    if(texture.type == TEXTURE)
    {
      m_textureRes[texture.index].Release();
    }
  }

  void RenderContext::ReleaseVertexShaderIntern(const Handle& vertexShader)
  {
    if(vertexShader.type == VERTEX_SHADER)
    {
      m_vertexShaderRes[vertexShader.index].Release();
    }
  }

  void RenderContext::ReleasePixelShaderIntern(const Handle& pixelShader)
  {
    if(pixelShader.type == PIXEL_SHADER)
    {
      SafeRelease(m_pixelShaderRes[pixelShader.index]);
    }
  }

  void RenderContext::ReleaseVertexBufferIntern(const Handle& vertexBuffer)
  {
    if(vertexBuffer.type == VERTEX_BUFFER)
    {
      SafeRelease(m_vertexBufferRes[vertexBuffer.index]);
    }
  }

  void RenderContext::ReleaseIndexBufferIntern(const Handle& indexBuffer)
  {
    if(indexBuffer.type == INDEX_BUFFER)
    {
      SafeRelease(m_indexBufferRes[indexBuffer.index]);
    }
  }

  void RenderContext::ReleaseConstantBufferIntern(const Handle& constantBuffer)
  {
    if(constantBuffer.type == CONSTANT_BUFFER)
    {
      SafeRelease(m_constBufferRes[constantBuffer.index]);
    }
  }

  void RenderContext::ReleaseRenderTargetIntern(const Handle& renderTarget)
  {
    if(renderTarget.type == RENDER_TARGET)
    {
      m_renderTargetRes[renderTarget.index].Release();
    }
  }

}