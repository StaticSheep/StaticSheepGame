#pragma once

#include "RenderTarget.h"
#include "DirectXIncludes.h"

namespace DirectSheep
{
  class RenderContext : public RenderTarget
  {
  public:
    HWND getHandle() const;

    void present();

    RenderContext(IDXGIFactory* Factory,
      ID3D11Device* dev, ID3D11DeviceContext* devCon,
      HWND hwnd, UINT width, UINT height, bool fullscreen);

    void vSync(bool enabled);

    ~RenderContext();

  private:
    void createSwapChain(IDXGIFactory* factory, UINT width, UINT height, bool fullscreen);

  private:
    HWND m_hwnd;

    bool m_vSync;

    IDXGISwapChain* m_swapChain;
  };
}