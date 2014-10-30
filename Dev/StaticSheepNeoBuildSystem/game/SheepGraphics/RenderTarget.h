#pragma once

#include "Effects\Effect.h"
#include "model.h"
#include "Tex2d.h"
#include "DirectXIncludes.h"
#include "DataTypes.h"

namespace DirectSheep
{
  enum BlendStates
  {
    ADDITIVE = 0,
    PREMULTIPLIED_ALPHA,
    NUMSTATES,
  };
  class RenderTarget
  {
  public:

    void clearBackBuffer(const Color& color);

    void clearDepthBuffer();

    UINT getHeight() const;

    UINT getWidth() const;

    void startFrame(Color& clearColor);

    void startBatch();

    void endBatch();

    void endFrame();

    template<class VertexType>
    void render(Model<VertexType>* Model, Effect* effect, BlendStates blend = BlendStates::PREMULTIPLIED_ALPHA)
    {
      Model->bind(m_linkedContext);

      effect->bind(m_linkedContext);

      m_linkedContext->OMSetBlendState((m_blendEnabled ? m_blendState[blend] : nullptr), (float*)&Vec4(1.0f,1.0f,1.0f,1.0f), 0xffffffff);

      m_linkedContext->OMSetDepthStencilState(m_depthEnabled ? m_depthStencilState : nullptr, 0);

      m_linkedContext->OMSetRenderTargets(1, &m_Target->m_renderTarget, m_depthEnabled ? m_depthStencilView : NULL);

      m_linkedContext->RSSetViewports(1, &m_viewport);

      m_linkedContext->DrawIndexed(Model->getIndexCount(), 0, 0);
    }

    template<class VertexType>
    void render(Model<VertexType>* Model, Effect* effect, Tex2D texture, BlendStates blend = BlendStates::PREMULTIPLIED_ALPHA)
    {
      Model->bind(m_linkedContext);

      effect->bind(m_linkedContext);

      m_linkedContext->OMSetBlendState((m_blendEnabled ? m_blendState[blend] : nullptr), (float*)&Vec4(1.0f, 1.0f, 1.0f, 1.0f), 0xffffffff);

      m_linkedContext->OMSetDepthStencilState(m_depthEnabled ? m_depthStencilState : nullptr, 0);

      m_linkedContext->OMSetRenderTargets(1, &m_Target->m_renderTarget, m_depthEnabled ? m_depthStencilView : NULL);

      m_linkedContext->PSSetSamplers(0, 1, &m_sampler);

      m_deviceContext->RSSetState(m_rastState);

      m_linkedContext->RSSetViewports(1, &m_viewport);

      m_linkedContext->PSSetShaderResources(slot, 1, &texture.m_ShaderRes);

      m_linkedContext->DrawIndexed(Model->getIndexCount(), 0, 0);
    }

    void renderBatch(Tex2D texture,
      const Vec2& position = Vec2(0, 0), Color& color = Color(1, 1, 1, 1),
      float rotation = 0.0f,
      const Vec2& scale = Vec2(1.0f, 1.0f),
      const Vec4& uv = Vec4(0.0f, 0.0f, 1.0f, 1.0f),
      DirectX::SpriteEffects effect = DirectX::SpriteEffects_None, float layer = 0.0f)
    {
      unsigned width = texture.getWidth();
      unsigned height = texture.getHeight();

      RECT sourcePos;
      sourcePos.left = (long)(width * uv.x);
      sourcePos.right = (long)(width * uv.z);
      sourcePos.top = (long)(height * uv.y);
      sourcePos.bottom = (long)(height * uv.w);

      m_Batcher->Draw(texture.m_ShaderRes.Get(),
        position,
        &sourcePos,
        color,
        rotation,
       Vec2( sourcePos.right - sourcePos.left / 2.0f, (sourcePos.bottom - sourcePos.top) / 2.0f),
        scale);
    }

    RenderTarget(ID3D11Device* dev, ID3D11DeviceContext* devCon,
      UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

    void renderText(DirectX::SpriteFont* Font, const std::string& text,
      const Vec2& position = Vec2(0, 0), Color& color = Color(1, 1, 1, 1),
      float rotation = 0.0f,
      const Vec2& origin = Vec2(0, 0),
      const Vec2& scale = Vec2(1.0f, 1.0f),
      DirectX::SpriteEffects effect = DirectX::SpriteEffects_None, float layer = 0.0f);

    void turnBlending(bool isBlend = true);

    void turnDepth(bool isDepth = true);

  protected:
    void setTargetOutput(Tex2D* Target);

  private:
    void createBlendState(ID3D11Device* dev);
    void createDepthStencilandView(ID3D11Device* dev, UINT width, UINT height);
    void createViewport(UINT width, UINT height);
    void createSamplerState(ID3D11Device* dev);
    void createRastState(ID3D11Device* dev);

  protected:
    ID3D11DeviceContext* m_linkedContext;
    ID3D11Device*        m_linkedDevice;

  private:
    bool m_blendEnabled;
    bool m_depthEnabled;

    ID3D11BlendState* m_blendState[BlendStates::NUMSTATES];
    ID3D11DepthStencilState* m_depthStencilState;
    ID3D11DepthStencilView* m_depthStencilView;
    ID3D11RasterizerState*  m_rastState;
    ID3D11SamplerState*     m_sampler;

    std::unique_ptr<DirectX::SpriteBatch> m_Batcher;

    std::unique_ptr<Tex2D> m_Target;
    D3D11_VIEWPORT         m_viewport;
  };
}