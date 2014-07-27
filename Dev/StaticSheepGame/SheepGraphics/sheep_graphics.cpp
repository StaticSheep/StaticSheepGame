#include "GFX_Core.h"
#include "VertexTypes.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "DxErr.lib")

using namespace Framework;
DirectX_Core *Framework::CORE = NULL;
VertexBufferQuad *Framework::QUAD = NULL;
extern ID3D11InputLayout *Framework::LAYOUT;


using namespace DirectX;
GFX_API void GFX_Init_D3D(HWND hWnd, int ScreenWidth, int ScreenHeight)
{

  HRESULT hr = S_OK;
  D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_NULL;
  D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

  CORE = new DirectX_Core();
  ZeroMemory(CORE, sizeof(DirectX_Core));

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
  scd.BufferDesc.Width = ScreenWidth;                    // set the back buffer width
  scd.BufferDesc.Height = ScreenHeight;                  // set the back buffer height
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
  viewport.Width = (float)ScreenHeight;
  viewport.Height = (float)ScreenHeight;
  viewport.MinDepth = 0.0f;    // the closest an object can be on the depth buffer is 0.0
  viewport.MaxDepth = 1.0f;    // the farthest an object can be on the depth buffer is 1.0

  CORE->devcon->RSSetViewports(1, &viewport);
  // create the depth buffer texture
  D3D11_TEXTURE2D_DESC texd;
  ZeroMemory(&texd, sizeof(texd));

  texd.Width = ScreenWidth;
  texd.Height = ScreenHeight;
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


}

GFX_API void GFX_Release_D3D(void)
{
  if(CORE->swapchain)
    CORE->swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

  SafeRelease(CORE->zbuffer);
  SafeRelease(CORE->backbuffer);
  SafeRelease(CORE->devcon);
  SafeRelease(CORE->dev);
  SafeRelease(CORE->swapchain);
  delete CORE;
  CORE = NULL;
}

GFX_API void GFX_Draw(void)
{
  CORE->devcon->ClearDepthStencilView(CORE->zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);

  CORE->devcon->ClearRenderTargetView(CORE->backbuffer, (D3DXCOLOR)Colors::Red);

  CORE->swapchain->Present(0, 0);
}

GFX_API bool InitGeometry()
{
  return true;
}

