/*****************************************************************
Filename: FuncSignature.h
Project: 

*****************************************************************/

#pragma once

namespace Framework
{
  class FunctionSignature
  {
  public:
    

    
    // god help me

    // Static functions with a return value ---------------------------------------

    // no args
    template<typename R>

    // One argument
    template<typename R, typename A1>
    {
      m_args = args;
    }

    template<typename R, typename A1, typename A2>
    {
      m_args = args;
    }

    template<typename R, typename A1, typename A2, typename A3>
    {
      m_args = args;
    }

    // Static functions with no return value -----------------------------------------------


    // One argument
    template<typename A1>
    {
      m_args = args;
    }

    template<typename A1, typename A2>
    {
      m_args = args;
    }

    template<typename A1, typename A2, typename A3>
    {
      m_args = args;
    }

    template<typename A1, typename A2, typename A3, typename A4>
    {
      m_args = args;
    }

    template<typename A1, typename A2, typename A3, typename A4, typename A5>
    {
      m_args = args;
    }

    // Function with a return value, non - const ------------------------------------------------

    template <typename C, typename R>

    // One argument
    template <typename C, typename R, typename A1>
    {
      m_args = args;
    }


    // Function with no return value, non-const ----------------------------------------------------------------

    template <typename C>

    template <typename C, typename A1>
    {
      m_args = args;
    }

    template <typename C, typename A1, typename A2>
    {
      m_args = args;
    }

    // Function with a return value, const ------------------------------------------------

    template <typename C, typename R>

    // One argument
    template <typename C, typename R, typename A1>
    {
      m_args = args;
    }


    // Function with no return value, const ----------------------------------------------------------------

    template <typename C>

    template <typename C, typename A1>
    {
      m_args = args;
    }


  private:
    const TypeInfo* m_ret;
    const TypeInfo** m_args;
    const TypeInfo* m_context;

    unsigned int m_argCount;
  };


}
