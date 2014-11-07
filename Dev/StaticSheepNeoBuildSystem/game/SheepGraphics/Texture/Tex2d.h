#pragma once

#include "DirectXIncludes.h"

namespace DirectSheep
{
  class Tex2D
  {
    friend class RenderContext;

  public:

    DXGI_FORMAT getFormat() const;

    UINT getWidth() const;

    UINT getHeight() const;

    Tex2D(ID3D11Device* dev, UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

    Tex2D(ID3D11Device* dev, ID3D11Texture2D* raw);

    Tex2D(ID3D11Device* dev, const std::string& filename);


  private:
    ID3D11Texture2D* m_rawTex;
    ID3D11RenderTargetView* m_renderTarget;
    ID3D11ShaderResourceView* m_ShaderRes;
  };
}