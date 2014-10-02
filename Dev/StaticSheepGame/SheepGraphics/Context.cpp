#include "Context.h"
#include "Handle.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "DxErr.lib")
#pragma comment (lib, "FW1FontWrapper.lib")

#define RGBTOBGR(color) (color & 0xFF000000) | ((color & 0xFF0000) >> 16) | (color & 0x00ff00) | ((color & 0x0000ff) << 16)

using namespace DirectX;

namespace DirectSheep
{

  RenderContext * RenderContext::Allocate(void)
  {
    return new RenderContext();
  }

  void RenderContext::UpdateCamera(float x, float y, float fov)
  {
    float cameraX = x;
    float cameraY = y;
    float cameraZ = -400.0f;


    Vec3 eyepoint(cameraX, cameraY, cameraZ);

    Vec3 lootAtPoint(cameraX, cameraY, 0.0f);

    Vec3 upVector( 0.0f, 1.0f, 0.0f);

    Mat4 matView;
    D3DXMatrixLookAtLH(&matView, &eyepoint, &lootAtPoint, &upVector);

    m_camera.view = matView;

    Mat4 matProj;
    if(m_camera.used)
      D3DXMatrixPerspectiveFovLH(&matProj, (FLOAT)D3DXToRadian(fov), (float)m_viewport.dim.width / (float)m_viewport.dim.height, 1.0f, 1000.0f);
    else
      D3DXMatrixOrthoLH(&matProj, (float)m_viewport.dim.width, (float)m_viewport.dim.height, 1.0f, 1000.0f);
    m_camera.proj = matProj;

    m_camera.viewProj = matView * matProj;
  }

  RenderContext::RenderContext(void)
  {
    m_initialized = false;
    m_hwnd = NULL;
    m_resolution = Dimension(0,0);
    m_nativeResolution = Dimension(0,0);
    m_fullscreen = true;
    m_vsync = false;

    /////////////
    // DirectX //
    /////////////
    m_swapChain = NULL;
    m_device = NULL;   
    m_deviceContext = NULL;
    m_factory = NULL;
    m_adapter = NULL;
    m_output = NULL;
    m_sampleStates[0] = NULL;
    m_sampleStates[1] = NULL;
                                
    m_displayModeIndex = 0;
                                
    m_backBuffer = NULL;
    m_depthBuffer.m_depthBuffer = NULL;
    m_backBufferSize = Dimension(0,0);
    m_clearColor = Color(Colors::DarkSlateGray);
    m_spriteBlend = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
    m_spriteTrans = Transform();
    m_primative = PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    /////////////////////////////////
    // Other render configurations //
    /////////////////////////////////
    m_rastState = NULL;
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

      delete rCon;
  }

    /////////////////////////////////////////////////////////////
    //                     DRAW FUNCTIONS                      //
    /////////////////////////////////////////////////////////////

    void RenderContext::Draw(unsigned vertexCount, unsigned vertexStart)
    {
      Mat4 matFinal;

      D3DXMATRIX scaleMat, rotMat, transMat;

      D3DXMatrixIdentity(&rotMat);
      D3DXMatrixIdentity(&transMat);
      D3DXMatrixIdentity(&scaleMat);

      D3DXMatrixScaling(&scaleMat,m_spriteTrans.w, m_spriteTrans.h, 1.0f);
      D3DXMatrixRotationZ(&rotMat, m_spriteTrans.theta);
      D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);

      D3DXMatrixTranslation(&transMat, m_spriteTrans.x, m_spriteTrans.y, 0.0f);


      D3DXMatrixMultiply(&scaleMat, &scaleMat, &transMat);

      matFinal = scaleMat * m_camera.viewProj;

      DefaultBuffer buffer;
      buffer.Final = matFinal;
      buffer.World = scaleMat;
      buffer.BlendColor = m_spriteBlend;
      buffer.uvBegin = m_spriteTrans.uvBegin;
      buffer.uvEnd = m_spriteTrans.uvEnd;

      m_deviceContext->RSSetState(m_rastState);

      m_deviceContext->PSSetSamplers(0, 1, &m_sampleStates[0]);

      SetBlendMode(BLEND_MODE_ALPHA);

      m_deviceContext->UpdateSubresource(m_constBufferRes[0], 0, 0, &buffer, 0, 0);

      m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      m_deviceContext->Draw(vertexCount,vertexStart);
    }

    void RenderContext::DrawSpriteText(const char * text, float size, const char * font)
    {
      Mat4 matFinal;

      Mat4 rotMat, transMat;

      D3DXMatrixIdentity(&matFinal);
      D3DXMatrixIdentity(&rotMat);
      D3DXMatrixIdentity(&transMat);
      D3DXMatrixRotationYawPitchRoll(&rotMat, 0.0f, (float)-D3DX_PI, m_spriteTrans.theta);

      D3DXMatrixTranslation(&transMat, (float)floor(m_spriteTrans.x), (float)floor(m_spriteTrans.y), (float)floor(0.0f));

      D3DXMatrixMultiply(&rotMat, &rotMat, &transMat);

      matFinal = rotMat * m_camera.viewProj;

      FW1_RECTF rect;
		  rect.Left = rect.Right = 0.0f;
		  rect.Top = rect.Bottom = 0.0f;

      std::string boop(text);
      std::wstring test(boop.begin(), boop.end());

      std::string sfont(font);
      std::wstring WFont(sfont.begin(), sfont.end());

      m_font.m_fontWrapper->DrawString(
      m_deviceContext,
      test.c_str(),// String
      WFont.c_str(),
      size,
      &rect,
      RGBTOBGR(D3DXCOLOR(m_spriteBlend.r,m_spriteBlend.g,m_spriteBlend.b, m_spriteBlend.a)),// Text color, 0xAaBbGgRr
      NULL,
      matFinal,
      FW1_RESTORESTATE | FW1_CENTER | FW1_VCENTER | FW1_NOWORDWRAP
      );// Flags (for example FW1_RESTORESTATE to keep context states 

    }

    void RenderContext::Present(void)
    {
      m_swapChain->Present(m_vsync, 0);
    }

    /////////////////////////////////////////////////////////////
    //                    CREATE FUNCTIONS                     //
    /////////////////////////////////////////////////////////////
    
    bool RenderContext::CreateVertexShader(Handle& handle, const std::string& filename, const InputLayout& inputLayout, const std::string& entryFunc)
    {
      ID3D10Blob *VS, *Errors; // Raw data type used to hold buffers
      VertexShader tempVS;

      D3DX11CompileFromFile(filename.c_str(),
                            0, 
                            0, 
                            entryFunc.c_str(),
                            "vs_4_0", 
                            D3D10_SHADER_OPTIMIZATION_LEVEL3 | D3D10_SHADER_WARNINGS_ARE_ERRORS, // fully optimized shader
                            0, 
                            0, 
                            &VS, 
                            &Errors, 
                            0);

      ErrorIf(Errors || !VS, "ShaderCompile", "Vertex Shader Failed To Compile"); // if there are any errors...

      DXVerify(m_device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &tempVS.vShader));

      std::vector<D3D11_INPUT_ELEMENT_DESC> ied;

      for(unsigned i = 0; i < inputLayout.size(); ++i)
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
      D3DX11CompileFromFile(filename.c_str(),
                            0, 
                            0, 
                            entryFunc.c_str(), 
                            "ps_4_0", 
                            D3D10_SHADER_OPTIMIZATION_LEVEL3 | D3D10_SHADER_WARNINGS_ARE_ERRORS, // fully optimized shader
                            0, 
                            0, 
                            &PS, 
                            &Errors, 
                            0);

      ErrorIf(Errors || !PS, "ShaderCompile", "%s", Errors->GetBufferPointer()); // if there are any errors...

      DXVerify(m_device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &tempPS));

      m_pixelShaderRes.push_back(tempPS);
      handle.type = PIXEL_SHADER;
      handle.index = m_pixelShaderRes.size() - 1;

      return true;
    }

    bool RenderContext::CreateTexture(Handle& handle, const std::string& filename)
    {
      Texture tempTex = {0};
      D3D11_TEXTURE2D_DESC desc;
      static bool attempt = false;
      HRESULT hr = D3DX11CreateShaderResourceViewFromFile(m_device,                                       // the Direct3D device
                                                      filename.c_str(),                               // load texture at path
                                                      NULL,                                           // no additional information
                                                      NULL,                                           // no multithreading
                                                      &tempTex.shaderResourceView, // address of the shader-resource-view
                                                      NULL);                                         // no multithreading

      if(hr == D3D11_ERROR_FILE_NOT_FOUND)
      {
        if(attempt)
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
          {Vec3(-0.5f, -0.5f, 0.0f), 0.0f, 1.0f},
          {Vec3(-0.5f, 0.5f, 0.0f), 0.0f, 0.0f},
          {Vec3(0.5f, -0.5f, 0.0f), 1.0f, 1.0f},

          {Vec3(-0.5f, 0.5f, 0.0f), 0.0f, 0.0f},
          {Vec3(0.5f, 0.5f, 0.0f), 1.0f, 0.0f},        
          {Vec3(0.5f, -0.5f, 0.0f), 1.0f, 1.0f},
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
    //                    BIND FUNCTIONS                       //
    /////////////////////////////////////////////////////////////

    void RenderContext::BindVertexShader(const Handle& vsHandle)
    {
      if(vsHandle.type == VERTEX_SHADER)
      {
        m_deviceContext->VSSetShader(m_vertexShaderRes[vsHandle.index].vShader, 0, 0);
        
      }
    }

    void RenderContext::BindPixelShader(const Handle& psHandle)
    {
      if(psHandle.type == PIXEL_SHADER)
        m_deviceContext->PSSetShader(m_pixelShaderRes[psHandle.index], 0, 0);
    }

    void RenderContext::BindTexture(unsigned slot, const Handle& texHandle)
    {
      if(texHandle.type == TEXTURE)
        m_deviceContext->PSSetShaderResources(slot, 1, &m_textureRes[texHandle.index].shaderResourceView);
    }

    void RenderContext::BindTextures(unsigned count, const Handle texHandles[], unsigned startSlot)
    {
      for(unsigned i = 0; i < count; ++i)
      {
        if(texHandles[i].type == TEXTURE)
          m_deviceContext->PSSetShaderResources(startSlot + i, 1, &m_textureRes[texHandles[i].index].shaderResourceView);
      }
    }

    void RenderContext::BindVertexBuffer(const Handle& vbHandle, size_t stride, size_t offset)
    {
      if(vbHandle.type == VERTEX_BUFFER)
        m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBufferRes[vbHandle.index], &stride, &offset);
    }

    void RenderContext::BindVertexBuffers(unsigned count, const Handle vertexBuffers[], size_t strides[], size_t offsets[])
    {
      for(unsigned i = 0; i < count; ++i)
      {
        if(vertexBuffers[i].type == VERTEX_BUFFER)
          m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBufferRes[vertexBuffers[i].index], &strides[i], &offsets[i]);
      }
    }

    void RenderContext::BindIndexBuffer(const Handle& ibHandle)
    {
      if(ibHandle.type == INDEX_BUFFER)
        m_deviceContext->IASetIndexBuffer(m_indexBufferRes[ibHandle.index], DXGI_FORMAT_R32_UINT, 0);
    }

    void RenderContext::BindConstantBuffer(unsigned slot, const Handle& cbHandle, ObjectType shaderType)
    {
      if(cbHandle.type == CONSTANT_BUFFER)
      {
        if(shaderType == VERTEX_SHADER)
          m_deviceContext->VSSetConstantBuffers(slot, 1, &m_constBufferRes[cbHandle.index]);
        if(shaderType == PIXEL_SHADER)
          m_deviceContext->PSSetConstantBuffers(slot, 1, &m_constBufferRes[cbHandle.index]);
      }
    }

    void RenderContext::BindRenderTarget(const Handle& rtHandle)
    {
      if(rtHandle.type == RENDER_TARGET)
        m_deviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthBuffer.m_depthBuffer);
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
     m_spriteBlend = D3DXCOLOR(r,g,b,a);
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
        m_deviceContext->ClearRenderTargetView(m_renderTargetRes[handle.index].renderTargetView, D3DXCOLOR(r,g,b,a));
    }

    void RenderContext::ClearRenderTarget(const Handle& handle, Color clearColor)
    {
      if(handle.type == RENDER_TARGET)
        m_deviceContext->ClearRenderTargetView(m_renderTargetRes[handle.index].renderTargetView, D3DXCOLOR(clearColor.r,clearColor.g,clearColor.b,clearColor.a));
    }

    void RenderContext::ClearBackBuffer(void)
    {
      m_deviceContext->ClearRenderTargetView(m_backBuffer, D3DXCOLOR(m_clearColor.r,m_clearColor.g,m_clearColor.b,1.0f));
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
    for(UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
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

      if(SUCCEEDED(hr)) // If succeeded then break otherwise try lower driver settings
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