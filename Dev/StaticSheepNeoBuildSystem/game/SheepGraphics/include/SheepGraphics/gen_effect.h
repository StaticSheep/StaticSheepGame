#pragma once

#include "CBuffer.h"
#include "CBuffer_types.h"
#include "Effect.h"
#include "Light.h"
#include "Vertices.h"
namespace DirectSheep
{
  class GenEffect : public Effect
  {
  public:


    void bindPosUV(ID3D11DeviceContext* pContext,
      const Mat4& proj,
      const Mat4& view,
      const Mat4& world,
      const Vec2& uvBegin,
      const Vec2& uvEnd);

    void bindAmbient(ID3D11DeviceContext* pContext,
      const Vec4 Ambience, float intesity);


  private:


  private:
    CBuffer<PosUV>*       m_posUV;
    CBuffer<Ambient>*     m_ambient;

  };
}
