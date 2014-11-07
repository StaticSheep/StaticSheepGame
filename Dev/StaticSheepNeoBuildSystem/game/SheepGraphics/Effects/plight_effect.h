#pragma once

#include "CBuffer.h"
#include "CBuffer_types.h"
#include "Effect.h"
#include "Light.h"
#include "Vertices.h"
namespace DirectSheep
{
  class PointLight : public Effect
  {
  public:

    void bind(ID3D11DeviceContext* pContext);

    void bindMatrices(ID3D11DeviceContext* pContext,
      const Mat4& proj,
      const Mat4& view,
      const Mat4& world);

    void bindLight(ID3D11DeviceContext* pContext,
      const Light& light);

    PointLight(ID3D11Device* pDevice);

  private:
    void createInputLayout(ID3D11Device* pDevice);


  private:
    CBuffer<LightBuffer>* m_lightBuffer;
    CBuffer<MatBuffer>*   m_matBuffer;

  };
}