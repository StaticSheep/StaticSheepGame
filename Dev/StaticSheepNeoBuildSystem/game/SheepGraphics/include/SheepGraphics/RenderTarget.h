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










      DirectX::SpriteEffects effect = DirectX::SpriteEffects_None, float layer = 0.0f)
    {

      RECT sourcePos;

      m_Batcher->Draw(texture.m_ShaderRes,
        position,
        &sourcePos,
        color,
        scale);
    }

    RenderTarget(ID3D11Device* dev, ID3D11DeviceContext* devCon,
      UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
      DirectX::SpriteEffects effect = DirectX::SpriteEffects_None, float layer = 0.0f);



  protected:

  private:

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
