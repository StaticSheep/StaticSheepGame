#pragma once

#include "CBuffer.h"
#include "CBuffer_types.h"
#include "Effect.h"
#include "Light.h"
#include "precompiled.h"
#include "gen_effect.h"
namespace DirectSheep
{
  void GenEffect::bind(ID3D11DeviceContext* pContext)
  {
    pContext->IASetInputLayout(m_inputLayout);
    pContext->VSSetShader(m_vShader, NULL, 0);
    pContext->VSSetConstantBuffers(0, 1, &m_matBuffer->m_CBufferRaw);
    pContext->PSSetShader(m_pShader, NULL, 0);
    pContext->PSSetConstantBuffers(0, 1, &m_uvColBuffer->m_CBufferRaw);
  }

  void GenEffect::bindMatrices(ID3D11DeviceContext* pContext,
    const Mat4& proj,
    const Mat4& view,
    const Mat4& world)
  {
    MatBuffer buf = {
      proj.Transpose(),
      view.Transpose(),
      world.Transpose(),
    };

    m_matBuffer->setData(pContext, buf);
  }

  void GenEffect::bindUVCOL(ID3D11DeviceContext* pContext,
    const Vec2 uvBegin, Vec2 uvEnd, Vec4 Color)
  {
    UV_ColBuffer buf = {
      Color,
      uvBegin,
      uvEnd,
    };

    m_uvColBuffer->setData(pContext, buf);
  }

  GenEffect::GenEffect(ID3D11Device* pDevice) : Effect(pDevice, "content/shaders/VShader.hlsl", "VSMain", "vs_5_0", "content/shaders/PShader.hlsl", "PSMain", "ps_5_0")
  {
    m_uvColBuffer = new CBuffer<UV_ColBuffer>(pDevice);

    m_matBuffer = new CBuffer<MatBuffer>(pDevice);

    createInputLayout(pDevice);
  }

  void GenEffect::createInputLayout(ID3D11Device* pDevice)
  {
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    DXVerify(pDevice->CreateInputLayout(layout, 2, m_vShaderBlob->GetBufferPointer(), m_vShaderBlob->GetBufferSize(), &m_inputLayout));
  }
}