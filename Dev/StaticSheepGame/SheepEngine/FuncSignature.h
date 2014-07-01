/*****************************************************************
Filename: FuncSignature.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  class FunctionSignature
  {
  public:
    unsigned int ArgCount() const;
    
    const TypeInfo* GetRet() const;
    const TypeInfo* GetArg(unsigned int i) const;
    const TypeInfo* GetContext() const;

    FunctionSignature();
    FunctionSignature(const FunctionSignature& rhs);
    FunctionSignature& operator=(const FunctionSignature& rhs);
    
    // god help me

    // Static functions with a return value ---------------------------------------

    // no args
    template<typename R>
    FunctionSignature(R (*fn)(void)) : m_ret(GET_TYPE(R)), m_argCount(0), m_context(NULL), m_args(NULL) {}

    // One argument
    template<typename R, typename A1>
    FunctionSignature(R (*fn)(A1)) : m_ret(GET_TYPE(R)), m_argCount(1), m_context(NULL)
    {
      static const TypeInfo* args[] = { GET_TYPE(A1) };
      m_args = args;
    }

    template<typename R, typename A1, typename A2>
    FunctionSignature(R (*fn)(A1, A2)) : m_ret(GET_TYPE(R)), m_argCount(2), m_context(NULL)
    {
      static const TypeInfo* args[] = { GET_TYPE(A1), GET_TYPE(A2) };
      m_args = args;
    }

    template<typename R, typename A1, typename A2, typename A3>
    FunctionSignature(R (*fn)(A1, A2, A3)) : m_ret(GET_TYPE(R)), m_argCount(3), m_context(NULL)
    {
      static const TypeInfo* args[] = { GET_TYPE(A1), GET_TYPE(A2), GET_TYPE(A3) };
      m_args = args;
    }

    // Static functions with no return value -----------------------------------------------

    FunctionSignature( void (*)( void ) );

    // One argument
    template<typename A1>
    FunctionSignature(void (*fn)(A1)) : m_ret(NULL), m_argCount(1), m_context(NULL)
    {
      static const TypeInfo* args[] = { GET_TYPE(A1) };
      m_args = args;
    }

    template<typename A1, typename A2>
    FunctionSignature(void (*fn)(A1, A2)) : m_ret(NULL), m_argCount(2), m_context(NULL)
    {
      static const TypeInfo* args[] = { GET_TYPE(A1), GET_TYPE(A2) };
      m_args = args;
    }

    // Function with a return value, non - const ------------------------------------------------

    template <typename C, typename R>
    FunctionSignature( R (C::*)(void)) : m_ret(GET_TYPE(R)), m_argCount(0), m_context(GET_TYPE(C)), m_args(NULL) {}

    // One argument
    template <typename C, typename R, typename A1>
    FunctionSignature( R (C::*)(A1)) : m_ret(GET_TYPE(R)), m_argCount(1), m_context(GET_TYPE(C))
    {
      static const TypeInfo* args[] = { GET_TYPE(A1) };
      m_args = args;
    }


    // Function with no return value, non-const ----------------------------------------------------------------

    template <typename C>
    FunctionSignature( void (C::*)(void) ) : m_ret(NULL), m_argCount(0), m_context(GET_TYPE(C)), m_args(NULL) {}

    template <typename C, typename A1>
    FunctionSignature( void (C::*)(A1) ) : m_ret(NULL), m_argCount(1), m_context(GET_TYPE(C))
    {
      static const TypeInfo* args[] = { GET_TYPE(A1) };
      m_args = args;
    }

    // Function with a return value, const ------------------------------------------------

    template <typename C, typename R>
    FunctionSignature( R (C::*)(void) const) : m_ret(GET_TYPE(R)), m_argCount(0), m_context(GET_TYPE(C)), m_args(NULL) {}

    // One argument
    template <typename C, typename R, typename A1>
    FunctionSignature( R (C::*)(A1) const) : m_ret(GET_TYPE(R)), m_argCount(1), m_context(GET_TYPE(C))
    {
      static const TypeInfo* args[] = { GET_TYPE(A1) };
      m_args = args;
    }


    // Function with no return value, const ----------------------------------------------------------------

    template <typename C>
    FunctionSignature( void (C::*)(void) const) : m_ret(NULL), m_argCount(0), m_context(GET_TYPE(C)), m_args(NULL) {}

    template <typename C, typename A1>
    FunctionSignature( void (C::*)(A1) const) : m_ret(NULL), m_argCount(1), m_context(GET_TYPE(C))
    {
      static const TypeInfo* args[] = { GET_TYPE(A1) };
      m_args = args;
    }


  private:
    const TypeInfo* m_ret;
    const TypeInfo** m_args;
    const TypeInfo* m_context;

    unsigned int m_argCount;
  };


}