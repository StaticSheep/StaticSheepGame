#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "DxErr.lib")

using namespace DirectX;

DirectSheep::DirectX_Core* CORE = NULL;
DirectSheep::VertexBufferQuad *QUAD = NULL;
DirectSheep::States *STATES = NULL;
DirectSheep::TextureMap TEXTUREMAP;
DirectSheep::shapeStates SHAPESTATES;
DirectSheep::Camera *CAMERA = NULL;

float ScreenHeight = 0;
float ScreenWidth = 0;

namespace DirectSheep
{
  void LoadAssets(void);
  void LoadEffect(void);
  void SetStates(void);
  ID3D11ShaderResourceView* GetTexture(std::string texture);

  GFX_API void InitD3D(HWND hWnd, int screenWidth, int screenHeight)
  {

    HRESULT hr = S_OK;
    D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_NULL;
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    CORE = new DirectX_Core;
    QUAD = new VertexBufferQuad;
    STATES = new States;
    CAMERA = new Camera;

    ScreenWidth = (int)screenWidth;
    ScreenHeight = (int)screenHeight;

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

     // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                   // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
    scd.BufferDesc.Width = screenWidth;                    // set the back buffer width
    scd.BufferDesc.Height = screenHeight;                  // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
    scd.OutputWindow = hWnd;                               // the window to be used
    scd.SampleDesc.Count = 4;                              // how many multisamples
    scd.Windowed = TRUE;                                   // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

    // create a device, device context and swap chain using the information in the scd struct

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
                                  &scd,
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

    // create the depth buffer
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));

    dsvd.Format = DXGI_FORMAT_D32_FLOAT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

    DXVerify(CORE->dev->CreateDepthStencilView(pDepthBuffer, &dsvd, &CORE->zbuffer));
    pDepthBuffer->Release();

    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    DXVerify(CORE->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));

    // use the back buffer address to create the render target
    DXVerify(CORE->dev->CreateRenderTargetView(pBackBuffer, NULL, &CORE->backbuffer));
    pBackBuffer->Release();

    // set the render target as the back buffer
    CORE->devcon->OMSetRenderTargets(1, &CORE->backbuffer, CORE->zbuffer);


    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)screenWidth;
    viewport.Height = (float)screenHeight;
    viewport.MinDepth = 0.0f;    // the closest an object can be on the depth buffer is 0.0
    viewport.MaxDepth = 1.0f;    // the farthest an object can be on the depth buffer is 1.0

    CORE->devcon->RSSetViewports(1, &viewport);
    // create the depth buffer texture

    CAMERA->ScreenDimensions = Vec2(ScreenWidth, ScreenHeight);

    SetStates();
    LoadAssets();
  }

  GFX_API void DrawSprite()
  {
    Mat4 matFinal;

    D3DXMATRIX scaleMat, rotMat, transMat;

    ID3D11ShaderResourceView* Texture = GetTexture(SHAPESTATES.filename);

    D3DXMatrixIdentity(&rotMat);
    D3DXMatrixIdentity(&transMat);
    D3DXMatrixIdentity(&scaleMat);

    D3DXMatrixScaling(&scaleMat,SHAPESTATES.scale.x, SHAPESTATES.scale.y, 1.0f);
    D3DXMatrixRotationZ(&rotMat, SHAPESTATES.rotation);
    D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);

    if (SHAPESTATES.useCamera)
      D3DXMatrixTranslation(&transMat, floor(SHAPESTATES.position.x), floor(SHAPESTATES.position.y), floor(0.0f));
    else
      D3DXMatrixTranslation(&transMat, floor(SHAPESTATES.position.x) - ScreenWidth / 2, floor(-SHAPESTATES.position.y) + ScreenHeight / 2, floor(0.0f));
    
    D3DXMatrixMultiply(&scaleMat, &scaleMat, &transMat);

    matFinal = scaleMat * CAMERA->ViewProjMatrix;

    CORE->devcon->RSSetState(STATES->RSDefault);

    CORE->devcon->PSSetSamplers(0, 1, &STATES->SS);

    CORE->devcon->OMSetBlendState(STATES->BS, 0, 0xffffffff);

    UINT stride = sizeof(Vertex2D);
    UINT offset = 0;
    CORE->devcon->IASetVertexBuffers(0, 1, &QUAD->vBuffer, &stride, &offset);

    CORE->devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    CORE->devcon->UpdateSubresource(CORE->pCBuffer, 0, 0, &matFinal, 0, 0);
    CORE->devcon->PSSetShaderResources(0, 1, &Texture);
    CORE->devcon->Draw(6, 0);
  }

  GFX_API void FinishFrame()
  {
    CORE->swapchain->Present(0, 0);
  }

  GFX_API void StartFrame(float dt)
  {

    CORE->devcon->ClearRenderTargetView(CORE->backbuffer, (D3DXCOLOR)Colors::Black);

    CORE->devcon->ClearDepthStencilView(CORE->zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
  }

  GFX_API void InitGeometry(void)
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

  void LoadAssets(void)
  {
    InitGeometry();

    LoadTexture("content/bricks.png");

    //GFX_TODO call LoadTexture(filename) 
    //for every texture you want on the texture map
    LoadEffect();
  }

  void LoadEffect(void)
  {
    HRESULT hr;
        // load and compile the two shaders
    ID3D10Blob *VS, *PS, *Errors; // Raw data type used to hold buffers
    D3DX11CompileFromFile("SheepGraphics/Shaders/Shaders.hlsl", 
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

    D3DX11CompileFromFile("SheepGraphics/Shaders/Shaders.hlsl", 
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
    DXVerify(CORE->dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &CORE->pVS));
    DXVerify(CORE->dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &CORE->pPS));

    // set the shader objects
    CORE->devcon->VSSetShader(CORE->pVS, 0, 0);
    CORE->devcon->PSSetShader(CORE->pPS, 0, 0);

    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    DXVerify(CORE->dev->CreateInputLayout(ied, ARRAYSIZE(ied), VS->GetBufferPointer(), VS->GetBufferSize(), &CORE->pLayout));
    CORE->devcon->IASetInputLayout(CORE->pLayout);

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = 64;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

   DXVerify(CORE->dev->CreateBuffer(&bd, NULL, &CORE->pCBuffer));
    CORE->devcon->VSSetConstantBuffers(0, 1, &CORE->pCBuffer);
  }

  GFX_API void SetupMatrices(void)
  {
    float cameraX = 0.0f;
    float cameraY = 0.0f;
    float cameraZ = -10.0f;

    Vec3 eyepoint(cameraX, cameraY, cameraZ);

    Vec3 lootAtPoint(cameraX, cameraY, 0.0f);

    Vec3 upVector( 0.0f, 1.0f, 0.0f);

    Mat4 matView;
    D3DXMatrixLookAtLH(&matView, &eyepoint, &lootAtPoint, &upVector);

    CAMERA->ViewMatrix = matView;

    Mat4 matProj;
    D3DXMatrixOrthoLH(&matProj, CAMERA->ScreenDimensions.x, CAMERA->ScreenDimensions.y, 1.0f, 100.0f);

    CAMERA->ProjMatrix = matProj;

    CAMERA->ViewProjMatrix = matView * matProj;
  }

  GFX_API void ReleaseD3D(void)
  {
    if(CORE->swapchain)
      CORE->swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

    for( TextureMap::iterator it = TEXTUREMAP.begin(); it != TEXTUREMAP.end(); ++it)
      it->second->Release();

    SafeRelease(STATES->BS);
    SafeRelease(STATES->RSDefault);
    SafeRelease(STATES->SS);
    SafeRelease(QUAD->vBuffer);
    SafeRelease(CORE->pCBuffer);
    SafeRelease(CORE->pPS);
    SafeRelease(CORE->pVS);
    SafeRelease(CORE->pLayout);
    SafeRelease(CORE->zbuffer);
    SafeRelease(CORE->backbuffer);
    SafeRelease(CORE->devcon);
    SafeRelease(CORE->dev);
    SafeRelease(CORE->swapchain);

    delete CORE;
    delete QUAD;
    delete STATES;
    delete CAMERA;

    CORE = NULL;
    QUAD = NULL;
    CAMERA = NULL;
    STATES = NULL;
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

  GFX_API void LoadTexture(const std::string& filename)
  {
    ID3D11ShaderResourceView* newTexture = NULL;

    if(SUCCEEDED(D3DX11CreateShaderResourceViewFromFile(CORE->dev,        // the Direct3D device
                                                        filename.c_str(),   // load Wood.png in the local folder
                                                        NULL,             // no additional information
                                                        NULL,             // no multithreading
                                                        &newTexture,      // address of the shader-resource-view
                                                        NULL)))          // no multithreading
    {
      TEXTUREMAP[filename.c_str()] = newTexture;
    }
    else
    {
      ErrorIf(false, "sheep_graphics", "Failed to load texture %s", filename.c_str());
    }
  }

  ID3D11ShaderResourceView* GetTexture(std::string texture)
  {
    TextureMap::iterator it = TEXTUREMAP.find(texture);
    if(it != TEXTUREMAP.end())
      return it->second;
    else
    {
       LoadTexture(texture);
       return GetTexture(texture); 
    }
  }

  GFX_API void SetPosition(float x, float y)
  {
    SHAPESTATES.position = Vec2(x, y);
  }
  GFX_API void SetRotation(float theta)
  {
    SHAPESTATES.rotation = theta;
  }
  GFX_API void SetSize(float x, float y)
  {
    SHAPESTATES.scale = Vec2(x, y);
  }
  GFX_API void SetTexture(std::string& filepath)
  {
    SHAPESTATES.filename = filepath;
  }
  GFX_API void SetUseCamera(bool useCam)
  {
    SHAPESTATES.useCamera = useCam;
  }
}


