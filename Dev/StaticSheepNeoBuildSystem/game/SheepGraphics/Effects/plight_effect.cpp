/*****************************************************************
Filename: plight_effect.cpp
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "precompiled.h"
#include "plight_effect.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace DirectSheep
{
  void PointLight::bind(ID3D11DeviceContext* pContext)
  {
    pContext->IASetInputLayout(m_inputLayout);
    pContext->VSSetShader(m_vShader, NULL, 0);
    pContext->VSSetConstantBuffers(0, 1, &m_matBuffer->m_CBufferRaw);
    pContext->PSSetShader(m_pShader, NULL, 0);
    pContext->PSSetConstantBuffers(0, 1, &m_lightBuffer->m_CBufferRaw);
  }

  void PointLight::bindMatrices(ID3D11DeviceContext* pContext,
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

  void PointLight::bindLights(ID3D11DeviceContext* pContext,
    const Light* lights,
    const int numLights)
  {
    D3D11_MAPPED_SUBRESOURCE mapResource;

    pContext->Map(m_lightBuffer->m_CBufferRaw, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);

    LightsBuffer* data = reinterpret_cast<LightsBuffer*>(mapResource.pData);

    for (int i = 0; i < numLights; ++i)
    {
      data->atten[i] = Vec4(lights[i].getAttenuation());
      data->col[i] = lights[i].getColor();
      data->pos[i] = Vec4(lights[i].getPosition());
    }
    data->numLights = numLights;

    pContext->Unmap(m_lightBuffer->m_CBufferRaw, 0);
  }

  PointLight::PointLight(ID3D11Device* pDevice) : Effect(pDevice, "content/shaders/light_vs.cso", "content/shaders/light_ps.cso")
  {
    m_lightBuffer = new CBuffer<LightsBuffer>(pDevice);

    m_matBuffer = new CBuffer<MatBuffer>(pDevice);

    createInputLayout(pDevice);
  }

  void PointLight::createInputLayout(ID3D11Device* pDevice)
  {
    D3D11_INPUT_ELEMENT_DESC layout[1] = 
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    DXVerify(pDevice->CreateInputLayout(layout, 1, m_vShaderData, m_vShaderSize, &m_inputLayout));
  }
}