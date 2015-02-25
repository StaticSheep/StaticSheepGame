/*****************************************************************
Filename: gen_effect.cpp
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "precompiled.h"
#include "CBuffer.h"
#include "CBuffer_types.h"
#include "Effect.h"
#include "Light.h"
#include "gen_effect.h"
namespace DirectSheep
{
  void GenEffect::bind(ID3D11DeviceContext* pContext)
  {
    pContext->IASetInputLayout(m_inputLayout);
    pContext->VSSetShader(m_vShader, NULL, 0);
    pContext->VSSetConstantBuffers(0, 1, &m_posUV->m_CBufferRaw);
    pContext->PSSetShader(m_pShader, NULL, 0);
    pContext->PSSetConstantBuffers(0, 1, &m_ambient->m_CBufferRaw);
  }

  void GenEffect::bindPosUV(ID3D11DeviceContext* pContext,
    const Mat4& proj,
    const Mat4& view,
    const Mat4& world,
    const Vec2& uvBegin,
    const Vec2& uvEnd)
  {
    PosUV buf = {
      proj.Transpose(),
      view.Transpose(),
      world.Transpose(),
      uvBegin,
      uvEnd,
    };

    m_posUV->setData(pContext, buf);
  }

  void GenEffect::bindAmbient(ID3D11DeviceContext* pContext,
    const Vec4 Ambience, float intensity)
  {
    Ambient buf = {
      Ambience,
      Vec4(intensity,1,1,1),
    };

    m_ambient->setData(pContext, buf);
  }

  GenEffect::GenEffect(ID3D11Device* pDevice)
    : Effect(pDevice, "content/shaders/VShader.cso",
    "content/shaders/PShader.cso")
  {
    m_posUV = new CBuffer<PosUV>(pDevice);

    m_ambient = new CBuffer<Ambient>(pDevice);

    createInputLayout(pDevice);
  }

  void GenEffect::createInputLayout(ID3D11Device* pDevice)
  {
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    DXVerify(pDevice->CreateInputLayout(layout, 2, m_vShaderData, m_vShaderSize, &m_inputLayout));
  }

  GenEffect::~GenEffect()
  {
    if (m_posUV)
      delete m_posUV;

    if (m_ambient)
      delete m_ambient;
  }
}