#include "precompiled.h"
#include "Effect.h"

namespace DirectSheep
{
  Effect::~Effect() {}

  Effect::Effect(ID3D11Device* pDevice,
    std::string vShaderName,
    std::string vShaderEntry,
    std::string vShaderTarget,
    std::string pShaderName,
    std::string pShaderEntry,
    std::string pShaderTarget)
  {
    compileShaders(pDevice,
             vShaderName, vShaderEntry, vShaderTarget,
             pShaderName, pShaderEntry, pShaderTarget);
  }

  void Effect::compileShaders(ID3D11Device* pDevice,
    std::string vShaderName,
    std::string vShaderEntry,
    std::string vShaderTarget,
    std::string pShaderName,
    std::string pShaderEntry,
    std::string pShaderTarget)
  {
    std::wstring wvShader(vShaderName.begin(), vShaderName.end());
    std::wstring wpShader(pShaderName.begin(), pShaderName.end());
    int compFlags;
#if defined (_DEBUG)
    compFlags |= D3DCOMPILE_DEBUG;
#endif

    DXVerify(D3DCompileFromFile(wvShader.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, vShaderEntry.c_str(), vShaderTarget.c_str(), compFlags, 0, &m_vShaderBlob, NULL));


    DXVerify(pDevice->CreateVertexShader(m_vShaderBlob->GetBufferPointer(), m_vShaderBlob->GetBufferSize(),
      NULL, &m_vShader));

    DXVerify(D3DCompileFromFile(wpShader.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, pShaderEntry.c_str(), pShaderTarget.c_str(), compFlags, 0, &m_pShaderBlob, NULL));

    DXVerify(pDevice->CreatePixelShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(),
      NULL, &m_pShader));

  }
}