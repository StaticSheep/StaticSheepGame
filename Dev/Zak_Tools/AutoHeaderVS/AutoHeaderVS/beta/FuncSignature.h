/*****************************************************************
Filename: FuncSignature.h
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

    // One argument



    // Static functions with no return value -----------------------------------------------


    // One argument





    // Function with a return value, non - const ------------------------------------------------


    // One argument


    // Function with no return value, non-const ----------------------------------------------------------------




    // Function with a return value, const ------------------------------------------------


    // One argument


    // Function with no return value, const ----------------------------------------------------------------




  private:
    const TypeInfo* m_ret;
    const TypeInfo** m_args;
    const TypeInfo* m_context;

    unsigned int m_argCount;
  };


}
