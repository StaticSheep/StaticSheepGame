#include "precompiled.h"
#include<iostream>
#include<fstream>
#include "Effect.h"

namespace DirectSheep
{
  Effect::~Effect() {}

  Effect::Effect(ID3D11Device* pDevice,
    std::string vShaderName,
    std::string pShaderName) : m_inputLayout(NULL), m_pShader(NULL), m_vShader(NULL)
  {
    compileShaders(pDevice,
             vShaderName,
             pShaderName);
  }

  void Effect::compileShaders(ID3D11Device* pDevice,
    std::string& vShaderName,
    std::string& pShaderName)
  {
    /*ID3DBlob* error = NULL;
    std::wstring wvShader(vShaderName.begin(), vShaderName.end());
    std::wstring wpShader(pShaderName.begin(), pShaderName.end());
    int compFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#if defined (_DEBUG)
    compFlags |= D3DCOMPILE_DEBUG;
#endif

    D3DCompileFromFile(wvShader.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, vShaderEntry.c_str(), vShaderTarget.c_str(), compFlags, NULL, &m_vShaderBlob, &error);

    ErrorIf(error, "VShader", "Failed to compile shader: %s", (const char *)error->GetBufferPointer());

    DXVerify(pDevice->CreateVertexShader(m_vShaderBlob->GetBufferPointer(), m_vShaderBlob->GetBufferSize(),
      NULL, &m_vShader));

    D3DCompileFromFile(wpShader.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, pShaderEntry.c_str(), pShaderTarget.c_str(), compFlags, NULL, &m_pShaderBlob, &error);

    ErrorIf(error, "PShader", "Failed to compile shader: %s", (const char *)error->GetBufferPointer());

    DXVerify(pDevice->CreatePixelShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(),
      NULL, &m_pShader));*/

    std::ifstream s_stream;

    s_stream.open(vShaderName, std::ifstream::in | std::ifstream::binary);
    if (s_stream.good())
    {
      s_stream.seekg(0, std::ios::end);
      m_vShaderSize = size_t(s_stream.tellg());
      m_vShaderData = new char[m_vShaderSize];
      s_stream.seekg(0, std::ios::beg);
      s_stream.read((char*)m_vShaderData, m_vShaderSize);
      s_stream.close();
    }

    DXVerify(pDevice->CreateVertexShader(m_vShaderData, m_vShaderSize,
      NULL, &m_vShader));

    s_stream.open(pShaderName, std::ifstream::in | std::ifstream::binary);
    if (s_stream.good())
    {
      s_stream.seekg(0, std::ios::end);
      m_pShaderSize = size_t(s_stream.tellg());
      m_pShaderData = new char[m_pShaderSize];
      s_stream.seekg(0, std::ios::beg);
      s_stream.read((char*)m_pShaderData, m_pShaderSize);
      s_stream.close();
    }

    DXVerify(pDevice->CreatePixelShader(m_pShaderData, m_pShaderSize,
      NULL, &m_pShader));

  }
}