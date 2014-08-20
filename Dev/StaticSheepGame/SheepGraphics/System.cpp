#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "DxErr.lib")
#pragma comment (lib, "FW1FontWrapper.lib")

using namespace DirectX;

DirectSheep::DirectX_Core* CORE = NULL;
DirectSheep::VertexBufferQuad *QUAD = NULL;
DirectSheep::States *STATES = NULL;
DirectSheep::TextureMap *TEXTUREMAP = NULL;
DirectSheep::ShaderMap SHADERMAP;

float ScreenHeight = 0;
float ScreenWidth = 0;

namespace DirectSheep
{
  void InitGeometry(void);
  void SetStates(void);
  void LoadDefaultShader(void);
  void SetupMatrices(void);
  void CreateConstantBuffer(void);

  GFX_API void Init(HWND hWnd, int screenWidth, int screenHeight)
  {

    HRESULT hr = S_OK;
    D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_NULL;
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    CORE = new DirectX_Core;
    QUAD = new VertexBufferQuad;
    STATES = new States;
    TEXTUREMAP = new TextureMap;
    CAMERA = new Camera;

    ScreenWidth = (float)screenWidth;
    ScreenHeight = (float)screenHeight;

    ZeroMemory(CORE, sizeof(DirectX_Core));
    ZeroMemory(QUAD, sizeof(VertexBufferQuad));
    ZeroMemory(STATES, sizeof(States));
    ZeroMemory(CAMERA, sizeof(Camera));

    SHAPESTATES.useCamera = true;

    UINT deviceFlags = 0;

  #if defined (_DEBUG)
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
  #endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
      D3D_DRIVER_TYPE_HARDWARE,
      D3D_DRIVER_TYPE_WARP,
      D3D_DRIVER_TYPE_REFERENCE,
    };

    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
    };

    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    // struct to hold information about the swapchain
    DXGI_SWAP_CHAIN_DESC swapDesc;

    // zero out all members
    ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    // Set arguments for swapchain creation
    swapDesc.BufferCount = 1;                                   // single back buffer
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // 32-bit color
    swapDesc.BufferDesc.Width = screenWidth;                    // back buffer width
    swapDesc.BufferDesc.Height = screenHeight;                  // back buffer height
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // use buffer as render target
    swapDesc.OutputWindow = hWnd;                               // link to window
    swapDesc.SampleDesc.Count = 4;                              // multisamples
    swapDesc.Windowed = TRUE;                                   // windowed/full-screen mode
    swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

    // create DirectX device, it's context, and swapchain using swapDesc

    for(UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
      driverType = driverTypes[driverTypeIndex];

      hr = D3D11CreateDeviceAndSwapChain(NULL,
                                  driverType,
                                  NULL,
                                  deviceFlags,
                                  featureLevels,
                                  numFeatureLevels,
                                  D3D11_SDK_VERSION,
                                  &swapDesc,
                                  &CORE->swapchain,
                                  &CORE->dev,
                                  &featureLevel,
                                  &CORE->devcon);

      if(SUCCEEDED(hr))
        break;
    }
    DXVerify(hr);

    D3D11_TEXTURE2D_DESC texd;
    ZeroMemory(&texd, sizeof(texd));

    texd.Width = screenWidth;
    texd.Height = screenHeight;
    texd.ArraySize = 1;
    texd.MipLevels = 1;
    texd.SampleDesc.Count = 4;
    texd.Format = DXGI_FORMAT_D32_FLOAT;
    texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D *pDepthBuffer;
    DXVerify(CORE->dev->CreateTexture2D(&texd, NULL, &pDepthBuffer));

    // create a depth buffer (z-sorting)
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));

    dsvd.Format = DXGI_FORMAT_D32_FLOAT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

    DXVerify(CORE->dev->CreateDepthStencilView(pDepthBuffer, &dsvd, &CORE->zbuffer));
    pDepthBuffer->Release();

    // find the address of the backbuffer
    ID3D11Texture2D *pBackBuffer;
    DXVerify(CORE->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));

    // create render target using backbuffer address
    DXVerify(CORE->dev->CreateRenderTargetView(pBackBuffer, NULL, &CORE->backbuffer));
    pBackBuffer->Release();

    // Set render target as the backbuffer
    CORE->devcon->OMSetRenderTargets(1, &CORE->backbuffer, CORE->zbuffer);


    // Init viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)screenWidth;
    viewport.Height = (float)screenHeight;
    viewport.MinDepth = 0.0f;    // Closest plane accounted by depth buffer (0 - 1 scale)
    viewport.MaxDepth = 1.0f;    // Furthest plane accounted by depth buffer (0 - 1 scale)

    // Register the viewport with device
    CORE->devcon->RSSetViewports(1, &viewport);

    // Set screen dimensions for camera
    CAMERA->ScreenDimensions = Vec2(ScreenWidth, ScreenHeight);

	  HRESULT hResult = FW1CreateFactory(FW1_VERSION, &CORE->pFW1Factory);
	
	  hResult = CORE->pFW1Factory->CreateFontWrapper(CORE->dev, L"Arial", &CORE->pFontWrapper);

    SetStates();

    InitGeometry();
    LoadTexture("content/bricks.png");
    LoadDefaultShader();
    CreateConstantBuffer();
  }

  void SetStates(void)
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

    //rd.FillMode = D3D11_FILL_WIREFRAME;
    //rd.AntialiasedLineEnable = TRUE;

    DXVerify(CORE->dev->CreateRasterizerState(&rd, &STATES->RSDefault));

    /*// set the changed values for wireframe mode
    rd.FillMode = D3D11_FILL_WIREFRAME;
    rd.AntialiasedLineEnable = TRUE;

    dev->CreateRasterizerState(&rd, &pRSWireFrame);*/

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

    DXVerify(CORE->dev->CreateSamplerState(&sd, &STATES->SS));     // create the default sampler

    
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

    DXVerify(CORE->dev->CreateBlendState(&bd, &STATES->BS));
  }

  void InitGeometry(void)
  {
    // create a triangle using the VERTEX struct
    Vertex2D QuadVertices[] =
    {
        {Vec3(-0.5f, -0.5f, 0.0f), D3DXCOLOR(Colors::White), 0.0f, 1.0f},
        {Vec3(-0.5f, 0.5f, 0.0f), D3DXCOLOR(Colors::White), 0.0f, 0.0f},
        {Vec3(0.5f, -0.5f, 0.0f), D3DXCOLOR(Colors::White), 1.0f, 1.0f},

        {Vec3(-0.5f, 0.5f, 0.0f), D3DXCOLOR(Colors::White), 0.0f, 0.0f},
        {Vec3(0.5f, 0.5f, 0.0f), D3DXCOLOR(Colors::White), 1.0f, 0.0f},        
        {Vec3(0.5f, -0.5f, 0.0f), D3DXCOLOR(Colors::White), 1.0f, 1.0f},
    };

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access by CPU and GPU
    bd.ByteWidth = sizeof(Vertex2D) * (sizeof(QuadVertices) / sizeof(Vertex2D));  // size is the VERTEX struct
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    DXVerify(CORE->dev->CreateBuffer(&bd, NULL, &QUAD->vBuffer));       // create the buffer

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    CORE->devcon->Map(QUAD->vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, QuadVertices, sizeof(QuadVertices));                 // copy the data
    CORE->devcon->Unmap(QUAD->vBuffer, NULL);
  }

  void LoadDefaultShader(void)
  {
    //HRESULT hr;

        // load and compile the two shaders
    ID3D10Blob *VS, *PS, *Errors; // Raw data type used to hold buffers

    ID3D11VertexShader* tempVS;
    ID3D11PixelShader* tempPS;
    ID3D11InputLayout* tempInput;

    D3DX11CompileFromFile("SheepGraphics/Shaders/Generic.hlsl", 
                          0, 
                          0, 
                          "VShader", 
                          "vs_4_0", 
                          D3D10_SHADER_OPTIMIZATION_LEVEL3 | D3D10_SHADER_WARNINGS_ARE_ERRORS, // fully optimized shader
                          0, 
                          0, 
                          &VS, 
                          &Errors, 
                          0);

    ErrorIf(Errors || !VS, "ShaderCompile", "Vertex Shader Failed To Compile"); // if there are any errors...

    D3DX11CompileFromFile("SheepGraphics/Shaders/Generic.hlsl", 
                          0, 
                          0, 
                          "PShader", 
                          "ps_4_0", 
                          D3D10_SHADER_OPTIMIZATION_LEVEL3 | D3D10_SHADER_WARNINGS_ARE_ERRORS,
                          0, 
                          0, 
                          &PS, 
                          &Errors, 
                          0);

    ErrorIf(Errors || !PS, "ShaderCompile", "Pixel Shader Failed To Compile"); // if there are any errors...

    // encapsulate both shaders into shader objects
    DXVerify(CORE->dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &tempVS));
    DXVerify(CORE->dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &tempPS));

    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    DXVerify(CORE->dev->CreateInputLayout(ied, ARRAYSIZE(ied), VS->GetBufferPointer(), VS->GetBufferSize(), &tempInput));

    DXShader newShader;
    newShader.VS = tempVS;
    newShader.PS = tempPS;
    newShader.Input = tempInput;

    SHADERMAP["Generic"] = newShader;

    CORE->pVS = tempVS;
    CORE->pPS = tempPS;
    CORE->pLayout = tempInput;

    // set the shader objects
    CORE->devcon->VSSetShader(CORE->pVS, 0, 0);
    CORE->devcon->PSSetShader(CORE->pPS, 0, 0);
    CORE->devcon->IASetInputLayout(CORE->pLayout);
  }

  void CreateConstantBuffer(void)
  {
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = 64;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    DXVerify(CORE->dev->CreateBuffer(&bd, NULL, &CORE->pCBuffer));
    CORE->devcon->VSSetConstantBuffers(0, 1, &CORE->pCBuffer);
  }
  

  GFX_API void Release(void)
  {
    if(CORE->swapchain)
      CORE->swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

    SafeRelease(STATES->BS);
    SafeRelease(STATES->RSDefault);
    SafeRelease(STATES->SS);
    SafeRelease(QUAD->vBuffer);
    SafeRelease(CORE->pFontWrapper);
    SafeRelease(CORE->pFW1Factory);
    SafeRelease(CORE->pCBuffer);
    SafeRelease(CORE->pPS);
    SafeRelease(CORE->pVS);
    SafeRelease(CORE->pLayout);
    SafeRelease(CORE->zbuffer);
    SafeRelease(CORE->backbuffer);
    SafeRelease(CORE->devcon);
    SafeRelease(CORE->dev);
    SafeRelease(CORE->swapchain);

    for(unsigned i  = 0; i < TEXTUREMAP->TextureVec.size(); ++i)
      SafeRelease(TEXTUREMAP->TextureVec[i]);

    delete CORE;
    delete QUAD;
    delete CAMERA;
    delete STATES;
    delete TEXTUREMAP;

    CORE = NULL;
    QUAD = NULL;
    CAMERA = NULL;
    STATES = NULL;
  }

  GFX_API void LoadShader(const std::string& name)
  {
    // TODO
  }

  GFX_API void LoadTexture(const std::string& filename)
  {
    ID3D11ShaderResourceView* newTexture = NULL;

    if(SUCCEEDED(D3DX11CreateShaderResourceViewFromFile(CORE->dev,       // the Direct3D device
                                                        filename.c_str(),// load texture at path
                                                        NULL,            // no additional information
                                                        NULL,            // no multithreading
                                                        &newTexture,     // address of the shader-resource-view
                                                        NULL)))          // no multithreading
    {
      TEXTUREMAP->TextureVec.push_back(newTexture);
      TEXTUREMAP->TextureIndex[filename.c_str()] = TEXTUREMAP->TextureVec.size() - 1;
    }
    else
    {
      ErrorIf(false, "sheep_graphics", "Failed to load texture %s", filename.c_str());
    }
  }

 GFX_API int GetTextureID(std::string& texture)
  {
    auto it = TEXTUREMAP->TextureIndex.find(texture);
    if(it != TEXTUREMAP->TextureIndex.end())
      return it->second;
    else
    {
      LoadTexture(texture);
      return GetTextureID(texture);
    }
  }

}


