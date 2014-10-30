#pragma once
#include "DirectXIncludes.h"
namespace DirectSheep
{
  class Effect
  {
  public:

    Effect(ID3D11Device* pDevice,
      std::string vShaderName,
      std::string vShaderEntry,
      std::string vShaderTarget,
      std::string pShaderName,
      std::string pShaderEntry,
      std::string pShaderTarget);

  protected:


  private:
    void compileShaders(ID3D11Device* pDevice,
      std::string vShaderName,
      std::string vShaderEntry,
      std::string vShaderTarget,
      std::string pShaderName,
      std::string pShaderEntry,
      std::string pShaderTarget);

  protected:
    ID3D11InputLayout*   m_inputLayout;
    ID3D11VertexShader*  m_vShader;
    ID3DBlob*            m_vShaderBlob;
    ID3D11PixelShader*   m_pShader;
    ID3DBlob*            m_pShaderBlob;
  };
}
