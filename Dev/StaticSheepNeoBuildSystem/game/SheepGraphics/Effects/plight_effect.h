/*****************************************************************
Filename: plight_effect.h
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
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

    void bindLights(ID3D11DeviceContext* pContext,
      const Light* lights,
      const int numLights,
      Vec4 screenSize);

    PointLight(ID3D11Device* pDevice);

  private:
    void createInputLayout(ID3D11Device* pDevice);


  private:
    CBuffer<LightsBuffer>* m_lightBuffer;
    CBuffer<MatBuffer>*   m_matBuffer;

  };
}