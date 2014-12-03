#pragma once
#include "DirectXIncludes.h"
namespace DirectSheep
{
  class Effect
  {
  public:

    Effect(ID3D11Device* pDevice,
      std::string vShaderName,
      std::string pShaderName);


  protected:


  private:
    void compileShaders(ID3D11Device* pDevice,
      std::string& vShaderName,
      std::string& pShaderTarget);

  protected:
    ID3D11InputLayout*   m_inputLayout;
    ID3D11VertexShader*  m_vShader;
    ID3D11PixelShader*   m_pShader;
    void*                m_vShaderData;
    size_t               m_vShaderSize;
    void*                m_pShaderData;
    size_t               m_pShaderSize;
  };
}
