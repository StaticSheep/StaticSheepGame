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
    D3D11_TEXTURE2D_DESC texDesc;
    m_rawTex->GetDesc(&texDesc);

    return texDesc.Width;
  }

  UINT Tex2D::getHeight() const
  {
    D3D11_TEXTURE2D_DESC texDesc;
    m_rawTex->GetDesc(&texDesc);

    return texDesc.Height;
  }

  Tex2D::Tex2D(ID3D11Device* dev,
    UINT width, UINT height, DXGI_FORMAT format)
  {
    CD3D11_TEXTURE2D_DESC textureDesc(format,
      width, height, 1, 0,
      D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

    DXVerify(dev->CreateTexture2D(&textureDesc, NULL, m_rawTex.GetAddressOf()));

    DXVerify(dev->CreateRenderTargetView(m_rawTex.Get(), NULL, m_renderTarget.GetAddressOf()));

    DXVerify(dev->CreateShaderResourceView(m_rawTex.Get(), NULL, m_ShaderRes.GetAddressOf()));
  }

  Tex2D::Tex2D(ID3D11Device* dev, ID3D11Texture2D* raw)
  {
    ULONG refs = raw->AddRef();

    m_rawTex.Attach(raw);

    DXVerify(dev->CreateRenderTargetView(m_rawTex.Get(), NULL, m_renderTarget.GetAddressOf()));

    dev->CreateShaderResourceView(m_rawTex.Get(), NULL, m_ShaderRes.GetAddressOf());
  }

  Tex2D::Tex2D(ID3D11Device* dev, const std::string& filename)
  {
    std::wstring test(filename.begin(), filename.end());

    HRESULT hr = DirectX::CreateWICTextureFromFile(dev, test.c_str(), (ID3D11Resource **)m_rawTex.GetAddressOf(), m_ShaderRes.GetAddressOf(), 0);

    if (FAILED(hr))
    {
      DirectX::CreateWICTextureFromFile(dev, L"content/Default.png", (ID3D11Resource **)m_rawTex.GetAddressOf(), m_ShaderRes.GetAddressOf(), 0);
    }

    if (FAILED(hr))
      DXVerify(hr);
  }
}