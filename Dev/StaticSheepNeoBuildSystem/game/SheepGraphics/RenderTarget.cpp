#include "precompiled.h"
#include "RenderTarget.h"

using namespace DirectX;
namespace DirectSheep
{
  void RenderTarget::clearBackBuffer(const Color& color)
  {
    m_linkedContext->ClearRenderTargetView(m_Target->m_renderTarget, (float*)&Vec4(color.R(), color.G(), color.B(), color.A()));
  }

  void RenderTarget::clearDepthBuffer()
  {
    m_linkedContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
  }

  UINT RenderTarget::getHeight() const
  {
    return m_Target->getHeight();
  }

  UINT RenderTarget::getWidth() const
  {
    return m_Target->getWidth();
  }

  void RenderTarget::startFrame(Color& clearColor)
  {
    clearBackBuffer(clearColor);
    clearDepthBuffer();
  }

  void RenderTarget::endFrame()
  {
  }

  void RenderTarget::startBatch()
  {
    m_Batcher->Begin(DirectX::SpriteSortMode::SpriteSortMode_Texture, m_blendState[PREMULTIPLIED_ALPHA], nullptr, m_depthEnabled ? m_depthStencilState : NULL);
  }

  void RenderTarget::endBatch()
  {
    m_Batcher->End();
  }

  RenderTarget::RenderTarget(ID3D11Device* dev, ID3D11DeviceContext* devCon,
    UINT width, UINT height, DXGI_FORMAT format) :
    m_blendEnabled(false),
    m_depthEnabled(false),
    m_linkedContext(devCon),
    m_linkedDevice(dev)
  {
    m_Target.reset(new Tex2D(dev, width, height, format));

    createBlendState(dev);

    createDepthStencilandView(dev, width, height);

    createRastState(dev);

    createSamplerState(dev);

    m_Batcher.reset(new SpriteBatch(devCon));

    createViewport(width, height);
  }

  void RenderTarget::renderText(DirectX::SpriteFont* Font, const std::string& text,
    const Vec2& position, Color& color,
    float rotation,
    const Vec2& origin,
    const Vec2& scale,
    DirectX::SpriteEffects effect, float layer)
  {
    m_linkedContext->OMSetDepthStencilState(m_depthEnabled ? m_depthStencilState : NULL, 0);

    m_linkedContext->OMSetRenderTargets(1, &m_Target->m_renderTarget, m_depthEnabled ? m_depthStencilView : NULL);

    m_linkedContext->RSSetViewports(1, &m_viewport);

    Font->DrawString(m_Batcher.get(), std::wstring(text.begin(), text.end()).c_str(),
      position, color, rotation, origin, scale, effect, layer);
  }

  void RenderTarget::turnBlending(bool isBlend)
  {
    m_blendEnabled = isBlend;
  }

  void RenderTarget::turnDepth(bool isDepth)
  {
    m_depthEnabled = isDepth;
  }

  void RenderTarget::setTargetOutput(Tex2D* Target)
  {
    m_Target.reset(Target);
  }

  void RenderTarget::createBlendState(ID3D11Device* dev)
  {

    D3D11_RENDER_TARGET_BLEND_DESC rtbd;
    ZeroMemory(&rtbd, sizeof(rtbd));

    rtbd.BlendEnable = true;
    rtbd.BlendOp = D3D11_BLEND_OP_ADD;
    rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    rtbd.DestBlend = D3D11_BLEND_DEST_ALPHA;
    rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtbd.DestBlendAlpha = D3D11_BLEND_ONE;
    rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(blendDesc));

    blendDesc.AlphaToCoverageEnable = true;
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0] = rtbd;

    DXVerify(dev->CreateBlendState(&blendDesc, &m_blendState[BlendStates::ADDITIVE]));

    rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;

    DXVerify(dev->CreateBlendState(&blendDesc, &m_blendState[BlendStates::PREMULTIPLIED_ALPHA]));
  }

  void RenderTarget::createDepthStencilandView(ID3D11Device* dev, UINT width, UINT height)
  {
    D3D11_DEPTH_STENCIL_DESC dsDesc;
    ZeroMemory(&dsDesc, sizeof(dsDesc));

    dsDesc.StencilEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask = 0xFF;

    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    DXVerify(dev->CreateDepthStencilState(&dsDesc, &m_depthStencilState));

    ID3D11Texture2D* Tex(nullptr);

    CD3D11_TEXTURE2D_DESC texDesc(DXGI_FORMAT_D24_UNORM_S8_UINT,
      width, height, 1, 0, D3D11_BIND_DEPTH_STENCIL);

    DXVerify(dev->CreateTexture2D(&texDesc, nullptr, &Tex));

    DXVerify(dev->CreateDepthStencilView(Tex, nullptr, &m_depthStencilView));
  }

  void RenderTarget::createViewport(UINT width, UINT height)
  {
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.MinDepth = 0.0f;
    m_viewport.Width = static_cast<FLOAT>(width);
    m_viewport.Height = static_cast<FLOAT>(height);
  }

  void RenderTarget::createSamplerState(ID3D11Device* dev)
  {
    D3D11_SAMPLER_DESC sd;
    ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
    sd.Filter = D3D11_FILTER_ANISOTROPIC;
    sd.MaxAnisotropy = 16;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.BorderColor[0] = 0.0f;
    sd.BorderColor[1] = 0.0f;
    sd.BorderColor[2] = 0.0f;
    sd.BorderColor[3] = 0.0f;
    sd.MinLOD = 0.0f;
    sd.MaxLOD = FLT_MAX;
    sd.MipLODBias = 0.0f;

    DXVerify(dev->CreateSamplerState(&sd, &m_sampler));
  }

  void RenderTarget::createRastState(ID3D11Device* dev)
  {
    D3D11_RASTERIZER_DESC rd;
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;
    rd.FrontCounterClockwise = FALSE;
    rd.DepthClipEnable = TRUE;
    rd.ScissorEnable = FALSE;
    rd.AntialiasedLineEnable = FALSE;
    rd.MultisampleEnable = FALSE;
    rd.DepthBias = 0;
    rd.DepthBiasClamp = 0.0f;
    rd.SlopeScaledDepthBias = 0.0f;

    DXVerify(dev->CreateRasterizerState(&rd, &m_rastState));
  }
}