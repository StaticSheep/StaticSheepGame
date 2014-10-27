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

  bool RenderContext::CreateVertexShader(Handle& handle, const std::string& filename, const InputLayout& inputLayout, const std::string& entryFunc)
  {
    ID3D10Blob *VS, *Errors; // Raw data type used to hold buffers
    VertexShader tempVS;
    HRESULT result;

    char cDirectory[512]; // buffer

    _getcwd(cDirectory, sizeof(cDirectory));
    std::string lDirectory(cDirectory);
    lDirectory += "\\content\\shaders\\";
    lDirectory += filename.c_str();

    std::wstring finalDir;
    finalDir.assign(lDirectory.begin(), lDirectory.end());


    result = D3DCompileFromFile((LPCWSTR)finalDir.c_str(),
      0,
      0,
      entryFunc.c_str(),
      "vs_4_0",
      D3D10_SHADER_OPTIMIZATION_LEVEL3 | D3D10_SHADER_WARNINGS_ARE_ERRORS, // fully optimized shader
      0,
      &VS,
      &Errors);

    ErrorIf(result, "ShaderCompile", "Vertex Shader Failed To Compile"); // if there are any errors...

    DXVerify(m_device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &tempVS.vShader));

    std::vector<D3D11_INPUT_ELEMENT_DESC> ied;

    for (unsigned i = 0; i < inputLayout.size(); ++i)
    {
      D3D11_INPUT_ELEMENT_DESC temp;
      temp.SemanticName = inputLayout[i].semantic.c_str();
      temp.SemanticIndex = 0;
      temp.Format = (DXGI_FORMAT)inputLayout[i].format;
      temp.InputSlot = 0;
      temp.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
      temp.InputSlotClass = (D3D11_INPUT_CLASSIFICATION)inputLayout[i].instance;
      temp.InstanceDataStepRate = 0;
      ied.push_back(temp);
    }

    m_device->CreateInputLayout(ied.data(), ied.size(), VS->GetBufferPointer(), VS->GetBufferSize(), &tempVS.inputLayout);


    m_deviceContext->IASetInputLayout(tempVS.inputLayout);

    m_vertexShaderRes.push_back(tempVS);
    handle.type = VERTEX_SHADER;
    handle.index = m_vertexShaderRes.size() - 1;

    return true;
  }

  bool RenderContext::CreatePixelShader(Handle& handle, const std::string& filename, const std::string& entryFunc)
  {
    ID3D10Blob *PS, *Errors;
    ID3D11PixelShader *tempPS;
    HRESULT result;

    char cDirectory[512]; // buffer

    _getcwd(cDirectory, sizeof(cDirectory));
    std::string lDirectory(cDirectory);
    lDirectory += "\\content\\shaders\\";
    lDirectory += filename.c_str();

    std::wstring finalDir;
    finalDir.assign(lDirectory.begin(), lDirectory.end());

    result = D3DCompileFromFile((LPCWSTR)finalDir.c_str(),
      0,
      0,
      entryFunc.c_str(),
      "ps_4_0",
      D3D10_SHADER_OPTIMIZATION_LEVEL3 | D3D10_SHADER_WARNINGS_ARE_ERRORS, // fully optimized shader
      0,
      &PS,
      &Errors);

    ErrorIf(result, "ShaderCompile", "%s", Errors->GetBufferPointer()); // if there are any errors...

    DXVerify(m_device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &tempPS));

    m_pixelShaderRes.push_back(tempPS);
    handle.type = PIXEL_SHADER;
    handle.index = m_pixelShaderRes.size() - 1;

    return true;
  }

  bool RenderContext::CreateTexture(Handle& handle, const std::string& filename)
  {
    Texture tempTex = { 0 };
    D3D11_TEXTURE2D_DESC desc;
    static bool attempt = false;

    std::wstring test(filename.begin(), filename.end());


    //  (ID3D11Resource **)&tempTex.texture, &tempTex.shaderResourceView, NULL);

    HRESULT hr = DirectX::CreateWICTextureFromFile(m_device, test.c_str(), (ID3D11Resource **)&tempTex.texture, &tempTex.shaderResourceView, 0);

    if (FAILED(hr))
    {
      if (attempt)
        DXVerify(hr);
      attempt = true;
      return CreateTexture(handle, "content/Default.png");
    }
    attempt = false;
    tempTex.shaderResourceView->GetResource((ID3D11Resource **)&tempTex.texture);
    tempTex.texture->GetDesc(&desc);

    tempTex.size = Dimension(desc.Width, desc.Height);

    m_textureRes.push_back(tempTex);
    handle.type = TEXTURE;
    handle.index = m_textureRes.size() - 1;

    return true;
  }

  bool RenderContext::CreateVertexBuffer(Handle& handle, size_t size)
  {
    ID3D11Buffer *tempBuffer;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                 // write access by CPU and GPU
    bd.ByteWidth = size;                            // size is the VERTEX struct
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;        // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     // allow CPU to write in buffer

    DXVerify(m_device->CreateBuffer(&bd, NULL, &tempBuffer));       // create the buffer

    Vertex2D QuadVertices[] =
    {
      { Vec3(-0.5f, -0.5f, 0.0f), 0.0f, 1.0f },
      { Vec3(-0.5f, 0.5f, 0.0f), 0.0f, 0.0f },
      { Vec3(0.5f, -0.5f, 0.0f), 1.0f, 1.0f },

      { Vec3(-0.5f, 0.5f, 0.0f), 0.0f, 0.0f },
      { Vec3(0.5f, 0.5f, 0.0f), 1.0f, 0.0f },
      { Vec3(0.5f, -0.5f, 0.0f), 1.0f, 1.0f },
    };
    D3D11_MAPPED_SUBRESOURCE ms;
    m_deviceContext->Map(tempBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, QuadVertices, sizeof(QuadVertices));                               // copy the data
    m_deviceContext->Unmap(tempBuffer, NULL);
    m_vertexBufferRes.push_back(tempBuffer);

    handle.type = VERTEX_BUFFER;
    handle.index = m_vertexBufferRes.size() - 1;
    return true;
  }

  bool RenderContext::CreateIndexBuffer(Handle& handle, size_t size)
  {
    D3D11_BUFFER_DESC bd;
    ID3D11Buffer * tempIB;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = size;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;

    DXVerify(m_device->CreateBuffer(&bd, NULL, &tempIB));

    m_indexBufferRes.push_back(tempIB);

    handle.type = INDEX_BUFFER;
    handle.index = m_indexBufferRes.size() - 1;

    return true;
  }

  bool RenderContext::CreateConstantBuffer(Handle& handle, size_t size)
  {
    ID3D11Buffer* tempCB;

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = size;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    DXVerify(m_device->CreateBuffer(&bd, NULL, &tempCB));

    m_constBufferRes.push_back(tempCB);
    handle.type = CONSTANT_BUFFER;
    handle.index = m_constBufferRes.size() - 1;

    return true;
  }

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

    DXVerify(m_device->CreateTexture2D(&texd, NULL, &m_depthBuffer.texture2D));

    // create a depth buffer (z-sorting)
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));   // Zero members

    dsvd.Format = DXGI_FORMAT_D32_FLOAT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

    DXVerify(m_device->CreateDepthStencilView(m_depthBuffer.texture2D, &dsvd, &m_depthBuffer.m_depthBuffer));

    return true;
  }

  bool RenderContext::CreateFontWrapper(void)
  {
    DXVerify(FW1CreateFactory(FW1_VERSION, &m_font.m_fontFactory));

    FW1_FONTWRAPPERCREATEPARAMS Params;
    ZeroMemory(&Params, sizeof(Params));

    Params.SheetMipLevels = 5;
    Params.AnisotropicFiltering = TRUE;
    Params.DefaultFontParams.pszFontFamily = L"Arial";
    Params.DefaultFontParams.FontWeight = DWRITE_FONT_WEIGHT_NORMAL;
    Params.DefaultFontParams.FontStyle = DWRITE_FONT_STYLE_NORMAL;
    Params.DefaultFontParams.FontStretch = DWRITE_FONT_STRETCH_NORMAL;

    DXVerify(m_font.m_fontFactory->CreateFontWrapper(m_device, NULL, &Params, &m_font.m_fontWrapper));

    m_font.m_fontFactory->Release();
    m_font.m_fontFactory = NULL;


    return true;
  }

  /////////////////////////////////////////////////////////////
  //              PRIVATE INITIALIZE FUNCTIONS               //
  /////////////////////////////////////////////////////////////

  void RenderContext::InitializeDXGI(void)
  {

  }

  void RenderContext::InitializeDisplayModes(void)
  {
  }

  void RenderContext::InitializeDeviceAndSwapChain(void)
  {
    HRESULT hr = S_OK; // Error check, remains S_OK if no problems

    // Fallback if device can't find drivers attempt to init without
    D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_NULL;

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0; // Using DirectX 11

    UINT deviceFlags = 0; // Flags for registering device

#if defined (_DEBUG)
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG; // If in debug mode set DirectX to debug mode
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
    swapDesc.Windowed = true;                           // windowed/full-screen mode
    swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching
    //swapDesc.BufferDesc.RefreshRate.Numerator = 60;
    //swapDesc.BufferDesc.RefreshRate.Denominator	= 1;

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

  void RenderContext::InitializeRasterizerState(void)
  {
    D3D11_RASTERIZER_DESC rd;
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;
    rd.FrontCounterClockwise = FALSE;
    rd.DepthClipEnable = TRUE;
    rd.ScissorEnable = FALSE;
    rd.AntialiasedLineEnable = FALSE;
    rd.MultisampleEnable = FALSE;
    rd.DepthBias = 0;
    rd.DepthBiasClamp = 0.0f;
    rd.SlopeScaledDepthBias = 0.0f;

    DXVerify(m_device->CreateRasterizerState(&rd, &m_rastState));
  }
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

  void RenderContext::InitializeBlendModes(void)
  {
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

  void RenderContext::InitializeSamplerState(void)
  {
    D3D11_SAMPLER_DESC sd;
    sd.Filter = D3D11_FILTER_ANISOTROPIC;
    sd.MaxAnisotropy = 16;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.BorderColor[0] = 0.0f;
    sd.BorderColor[1] = 0.0f;
    sd.BorderColor[2] = 0.0f;
    sd.BorderColor[3] = 0.0f;
    sd.MinLOD = 0.0f;
    sd.MaxLOD = FLT_MAX;
    sd.MipLODBias = 0.0f;

    DXVerify(m_device->CreateSamplerState(&sd, &m_sampleStates[0]));
  }

  void RenderContext::InitializeDepthState(void)
  {
    D3D11_DEPTH_STENCIL_DESC dsDesc;

    ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    // Paramaters for Depth test
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

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