/*****************************************************************
Filename: Tex2d.h
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
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

    Tex2D(ID3D11Device* dev, UINT width, UINT height,
      DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT);

    Tex2D(ID3D11Device* dev, ID3D11Texture2D* raw);

    Tex2D(ID3D11Device* dev, const std::string& filename);

    void Release(void);


  private:
    ID3D11Texture2D* m_rawTex;
    ID3D11RenderTargetView* m_renderTarget;
    ID3D11ShaderResourceView* m_ShaderRes;

    unsigned m_width;
    unsigned m_height;
  };
}