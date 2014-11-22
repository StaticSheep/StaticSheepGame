/******************************************************************************
Filename: Context.cpp
Project:
Author(s): Scott Nelson

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "precompiled.h"
#include "Context.h"
#include "Handle.h"

#include "WICTextureLoader.h"
#include <direct.h>

#pragma comment (lib, "d3d11.lib")

using namespace DirectX;

namespace DirectSheep
{

  /*****************************************************************************/
  /*!
      \brief
      Allocates Graphics Engine
  */
  /*****************************************************************************/
  RenderContext * RenderContext::Allocate(void)
  {
    // Get an alligned malloc to keep directx matrices aligned
    RenderContext* rc = (RenderContext*)_aligned_malloc(sizeof(RenderContext), 16);
    new (rc) RenderContext();
    return rc;
  }

  /*****************************************************************************/
  /*!
      \brief
      Constructor for RenderContext
  */
  /*****************************************************************************/
  RenderContext::RenderContext(void) :
    m_initialized(false),                // Is not initialized yet
    m_hwnd(NULL),                        // Handle to game window
    m_fullscreen(false),                 // Don't start fullscreen
    m_vsync(false),                      // VSync off
    m_swapChain(NULL),                   // Null pointers to DirectX objects
    m_device(NULL),
    m_deviceContext(NULL),
    m_backBuffer(NULL),
    m_clearColor(Color(Colors::Black.operator const float *())), // Clear color for backbuffer
    m_spriteBlend(Vec4(1, 1, 1, 1)),                             // Start blending color as white
    m_primative(PRIMITIVE_TOPOLOGY_TRIANGLELIST)                 // Draw using triangle lists
  {
    m_sampleStates[0] = NULL;                                    // Null out graphics states
    m_sampleStates[1] = NULL;
    m_rastState[0] = NULL;
    m_rastState[1] = NULL;
  }

  /*****************************************************************************/
  /*!
      \brief
      Destructor for RenderContext
  */
  /*****************************************************************************/
  RenderContext::~RenderContext(void)
  {
  }

  /*****************************************************************************/
  /*!
      \brief
      Returns true if renderContext has been initialized
  */
  /*****************************************************************************/
  bool RenderContext::IsInitialized(void) const
  {
    return m_initialized;
  }

  /*****************************************************************************/
  /*!
      \brief
      Initializes RenderContext and DirectX device
  */
  /*****************************************************************************/
  bool RenderContext::Initialize(HWND hwnd, float width, float height)
  {
    // Start viewport at window dimensions
    m_viewport.dim = Dimension((unsigned)width, (unsigned)height);

    // Regiset window handle
    m_hwnd = hwnd;

    // Start with using perspective camera
    m_camUse = true;

    // Init DirectX
    InitializeDeviceAndSwapChain();

    // Initialize Sprite Batcher
    m_batcher = std::unique_ptr<DirectX::SpriteBatch>(new SpriteBatch(m_deviceContext));
    m_batcher->SetRotation(DXGI_MODE_ROTATION_UNSPECIFIED);

    // Initialize Depth Buffer for Z-sorting
    CreateDepthBuffer();

    // Initializes back buffer to draw on
    InitializeBackBuffer();
    
    // Set DirectX viewport
    SetViewport(0, 0, Dimension((unsigned)width, (unsigned)height));

    // Initializes SpriteFont
    CreateFontWrapper();
    
    // Initialize all DirectX states
    InitializeRasterizerState();
    InitializeSamplerState();
    InitializeBlendModes();
    InitializeDepthState();

    // Initialize Camera's
    m_Ortho = Handle(CAMERA, new Camera(1920, 1080, false));
    m_orthoScreen = Handle(CAMERA, new Camera(1920, 1080, false));
    m_Perspective = Handle(CAMERA, new Camera(1920, 1080, true));
    m_editor = Handle(CAMERA, new Camera(1920, 1080, true));
    m_camera = m_Perspective;

    // Initialize Effects
    m_genericEffect = new GenEffect(m_device);

    // RenderContext is now initialized
    m_initialized = true;
    return true;
  }

  /*****************************************************************************/
  /*!
      \brief
      Uninitialized RenderContext and clean up memory
  */
  /*****************************************************************************/
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

    for (int i = 0; i < RastStates::NumStates; ++i)
      SafeRelease(m_rastState[i]);

    for (int i = 0; i < (sizeof(m_sampleStates) / sizeof(ID3D11SamplerState*)); ++i)
      SafeRelease(m_sampleStates[i]);

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
    
  /*****************************************************************************/
  /*!
      \brief
      Sets clear color for backbuffer
  */
  /*****************************************************************************/
    void RenderContext::SetClearColor(const float r, const float g, const float b, const float a)
    {
      m_clearColor = Color(r, g, b, a);
    }

  /*****************************************************************************/
  /*!
      \brief
      Set's target window (currently not used as game is one window)
  */
  /*****************************************************************************/
    void RenderContext::SetTargetWindow(const HWND& hwnd)
    {
      m_hwnd = hwnd;
    }

  /*****************************************************************************/
  /*!
      \brief
      Toggles fullscreen (can also press alt + Enter)
  */
  /*****************************************************************************/
    void RenderContext::SetFullscreen(const bool fullscreen)
    {
      m_fullscreen = fullscreen;
      m_swapChain->SetFullscreenState(fullscreen, 0);
    }

  /*****************************************************************************/
  /*!
      \brief
      Update what type of draw primitive DirectX uses
  */
  /*****************************************************************************/
    void RenderContext::SetPrimitiveTopology(const PrimitiveTopology primitiveTopology)
    {
      m_primative = primitiveTopology;
    }

  /*****************************************************************************/
  /*!
      \brief
      Set Blend Mode I.E alpha, additive
  */
  /*****************************************************************************/
    void RenderContext::SetBlendMode(const BlendMode blendMode)
    {
      m_deviceContext->OMSetBlendState(m_blendStateMap[blendMode],0, 0xffffffff);
    }

  /*****************************************************************************/
  /*!
      \brief
      Set DirectX viewport
  */
  /*****************************************************************************/
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

  /*****************************************************************************/
  /*!
      \brief
      Set DirectX vieport using Viewport class
  */
  /*****************************************************************************/
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

  /*****************************************************************************/
  /*!
      \brief
      Toggles VSync
  */
  /*****************************************************************************/
    void RenderContext::SetVSync(bool vsync)
    {
      m_vsync = vsync;
    }


  /*****************************************************************************/
  /*!
      \brief
      Sets position for next Draw Call
  */
  /*****************************************************************************/
   void RenderContext::SetPosition(const float x, const float y, float z)
   {
     m_spriteTrans.x = x;
     m_spriteTrans.y = y;
     m_spriteTrans.z = z;
   }

  /*****************************************************************************/
  /*!
      \brief
      Sets orientation for next Draw Call
  */
  /*****************************************************************************/
   void RenderContext::SetRotation(const float theta)
   {
     m_spriteTrans.theta = theta;
   }

  /*****************************************************************************/
  /*!
      \brief
      Sets scale for next Draw Call
  */
  /*****************************************************************************/
   void RenderContext::SetDimensions(const float w, const float h)
   {
     m_spriteTrans.w = w;
     m_spriteTrans.h = h;
   }

  /*****************************************************************************/
  /*!
      \brief
      Sets blend color for next draw call
  */
  /*****************************************************************************/
   void RenderContext::SetBlendCol(const float r, const float g, const float b, const float a)
   {
     m_spriteBlend = Vec4(r,g,b,a);
   }

  /*****************************************************************************/
  /*!
      \brief
      Sets uv scope for next draw call (used in animator)
  */
  /*****************************************************************************/
   void RenderContext::SetUV(float x1, float y1, float x2, float y2)
   {
     m_spriteTrans.uvBegin = Vec2(x1, y1);
     m_spriteTrans.uvEnd = Vec2(x2, y2);
   }
    /////////////////////////////////////////////////////////////
    //                    GETTER FUNCTIONS                     //
    /////////////////////////////////////////////////////////////

  /*****************************************************************************/
  /*!
      \brief
      Getter for fullscreen state
  */
  /*****************************************************************************/
    bool RenderContext::GetFullscreen(void) const
    {
      return m_fullscreen;
    }

  /*****************************************************************************/
  /*!
      \brief
      Get DirectX device (used by AntTweak)
  */
  /*****************************************************************************/
    void* RenderContext::ExternalGetDevice(void) const
    {
      return m_device;
    }

  /*****************************************************************************/
  /*!
      \brief
      Get Device within engine
  */
  /*****************************************************************************/
    ID3D11Device* RenderContext::GetDevice(void) const
    {
      return m_device;
    }

  /*****************************************************************************/
  /*!
      \brief
      Get device Context within engine
  */
  /*****************************************************************************/
    ID3D11DeviceContext* RenderContext::GetDeviceContext(void) const
    {
      return m_deviceContext;
    }

  /*****************************************************************************/
  /*!
      \brief
      Get current viewport being used
  */
  /*****************************************************************************/
    const Viewport& RenderContext::GetViewport(void) const
    {
      return m_viewport;
    }

  /*****************************************************************************/
  /*!
      \brief
      Get dimensions of Texture by Handle
  */
  /*****************************************************************************/
    const Dimension RenderContext::GetTextureSize(const Handle& texHandle) const
    {
      return Dimension(m_textureRes[texHandle.index].getWidth(), m_textureRes[texHandle.index].getHeight());
    }
    /////////////////////////////////////////////////////////////
    //                    UTILITY FUNCTIONS                    //
    /////////////////////////////////////////////////////////////

  /*****************************************************************************/
  /*!
      \brief
      Clear back buffer for next frame
  */
  /*****************************************************************************/
    void RenderContext::ClearBackBuffer(void)
    {
      m_deviceContext->ClearRenderTargetView(m_backBuffer, (float*)&Vec4(m_clearColor.R(), m_clearColor.G(), m_clearColor.B(), 1.0f));
    }

  /*****************************************************************************/
  /*!
      \brief
      Clear depth buffer for next Frame
  */
  /*****************************************************************************/
    void RenderContext::ClearDepthBuffer(void)
    {
      m_deviceContext->ClearDepthStencilView(m_depthBuffer.m_depthBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    /////////////////////////////////////////////////////////////
    //                 PUBLIC RELEASE FUCNTION                 //
    /////////////////////////////////////////////////////////////

  /*****************************************************************************/
  /*!
      \brief
      Release handles of different types
  */
  /*****************************************************************************/
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
     SafeRelease(m_textureRes[texture.index].m_rawTex);
     SafeRelease(m_textureRes[texture.index].m_renderTarget);
     SafeRelease(m_textureRes[texture.index].m_ShaderRes);
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