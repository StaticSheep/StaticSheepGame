#pragma once
#include "DirectXIncludes.h"
namespace DirectSheep
{
  class Effect
  {
  public:
    virtual void bind(ID3D11DeviceContext* pDevCon) = 0;

    Effect(ID3D11Device* pDevice,
      std::string vShaderName,
      std::string pShaderName);
    virtual ~Effect();

  protected:

    virtual void createInputLayout(ID3D11Device* pDevice) = 0;

  private:
    void compileShaders(ID3D11Device* pDevice,
      std::string& vShaderName,
      std::string& pShaderTarget);

  protected:
    ID3D11InputLayout*   m_inputLayout;
    ID3D11VertexShader*  m_vShader;
    //ID3DBlob*            m_vShaderBlob;
    ID3D11PixelShader*   m_pShader;
    //ID3DBlob*            m_pShaderBlob;
    void*                m_vShaderData;
    size_t               m_vShaderSize;
    void*                m_pShaderData;
    size_t               m_pShaderSize;
  };
}