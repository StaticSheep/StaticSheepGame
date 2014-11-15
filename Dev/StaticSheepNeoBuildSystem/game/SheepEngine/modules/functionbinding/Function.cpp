/*****************************************************************
Filename: Function.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
namespace Framework
{
  Function::Function() :m_sig(), m_callHelper(nullptr), m_context() {}

  Function::Function(const Function& rhs) : m_sig(rhs.m_sig), m_callHelper(rhs.m_callHelper), m_context() {}

  Function::Function(void (*fn)(void), void (*helper)(Variable*, void*, Variable*, size_t))
    :m_sig(fn), m_callHelper(helper)
  {

  }

  void Function::Clear()
  {
    m_callHelper = nullptr;
    m_sig = FunctionSignature();
    m_context = Variable();
  }

  // Get the owner
  Variable& Function::Context()
  {
    return m_context;
  }

  const Variable& Function::Context() const
  {
    return m_context;
  }

  // If it is a class function (has an owner) then it is a method
  bool Function::IsMethod() const
  {
    return m_sig.GetContext() ? true : false;
  }

  Function& Function::operator=(const Function& rhs)
  {
    m_sig = rhs.m_sig;
    m_callHelper = rhs.m_callHelper;

    return *this;
  }

  void Function::operator()(Variable& ret) const
  {
    m_callHelper(&ret, m_context.GetData(), nullptr, 0);
  }

  void Function::operator()() const
  {
    m_callHelper(nullptr, m_context.GetData(), nullptr, 0);
  }

  void Function::operator()(Variable& ret, Variable* args, size_t argCount) const
  {
    m_callHelper(&ret, m_context.GetData(), args, argCount);
  }

  void Function::operator()(Variable& ret, Variable* args, size_t argCount)
  {
    m_callHelper(&ret, m_context.GetData(), args, argCount);
  }


}