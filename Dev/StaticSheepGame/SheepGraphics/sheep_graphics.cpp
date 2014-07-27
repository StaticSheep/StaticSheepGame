#include "GFX_Core.h"
DirectX_Core *CORE = NULL;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

using namespace DirectX;
GFX_API void GFX_Init_D3D(HWND hWnd, int ScreenWidth, int ScreenHeight)
{

  CORE = new DirectX_Core();
  ZeroMemory(CORE, sizeof(DirectX_Core));

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
  D3D11CreateDeviceAndSwapChain(NULL,
                                D3D_DRIVER_TYPE_HARDWARE,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                D3D11_SDK_VERSION,
                                &scd,
                                &CORE->swapchain,
                                &CORE->dev,
                                NULL,
                                &CORE->devcon);

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
  CORE->dev->CreateTexture2D(&texd, NULL, &pDepthBuffer);

  // create the depth buffer
  D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
  ZeroMemory(&dsvd, sizeof(dsvd));

  dsvd.Format = DXGI_FORMAT_D32_FLOAT;
  dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

  CORE->dev->CreateDepthStencilView(pDepthBuffer, &dsvd, &CORE->zbuffer);
  pDepthBuffer->Release();


  // get the address of the back buffer
  ID3D11Texture2D *pBackBuffer;
  CORE->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

  // use the back buffer address to create the render target
  CORE->dev->CreateRenderTargetView(pBackBuffer, NULL, &CORE->backbuffer);
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
  viewport.MinDepth = 0;    // the closest an object can be on the depth buffer is 0.0
  viewport.MaxDepth = 1;    // the farthest an object can be on the depth buffer is 1.0

  CORE->devcon->RSSetViewports(1, &viewport);
}

GFX_API void GFX_Release_D3D(void)
{
  CORE->zbuffer->Release();
  CORE->backbuffer->Release();
  CORE->devcon->Release();
  CORE->dev->Release();
  CORE->swapchain->Release();
  delete CORE;
  CORE = NULL;
}

GFX_API void GFX_Draw(void)
{
  CORE->devcon->ClearDepthStencilView(CORE->zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);

  CORE->devcon->ClearRenderTargetView(CORE->backbuffer, (D3DXCOLOR)Colors::Red);

  CORE->swapchain->Present(0, 0);
}