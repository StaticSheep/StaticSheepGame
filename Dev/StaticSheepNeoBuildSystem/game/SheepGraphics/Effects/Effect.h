/*****************************************************************
Filename: Effect.h
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
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
    ID3D11PixelShader*   m_pShader;
    void*                m_vShaderData;
    size_t               m_vShaderSize;
    void*                m_pShaderData;
    size_t               m_pShaderSize;
    ID3DBlob*            m_pShaderBlob;
    ID3DBlob*            m_vShaderBlob;
  };
}