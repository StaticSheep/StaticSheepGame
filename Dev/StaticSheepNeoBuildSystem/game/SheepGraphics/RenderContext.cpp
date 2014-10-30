#include "precompiled.h"
#include "RenderContext.h"
#include "Tex2d.h"

namespace DirectSheep
{
  HWND RenderContext::getHandle() const
  {
    return m_hwnd;
  }

  void RenderContext::present()
  {
    m_swapChain->Present((m_vSync ? 1 : 0), 0);
  }

  RenderContext::RenderContext(IDXGIFactory* Factory,
    ID3D11Device* dev, ID3D11DeviceContext* devCon,
    HWND hwnd, UINT width, UINT height, bool fullscreen) : RenderTarget(dev, devCon, width, height),
    m_hwnd(hwnd),
    m_vSync(false)
  {
    createSwapChain(Factory, width, height, fullscreen);

    ID3D11Texture2D* rawTex(nullptr);

    DXVerify(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&rawTex)));

    Tex2D* newTex(new Tex2D(dev, rawTex));

    ULONG refs = rawTex->Release();

    RenderTarget::setTargetOutput(newTex);
  }

  void RenderContext::vSync(bool enabled)
  {
    m_vSync = enabled;
  }

  RenderContext::~RenderContext()
  {
    m_hwnd = nullptr;
  }

  void RenderContext::createSwapChain(IDXGIFactory* factory, UINT width, UINT height, bool fullscreen)
  {
    DXGI_MODE_DESC buffDesc =
    {
      width, height,
      { 0, 1 },
      DXGI_FORMAT_R8G8B8A8_UNORM,
      DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
      DXGI_MODE_SCALING_UNSPECIFIED
    };

    DXGI_SWAP_CHAIN_DESC swapDesc =
    {
      buffDesc,
      { 1, 0 },
      DXGI_USAGE_RENDER_TARGET_OUTPUT,
      1,
      m_hwnd,
      !fullscreen,
      DXGI_SWAP_EFFECT_DISCARD,
      0
    };

    DXVerify(factory->CreateSwapChain(m_linkedDevice, &swapDesc, &m_swapChain));
  }



}