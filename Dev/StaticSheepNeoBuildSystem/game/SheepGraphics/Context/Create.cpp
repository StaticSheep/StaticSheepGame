/*****************************************************************
Filename: Create.cpp
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "precompiled.h"
#include "Context.h"
#include "Handle.h"

#include "WICTextureLoader.h"
#include <direct.h>
using namespace DirectX;
namespace DirectSheep
{
  /////////////////////////////////////////////////////////////
  //                    CREATE FUNCTIONS                     //
  /////////////////////////////////////////////////////////////

  /*!
      \brief
      Instantiate a new tex2D and bind it to a handle
  */
  bool RenderContext::CreateTexture(Handle& handle, const std::string& filename)
  {
    // Construct new tex2D
    Tex2D temp(m_device, m_contentPath + filename);

    // Push back to texture pool for access
    m_textureRes.push_back(temp);

    // Create handle
    handle.type = TEXTURE;
    handle.index = m_textureRes.size() - 1;

    // Insert handle into handle pool for cleanup
    m_handles.push_back(handle);
    return true;
  }

  /*!
      \brief
      Generates a vertex buffer
  */
  bool RenderContext::CreateVertexBuffer(Handle& handle, size_t size)
  {
    ID3D11Buffer *tempBuffer;   // Temporary for instantiation
    D3D11_BUFFER_DESC bd;       // Description to fill out the little things
    ZeroMemory(&bd, sizeof(bd));// Set everything to 0 or OFF

    bd.Usage = D3D11_USAGE_DYNAMIC;                 // write access by CPU and GPU
    bd.ByteWidth = size;                            // size is the VERTEX struct
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;        // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     // allow CPU to write in buffer

    DXVerify(m_device->CreateBuffer(&bd, NULL, &tempBuffer));       // create the buffer

    // Yes we currently are hardcoded to the unit square. If you are reading this Zach, I'm sorry.
    //                                                    If you are reading this Zak, you're welcome.
    Vertex2D QuadVertices[] =
    {
      { Vec3(-0.5f, -0.5f, 0.0f),0.0f, 1.0f },
      { Vec3(-0.5f, 0.5f, 0.0f), 0.0f, 0.0f },
      { Vec3(0.5f, -0.5f, 0.0f), 1.0f, 1.0f },

      { Vec3(-0.5f, 0.5f, 0.0f), 0.0f, 0.0f },
      { Vec3(0.5f, 0.5f, 0.0f),  1.0f, 0.0f },
      { Vec3(0.5f, -0.5f, 0.0f), 1.0f, 1.0f },
    };

    D3D11_MAPPED_SUBRESOURCE ms;
    m_deviceContext->Map(tempBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);         // map the buffer
    memcpy(ms.pData, QuadVertices, sizeof(QuadVertices));                               // copy the data
    m_deviceContext->Unmap(tempBuffer, NULL);
    m_vertexBufferRes.push_back(tempBuffer);

    handle.type = VERTEX_BUFFER;
    handle.index = m_vertexBufferRes.size() - 1;

    // Put handle in handle pool for cleanup
    m_handles.push_back(handle);
    return true;
  }

  /*!
      \brief
      Create an index buffer
  */
  bool RenderContext::CreateIndexBuffer(Handle& handle, size_t size)
  {
    D3D11_BUFFER_DESC bd;    // A description to be filled with the little things
    ID3D11Buffer * tempIB;   // Temporary to create the buffer
    ZeroMemory(&bd, sizeof(bd));

    // Fill out description
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = size;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;

    // Create the buffer
    DXVerify(m_device->CreateBuffer(&bd, NULL, &tempIB));

    m_indexBufferRes.push_back(tempIB);

    handle.type = INDEX_BUFFER;
    handle.index = m_indexBufferRes.size() - 1;

    // Push handle into handle pool for cleanup
    m_handles.push_back(handle);

    return true;
  }

  /*!
      \brief
      Create a generic constant buffer
  */
  bool RenderContext::CreateConstantBuffer(Handle& handle, size_t size)
  {
    // Temporary buffer pointer
    ID3D11Buffer* tempCB;

    // Describes the buffer on creation
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    // Fill out description
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = size;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    // Create buffer
    DXVerify(m_device->CreateBuffer(&bd, NULL, &tempCB));

    m_constBufferRes.push_back(tempCB);
    handle.type = CONSTANT_BUFFER;
    handle.index = m_constBufferRes.size() - 1;

    // Push back handle to handle pool for cleanup
    m_handles.push_back(handle);
    return true;
  }

  /*!
      \brief
      Creates the engines depth buffer
  */
  bool RenderContext::CreateDepthBuffer(void)
  {
    D3D11_TEXTURE2D_DESC texd;       // Description structure for depth buffer texture
    ZeroMemory(&texd, sizeof(texd)); // Null all members

    texd.Width = m_viewport.dim.width; // Set screen dimensions
    texd.Height = m_viewport.dim.height;
    texd.ArraySize = 1;                      // Only one depth buffer
    texd.MipLevels = 1;                      // Mip Mapping
    texd.SampleDesc.Count = 4;
    texd.Format = DXGI_FORMAT_D32_FLOAT;
    texd.BindFlags = D3D11_BIND_DEPTH_STENCIL; // This is a depth stencil

    // Generate Depth buffers texture (functions as buffer to draw to)
    DXVerify(m_device->CreateTexture2D(&texd, NULL, &m_depthBuffer.texture2D));

    // create a depth buffer (z-sorting)
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));   // Zero members

    dsvd.Format = DXGI_FORMAT_D32_FLOAT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

    // Create the depth buffer
    DXVerify(m_device->CreateDepthStencilView(m_depthBuffer.texture2D, &dsvd, &m_depthBuffer.m_depthBuffer));

    return true;
  }

  /*!
      \brief
      Creates the engines font wrapper
  */
  bool RenderContext::CreateFontWrapper(void)
  {
    // Create the factory
    DXVerify(FW1CreateFactory(FW1_VERSION, &m_font.m_fontFactory));

    // Describes how the wrapper will be created
    FW1_FONTWRAPPERCREATEPARAMS Params;
    ZeroMemory(&Params, sizeof(Params));

    Params.SheetMipLevels = 5;                                         // Mip map up to 5
    Params.AnisotropicFiltering = TRUE;                                // Anisotropic
    Params.DefaultFontParams.pszFontFamily = L"Arial";                 // Default to Arial if requested font missing
    Params.DefaultFontParams.FontWeight = DWRITE_FONT_WEIGHT_NORMAL;   // Thickness of font
    Params.DefaultFontParams.FontStyle = DWRITE_FONT_STYLE_NORMAL;     // Italic vs Normal
    Params.DefaultFontParams.FontStretch = DWRITE_FONT_STRETCH_NORMAL; // Distortion

    // Use the factory to create the wrapper
    DXVerify(m_font.m_fontFactory->CreateFontWrapper(m_device, NULL, &Params, &m_font.m_fontWrapper));

    // Factory no longer needed
    m_font.m_fontFactory->Release();
    m_font.m_fontFactory = NULL;


    return true;
  }

  /*!
      \brief
      Creates the DirectX device (very important)
  */
  void RenderContext::InitializeDeviceAndSwapChain(void)
  {
    HRESULT hr = S_OK; // Error check, remains S_OK if no problems

    // Fallback if device can't find drivers attempt to init without
    D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_NULL;

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0; // Using DirectX 11

    UINT deviceFlags = 0; // Flags for registering device

#if defined (_DEBUG)
    //deviceFlags |= D3D11_CREATE_DEVICE_DEBUG; // If in debug mode set DirectX to debug mode
#endif

    // Array of driver types in order of most prefered to least
    D3D_DRIVER_TYPE driverTypes[] =
    {
      D3D_DRIVER_TYPE_HARDWARE, // Hardware acceleration
      D3D_DRIVER_TYPE_WARP,     // High performance Software Renderer
      D3D_DRIVER_TYPE_REFERENCE,// Slow but accurate Software Renderer
    };

    // Number of Driver options
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    // Feature set to Init Device with
    D3D_FEATURE_LEVEL featureLevels[] =
    {
      D3D_FEATURE_LEVEL_11_0, // DirectX 11
      D3D_FEATURE_LEVEL_10_1, // '' '' 10.1
      D3D_FEATURE_LEVEL_10_0, // '' '' 10
    };

    // Number of fearure options
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    // struct to hold information about the swapchain
    DXGI_SWAP_CHAIN_DESC swapDesc;

    // zero out all members
    ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    // Set arguments for swapchain creation
    swapDesc.BufferCount = 1;                                   // single back buffer
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // 32-bit color
    swapDesc.BufferDesc.Width = m_viewport.dim.width;           // back buffer width
    swapDesc.BufferDesc.Height = m_viewport.dim.height;         // back buffer height
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // use buffer as render target
    swapDesc.OutputWindow = m_hwnd;                             // attach to window
    swapDesc.SampleDesc.Count = 4;                              // # of multisamples
    swapDesc.Windowed = !m_fullscreen;                          // windowed/full-screen mode
    swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

    // create DirectX device, it's context, and swapchain using swapDesc

    // Loops through and attempts to init with highest driver settings
    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
      driverType = driverTypes[driverTypeIndex]; // Grabs driver type

      // Attempts to init
      hr = D3D11CreateDeviceAndSwapChain(NULL,      // No adapter
        driverType,       // Current driver setting attempt
        NULL,             // Don't want to use software
        deviceFlags,      // Special flags(debug)
        featureLevels,    // Pointer to feature levels
        numFeatureLevels, // Size of feature level array
        D3D11_SDK_VERSION,// Use DirectX 11 SDK
        &swapDesc,        // Struct with all params for device creatiosn
        &m_swapChain,     // Set swapchain pointer
        &m_device,        // Set device pointer
        &featureLevel,    // Give array of fearure levels DX will use best option
        &m_deviceContext);// Set devic context pointer

      if (SUCCEEDED(hr)) // If succeeded then break otherwise try lower driver settings
        break;
    }
    DXVerify(hr); // Check for any DirectX specific error messages

  }

  /*!
      \brief
      Creates a rasterizer state
  */
  void RenderContext::InitializeRasterizerState(void)
  {
    // Describes the rasterizer state
    D3D11_RASTERIZER_DESC rd;
    rd.FillMode = D3D11_FILL_SOLID;    // Solid vs Wireframe
    rd.CullMode = D3D11_CULL_BACK;     // Cull the backside of mesh
    rd.FrontCounterClockwise = FALSE;  // How front of mesh is determined by vertex order
    rd.DepthClipEnable = TRUE;         // We are using depth clipping (depth buffer)
    rd.ScissorEnable = FALSE;          // No scissoring
    rd.AntialiasedLineEnable = FALSE;  // No need for antialiasing yet
    rd.MultisampleEnable = TRUE;       // Definately want multisampling
    rd.DepthBias = 0;                  // No depth bias
    rd.DepthBiasClamp = 0.0f;          // "" "" ""
    rd.SlopeScaledDepthBias = 0.0f;    // "" "" ""

    // Create rasterizer state for fill
    DXVerify(m_device->CreateRasterizerState(&rd, &m_rastState[RastStates::Fill]));

    rd.FillMode = D3D11_FILL_WIREFRAME; // Make another raster state for wireframe

    DXVerify(m_device->CreateRasterizerState(&rd, &m_rastState[RastStates::Wire]));
  }

  /*!
      \brief
      Creates a backbuffer to draw to
  */
  void RenderContext::InitializeBackBuffer(void)
  {
    // find the address of the backbuffer
    ID3D11Texture2D *pBackBuffer;
    DXVerify(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));

    // create render target using backbuffer address
    DXVerify(m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_backBuffer));
    pBackBuffer->Release();

    m_deviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthBuffer.m_depthBuffer);
  }

  /*!
      \brief
      Create blend states
  */
  void RenderContext::InitializeBlendModes(void)
  {
    // This is a blend state for multiplicative alpha
    D3D11_BLEND_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
    bd.RenderTarget[0].BlendEnable = TRUE;
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    bd.IndependentBlendEnable = FALSE;
    bd.AlphaToCoverageEnable = TRUE;

    DXVerify(m_device->CreateBlendState(&bd, &m_blendStateMap[BLEND_MODE_ALPHA]));
  }

  /*!
      \brief
      Create a sampler state
  */
  void RenderContext::InitializeSamplerState(void)
  {
    // This affects how textures are sampled onto meshes
    D3D11_SAMPLER_DESC sd;
    sd.Filter = D3D11_FILTER_ANISOTROPIC;
    sd.MaxAnisotropy = 16;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP; // Clamp U
    sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP; // Clamp V
    sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP; // Clamp W (3D texture)
    sd.BorderColor[0] = 0.0f;
    sd.BorderColor[1] = 0.0f;
    sd.BorderColor[2] = 0.0f;
    sd.BorderColor[3] = 0.0f;
    sd.MinLOD = 0.0f;
    sd.MaxLOD = FLT_MAX;
    sd.MipLODBias = 0.0f;

    // Create the sampler state
    DXVerify(m_device->CreateSamplerState(&sd, &m_sampleStates[0]));
  }

  /*!
      \brief
      Create a depth state
  */
  void RenderContext::InitializeDepthState(void)
  {
    // This gives more control over depth sorting
    D3D11_DEPTH_STENCIL_DESC dsDesc;

    ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    // Paramaters for Depth test
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    // Paramaters for Stencil test
    dsDesc.StencilEnable = true;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask = 0xFF;

    // If Pixel is front facing
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // If Pixel is back facing
    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create depth stencil state
    DXVerify(m_device->CreateDepthStencilState(&dsDesc, &m_depthBuffer.m_depthState));

    // Bind state to device
  }
}