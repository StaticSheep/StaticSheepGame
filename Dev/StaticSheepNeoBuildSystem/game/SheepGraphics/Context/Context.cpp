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
    float cameraZ;// = -400.0f;

    if(m_camera.used)
      cameraZ = 400.0f;
    else
      cameraZ = 10.0f;

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
    m_resolution(Dimension(0, 0)),
    m_nativeResolution(Dimension(0, 0)),
    m_fullscreen(false),
    m_vsync(false),
    m_swapChain(NULL),
    m_device(NULL),
    m_deviceContext(NULL),
    m_factory(NULL),
    m_adapter(NULL),
    m_output(NULL),
    m_sampleStates(),
    m_displayModeIndex(0),
    m_backBuffer(NULL),
    m_backBufferSize(Dimension(0,0)),
    m_clearColor(Color(Colors::Black)),
    m_spriteBlend(Vec4(1, 1, 1, 1)),
    m_primative(PRIMITIVE_TOPOLOGY_TRIANGLELIST),
    m_rastState(NULL){}

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

    if(m_device)
    {
      m_device->Release();
      m_device = NULL;
    }

    if(m_deviceContext)
    {
      m_deviceContext->Release();
      m_deviceContext = NULL;
    }

    if(m_swapChain)
    {
      m_swapChain->Release();
      m_swapChain = NULL;
    }

    if(m_font.m_fontWrapper)
    {
      m_font.m_fontWrapper->Release();
      m_font.m_fontWrapper = NULL;
    }

    if(m_backBuffer)
    {
      m_backBuffer->Release();
      m_backBuffer = NULL;
    }

    if(m_font.m_fontFactory)
    {
      m_font.m_fontFactory->Release();
      m_font.m_fontFactory = NULL;
    }

    if(m_depthBuffer.m_depthBuffer)
    {
      m_depthBuffer.m_depthBuffer->Release();
      m_depthBuffer.m_depthBuffer = NULL;
    }

    if(m_depthBuffer.m_depthState)
    {
      m_depthBuffer.m_depthState->Release();
      m_depthBuffer.m_depthState = NULL;
    }

    if(m_depthBuffer.texture2D)
    {
      m_depthBuffer.texture2D->Release();
      m_depthBuffer.texture2D = NULL;
    }

    if(m_rastState)
    {
      m_rastState->Release();
      m_rastState = NULL;
    }

    if(m_sampleStates[0])
    {
      m_sampleStates[0]->Release();
      m_sampleStates[0] = NULL;
    }

    if(m_sampleStates[1])
    {
      m_sampleStates[1]->Release();
      m_sampleStates[1] = NULL;
    }



    for(auto it : m_blendStateMap)
    {
      if(it.second)
      {
        it.second->Release();
        it.second = NULL;
      }
    }

    for(unsigned i = 0; i < m_vertexShaderRes.size(); ++i)
    {
      Release(toRelease);
      toRelease.index++;
    }

    toRelease.type = PIXEL_SHADER;
    toRelease.index = 0;
    for(unsigned i = 0; i < m_pixelShaderRes.size(); ++i)
    {
      Release(toRelease);
      toRelease.index++;
    }

    toRelease.type = TEXTURE;
    toRelease.index = 0;
    for(unsigned i = 0; i < m_textureRes.size(); ++i)
    {
      Release(toRelease);
      toRelease.index++;
    }

    toRelease.type = INDEX_BUFFER;
    toRelease.index = 0;
    for(unsigned i = 0; i < m_indexBufferRes.size(); ++i)
    {
      Release(toRelease);
      toRelease.index++;
    }

    toRelease.type = VERTEX_BUFFER;
    toRelease.index = 0;
    for(unsigned i = 0; i < m_vertexBufferRes.size(); ++i)
    {
      Release(toRelease);
      toRelease.index++;
    }

    toRelease.type = CONSTANT_BUFFER;
    toRelease.index = 0;
    for(unsigned i = 0; i < m_constBufferRes.size(); ++i)
    {
      Release(toRelease);
      toRelease.index++;
    }

    toRelease.type = RENDER_TARGET;
    toRelease.index = 0;
    for(unsigned i = 0; i < m_renderTargetRes.size(); ++i)
    {
      Release(toRelease);
      toRelease.index++;
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

      case  CONSTANT_BUFFER : ReleaseConstantBufferIntern(handle);
        break;

      case    RENDER_TARGET : ReleaseRenderTargetIntern(handle);
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
      if(m_textureRes[texture.index].shaderResourceView)
      {
        m_textureRes[texture.index].shaderResourceView->Release();
        m_textureRes[texture.index].shaderResourceView = NULL;
      }

      if(m_textureRes[texture.index].texture)
      {
        m_textureRes[texture.index].texture->Release();
        m_textureRes[texture.index].texture = NULL;
      }
    }
  }

  void RenderContext::ReleaseVertexShaderIntern(const Handle& vertexShader)
  {
    if(vertexShader.type == VERTEX_SHADER)
    {
      if(m_vertexShaderRes[vertexShader.index].vShader)
      {
        m_vertexShaderRes[vertexShader.index].vShader->Release();
        m_vertexShaderRes[vertexShader.index].vShader = NULL;
      }

      if(m_vertexShaderRes[vertexShader.index].inputLayout)
      {
        m_vertexShaderRes[vertexShader.index].inputLayout->Release();
        m_vertexShaderRes[vertexShader.index].inputLayout = NULL;
      }
    }
  }

  void RenderContext::ReleasePixelShaderIntern(const Handle& pixelShader)
  {
    if(pixelShader.type == PIXEL_SHADER)
    {
      if(m_pixelShaderRes[pixelShader.index])
      {
        m_pixelShaderRes[pixelShader.index]->Release();
        m_pixelShaderRes[pixelShader.index] = NULL;
      }
    }
  }

  void RenderContext::ReleaseVertexBufferIntern(const Handle& vertexBuffer)
  {
    if(vertexBuffer.type == VERTEX_BUFFER)
    {
      if(m_vertexBufferRes[vertexBuffer.index])
      {
        m_vertexBufferRes[vertexBuffer.index]->Release();
        m_vertexBufferRes[vertexBuffer.index] = NULL;
      }
    }
  }

  void RenderContext::ReleaseIndexBufferIntern(const Handle& indexBuffer)
  {
    if(indexBuffer.type == INDEX_BUFFER)
    {
      if(m_indexBufferRes[indexBuffer.index])
      {
        m_indexBufferRes[indexBuffer.index]->Release();
        m_indexBufferRes[indexBuffer.index] = NULL;
      }
    }
  }

  void RenderContext::ReleaseConstantBufferIntern(const Handle& constantBuffer)
  {
    if(constantBuffer.type == CONSTANT_BUFFER)
    {
      if(m_constBufferRes[constantBuffer.index])
      {
        m_constBufferRes[constantBuffer.index]->Release();
        m_constBufferRes[constantBuffer.index] = NULL;
      }
    }
  }

  void RenderContext::ReleaseRenderTargetIntern(const Handle& renderTarget)
  {
    if(renderTarget.type == RENDER_TARGET)
    {
      if(m_renderTargetRes[renderTarget.index].renderTargetView)
      {
        m_renderTargetRes[renderTarget.index].renderTargetView->Release();
        m_renderTargetRes[renderTarget.index].renderTargetView = NULL;
      }

      if(m_renderTargetRes[renderTarget.index].shaderResourceView)
      {
        m_renderTargetRes[renderTarget.index].shaderResourceView->Release();
        m_renderTargetRes[renderTarget.index].shaderResourceView = NULL;
      }

      if(m_renderTargetRes[renderTarget.index].texture2D)
      {
        m_renderTargetRes[renderTarget.index].texture2D->Release();
        m_renderTargetRes[renderTarget.index].texture2D = NULL;
      }
    }
  }

}