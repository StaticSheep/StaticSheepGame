/*****************************************************************
Filename: CBuffer.h
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "DirectXIncludes.h"
#include "DXVerify.h"


namespace DirectSheep
{
  template<class T>
  class CBuffer
  {
    friend class Effect;
    friend class PointLight;
    friend class GenEffect;
    friend class PreMultFilter;

  public:
    CBuffer(ID3D11Device* pDevice)
    {
      CD3D11_BUFFER_DESC bufDesc(sizeof(T), D3D11_BIND_CONSTANT_BUFFER,
        D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0, 0);

      DXVerify(pDevice->CreateBuffer(&bufDesc, NULL, &m_CBufferRaw));
    }

    void setData(ID3D11DeviceContext* pDeviceContext, const T& data)
    {
      D3D11_MAPPED_SUBRESOURCE mSR;

      DXVerify(pDeviceContext->Map(m_CBufferRaw, 0, D3D11_MAP_WRITE_DISCARD, 0, &mSR));

      memcpy_s(mSR.pData, sizeof(T), &data, sizeof(T));

      pDeviceContext->Unmap(m_CBufferRaw, 0);
    }

    ~CBuffer() 
    {
      SafeRelease(m_CBufferRaw);
    }

  private:

    CBuffer(const CBuffer& rhs);
    CBuffer& operator=(const CBuffer& rhs);

  private:
    ID3D11Buffer* m_CBufferRaw;
  };
}