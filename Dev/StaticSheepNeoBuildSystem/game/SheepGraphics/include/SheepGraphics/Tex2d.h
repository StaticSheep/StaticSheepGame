#pragma once

#include "DirectXIncludes.h"

namespace DirectSheep
{
  class Tex2D
  {
    friend class RenderContext;

  public:








  private:
    ID3D11Texture2D* m_rawTex;
    ID3D11RenderTargetView* m_renderTarget;
    ID3D11ShaderResourceView* m_ShaderRes;
  };
}
