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

    void bind(ID3D11DeviceContext* pContext);

    void bindMatrices(ID3D11DeviceContext* pContext,
      const Mat4& proj,
      const Mat4& view,
      const Mat4& world);

    void bindUVCOL(ID3D11DeviceContext* pContext,
      const Vec2 uvBegin, Vec2 uvEnd, Vec4 Color);

    GenEffect(ID3D11Device* pDevice);

  private:
    void createInputLayout(ID3D11Device* pDevice);


  private:
    CBuffer<UV_ColBuffer>* m_uvColBuffer;
    CBuffer<MatBuffer>*   m_matBuffer;

  };
}