/*****************************************************************
Filename: Tex2d.cpp
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "precompiled.h"
#include "Tex2d.h"
#include "WICTextureLoader.h"
#include "Context\Context.h"
#include "Effects\premult_effect.h"

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

  Tex2D::Tex2D(ID3D11Device* dev, const std::string& filename)
    : m_rawTex(NULL), m_renderTarget(NULL), m_ShaderRes(NULL),
    m_texName(filename)
  {
    m_texName = filename;

    Load();
  }

  void Tex2D::Load()
  {
    std::wstring test(m_texName.begin(), m_texName.end());
    HRESULT hr = DirectX::CreateWICTextureFromFile(RenderContext::Context->
      GetDevice(),
      test.c_str(), (ID3D11Resource **)&m_rawTex, &m_ShaderRes, 0);

    if (FAILED(hr))
    {
      hr = DirectX::CreateWICTextureFromFile(RenderContext::Context->GetDevice(),
        L"content\\Default.png", (ID3D11Resource **)&m_rawTex, &m_ShaderRes, 0);
      if (FAILED(hr))
        DXVerify(hr);
    }

    


    D3D11_TEXTURE2D_DESC texDesc;
    m_rawTex->GetDesc(&texDesc);

    m_width = texDesc.Width;
    m_height = texDesc.Height;


    // Store identity matrix for later use
    Mat4 identity = DirectX::XMMatrixIdentity();
    // Default sampling state (wrap)

    auto dcon = RenderContext::Context->GetDeviceContext();
    auto context = RenderContext::Context;

    D3D11_RENDER_TARGET_VIEW_DESC rtView;
    ZeroMemory(&rtView, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

    rtView.Format = texDesc.Format;
    rtView.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    //rtView.Texture2D.MipSlice = texDesc.MipLevels;

    DXVerify(context->m_device->CreateRenderTargetView(
      m_rawTex, &rtView, &m_renderTarget));

    dcon->OMSetRenderTargets(1,
      &m_renderTarget, nullptr);


    dcon->PSSetSamplers(0, 1, &RenderContext::Context->m_sampleStates[0]);

    PreMultFilter* PreMultEffect = RenderContext::Context->m_preMultFilter;

    PreMultEffect->bind(dcon);
    PreMultEffect->bindSize(dcon,
      Vec2(m_width, m_height));
    // No blend colors
    //PreMultEffect->bindAmbient(m_deviceContext, Vec4(1, 1, 1, 1), 1);
    //PreMultEffect->bind(dcon);

    context->m_quad->bind(dcon);

    context->SetBlendMode(BLEND_MODE_ADDITIVE);

    dcon->PSSetShaderResources(0, 1, &m_ShaderRes);
    dcon->DrawIndexed(context->m_quad->getIndexCount(), 0, 0);
    
  }

  void Tex2D::RefreshTexture()
  {
    Release();
    

    Load();
  }

  void Tex2D::Release(void)
  {
    SafeRelease(m_rawTex);
    SafeRelease(m_renderTarget);
    SafeRelease(m_ShaderRes);
  }
}