#pragma once

#include "DirectXIncludes.h"

namespace DirectSheep
{
  class Tex2D
  {
    friend class RenderTarget;

  public:








  private:
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_rawTex;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTarget;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShaderRes;
  };
}
