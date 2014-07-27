#pragma once

#include "graphics/api.h"

struct DirectX_Core
{
  IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
  ID3D11Device *dev;                     // the pointer to our Direct3D device interface
  ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
  ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer
  ID3D11DepthStencilView *zbuffer;       // the pointer to our depth buffer
  ID3D11InputLayout *pLayout;            // the pointer to the input layout
};

extern DirectX_Core *CORE;

GFX_API void GFX_Init_D3D(HWND hWnd, int ScreenWidth, int ScreenHeight);
GFX_API void GFX_Release_D3D(void);
GFX_API void GFX_Draw(void);