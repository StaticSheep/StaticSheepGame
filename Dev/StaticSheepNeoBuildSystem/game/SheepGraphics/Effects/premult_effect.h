/*****************************************************************
Filename: gen_effect.h
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
  class PreMultFilter : public Effect
  {
  public:

    void bind(ID3D11DeviceContext* pContext);

    void bindSize(ID3D11DeviceContext* pContext,
      const Vec2& size);


    PreMultFilter(ID3D11Device* pDevice);
    ~PreMultFilter();

  private:
    void createInputLayout(ID3D11Device* pDevice);


  private:
    CBuffer<PreMultBuffer>*       m_buffer;
    //CBuffer<Ambient>*     m_ambient;

  };
}