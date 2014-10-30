#pragma once

#include "RenderTarget.h"
#include "DirectXIncludes.h"

namespace DirectSheep
{
  class RenderContext : public RenderTarget
  {
  public:


    RenderContext(IDXGIFactory* Factory,
      ID3D11Device* dev, ID3D11DeviceContext* devCon,
      HWND hwnd, UINT width, UINT height, bool fullscreen);



  private:

  private:
    HWND m_hwnd;

    bool m_vSync;

    IDXGISwapChain* m_swapChain;
  };
}
