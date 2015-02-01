/*****************************************************************
Filename: Tex2d.cpp
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "precompiled.h"
#include "Tex2d.h"
#include "WICTextureLoader.h"

namespace DirectSheep
{
  DXGI_FORMAT Tex2D::getFormat() const
  {
    D3D11_TEXTURE2D_DESC texDesc;
    m_rawTex->GetDesc(&texDesc);

    return texDesc.Format;
  }

  UINT Tex2D::getWidth() const
  {
    return m_width;
  }

  UINT Tex2D::getHeight() const
  {
    return m_height;
  }

  Tex2D::Tex2D(ID3D11Device* dev,
    UINT width, UINT height, DXGI_FORMAT format) : m_rawTex(NULL), m_renderTarget(NULL), m_ShaderRes(NULL)
  {
    CD3D11_TEXTURE2D_DESC textureDesc(format,
      width, height, 1, 0,
      D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

    DXVerify(dev->CreateTexture2D(&textureDesc, NULL, &m_rawTex));

    DXVerify(dev->CreateRenderTargetView(m_rawTex, NULL, &m_renderTarget));

    DXVerify(dev->CreateShaderResourceView(m_rawTex, NULL, &m_ShaderRes));
  }

  Tex2D::Tex2D(ID3D11Device* dev, ID3D11Texture2D* raw) : m_rawTex(NULL), m_renderTarget(NULL), m_ShaderRes(NULL)
  {
    ULONG refs = raw->AddRef();

    m_rawTex = raw;

    DXVerify(dev->CreateRenderTargetView(m_rawTex, NULL, &m_renderTarget));

    dev->CreateShaderResourceView(m_rawTex, NULL, &m_ShaderRes);
  }

  Tex2D::Tex2D(ID3D11Device* dev, const std::string& filename) : m_rawTex(NULL), m_renderTarget(NULL), m_ShaderRes(NULL)
  {
    std::wstring test(filename.begin(), filename.end());

    HRESULT hr = DirectX::CreateWICTextureFromFile(dev, test.c_str(), (ID3D11Resource **)&m_rawTex, &m_ShaderRes, 0);

    if (FAILED(hr))
    {
      hr = DirectX::CreateWICTextureFromFile(dev, L"content\\Default.png", (ID3D11Resource **)&m_rawTex, &m_ShaderRes, 0);
      if (FAILED(hr))
        DXVerify(hr);
    }
    D3D11_TEXTURE2D_DESC texDesc;
    m_rawTex->GetDesc(&texDesc);
    

    m_width = texDesc.Width;
    m_height = texDesc.Height;

  }

  void Tex2D::Release(void)
  {
    SafeRelease(m_rawTex);
    SafeRelease(m_renderTarget);
    SafeRelease(m_ShaderRes);
  }
}