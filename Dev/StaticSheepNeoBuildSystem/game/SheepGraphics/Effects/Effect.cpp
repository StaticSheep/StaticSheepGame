/*****************************************************************
Filename: Effect.cpp
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "precompiled.h"
#include<iostream>
#include<fstream>
#include "Effect.h"

namespace DirectSheep
{
  Effect::~Effect() 
  {
    SafeRelease(m_pShader);

    if (m_pShaderData)
      delete m_pShaderData;

    SafeRelease(m_vShader);

    if (m_vShaderData)
      delete m_vShaderData;

    SafeRelease(m_inputLayout);
  }

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