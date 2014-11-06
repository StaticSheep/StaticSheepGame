#pragma once
#include "DirectXIncludes.h"

#define DXVerify(exp) if(FAILED(exp)){const DWORD size = 100+1; WCHAR buffer[size]; \
					  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL, \
					  exp,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)buffer,size,NULL); \
					  ErrorIf(true, "sheep_grahics", "DX Error %s", buffer);}

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
