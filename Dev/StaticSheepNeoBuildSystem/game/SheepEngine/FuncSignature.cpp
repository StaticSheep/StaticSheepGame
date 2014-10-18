/*****************************************************************
Filename: FuncSignature.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "precompiled.h"
#include "FuncSignature.h"

namespace Framework
{
  FunctionSignature::FunctionSignature() : m_ret(NULL), m_args(NULL), m_argCount(0) {}

  FunctionSignature::FunctionSignature(const FunctionSignature& rhs)
    : m_ret(rhs.m_ret), m_args(rhs.m_args), m_argCount(rhs.m_argCount) {}

  FunctionSignature& FunctionSignature::operator=(const FunctionSignature& rhs)
  {
    m_ret = rhs.m_ret;
    m_args = rhs.m_args;
    m_argCount = rhs.m_argCount;
    m_context = rhs.m_context;

    return *this;
  }

  FunctionSignature::FunctionSignature(void (*)( void )) : m_ret(NULL), m_args(NULL), m_argCount(0), m_context(NULL) {}

  unsigned int FunctionSignature::ArgCount() const
  {
    return m_argCount;
  }

  const TypeInfo* FunctionSignature::GetRet() const
  {
    return m_ret;
  }

  const TypeInfo* FunctionSignature::GetArg(unsigned int i) const
  {
    return m_args[i];
  }

  const TypeInfo* FunctionSignature::GetContext() const
  {
    return m_context;
  }

  const FunctionSignature* Function::Signature() const
  {
    return &m_sig;
  }

}