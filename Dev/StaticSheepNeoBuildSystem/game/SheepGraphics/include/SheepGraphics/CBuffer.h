#pragma once
#include "DirectXIncludes.h"

namespace DirectSheep
{
  template<class T>
  class CBuffer
  {
    friend class Effect;
    friend class PointLight;


  private:


  private:
    ID3D11Buffer* m_CBufferRaw;
  };
}
