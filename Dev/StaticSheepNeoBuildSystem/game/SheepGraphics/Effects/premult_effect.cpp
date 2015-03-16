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
#include "premult_effect.h"


namespace DirectSheep
{
  void PreMultFilter::bind(ID3D11DeviceContext* pContext)
  {
    pContext->IASetInputLayout(m_inputLayout);
    pContext->VSSetShader(m_vShader, NULL, 0);
    pContext->VSSetConstantBuffers(0, 1, &m_buffer->m_CBufferRaw);
    pContext->PSSetShader(m_pShader, NULL, 0);
    //pContext->PSSetConstantBuffers(0, 1, &m_ambient->m_CBufferRaw);
  }

  void PreMultFilter::bindSize(ID3D11DeviceContext* pContext,
    const Vec2& size)
  {
    Mat4 mtx = DirectX::XMMatrixScaling(size.x, size.y, 0);

    PreMultBuffer buf = {
      mtx
    };

    m_buffer->setData(pContext, buf);
  }


  PreMultFilter::PreMultFilter(ID3D11Device* pDevice)
    : Effect(pDevice, "content/shaders/PreMultVS.cso",
    "content/shaders/PreMult.cso")
  {
    m_buffer = new CBuffer<PreMultBuffer>(pDevice);

    //m_ambient = new CBuffer<Ambient>(pDevice);

    createInputLayout(pDevice);
  }

  void PreMultFilter::createInputLayout(ID3D11Device* pDevice)
  {
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    DXVerify(pDevice->CreateInputLayout(layout, 2, m_vShaderData, m_vShaderSize, &m_inputLayout));
  }

  PreMultFilter::~PreMultFilter()
  {
    if (m_buffer)
      delete m_buffer;

    //if (m_ambient)
    //  delete m_ambient;
  }
}