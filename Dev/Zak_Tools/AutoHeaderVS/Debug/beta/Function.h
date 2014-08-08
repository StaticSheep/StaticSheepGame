/*****************************************************************
Filename: Function.h
Project: 

*****************************************************************/

#pragma once

namespace Framework
{

  // Static function with a return value god help ============================================================


  template <typename FuncType, FuncType FuncPtr, typename R>
  {

  }

  template <typename FuncType, FuncType FuncPtr, typename R, typename A1>
  {

  }

  template <typename FuncType, FuncType FuncPtr, typename R, typename A1, typename A2>
  {

  }

  template <typename FuncType, FuncType FuncPtr, typename R, typename A1, typename A2, typename A3>
  {

  }



  // Call static function without a return value ============================================================



  template <typename FuncType, FuncType FuncPtr>
  {
  }

  template <typename FuncType, FuncType FuncPtr, typename A1>
  {
  }


  template <typename FuncType, FuncType FuncPtr, typename A1, typename A2>
  {
  }

  template <typename FuncType, FuncType FuncPtr, typename A1, typename A2, typename A3>
  {
  }

  template <typename FuncType, FuncType FuncPtr, typename A1, typename A2, typename A3, typename A4>
  {
  }

  template <typename FuncType, FuncType FuncPtr, typename A1, typename A2, typename A3, typename A4, typename A5>
  {
  }


  // Function that returns something ============================================================


  template <typename FuncType, FuncType FuncPtr, typename R, typename C>
  {

  }

  template <typename FuncType, FuncType FuncPtr, typename R, typename C, typename A1>
  {

  }



  // Function that returns nothing ============================================================

  template <typename FuncType, FuncType FuncPtr, typename C>
  {

  }

  template <typename FuncType, FuncType FuncPtr, typename C, typename A1>
  {

  }

  template <typename FuncType, FuncType FuncPtr, typename C, typename A1, typename A2>
  {

  }

  //  ============================================================ ============================================================

  class Function
  {
  public:


    // Binding ----------------------
    template<typename C>
    {
      m_context = &c;
    }

    template<typename C>
    {
      m_context = c;
    }

    

    // Static function with return values ===================================================

    template<typename R>

    template<typename R, typename A1>

    template<typename R, typename A1, typename A2>

    template<typename R, typename A1, typename A2, typename A3>

    
    // Static function without a return value ===================================================

    template<typename A1>

    template<typename A1, typename A2>

    template<typename A1, typename A2, typename A3>

    template<typename A1, typename A2, typename A3, typename A4>

    template<typename A1, typename A2, typename A3, typename A4, typename A5>


    // Class methods with return values! non-const ====================================================

    template<typename R, typename C>

    template<typename R, typename C, typename A1>

    // Class methods with OUT return values! non-const ====================================================

    template<typename C>

    template<typename C, typename A1>

    template<typename C, typename A1, typename A2>

    // Class methods with return values! const ====================================================

    template<typename R, typename C>

    template<typename R, typename C, typename A1>

    // Class methods with OUT return values! const ====================================================

    template<typename C>

    template<typename C, typename A1>

    template<typename C, typename A1, typename A2>



    // Operators ======================================================================



    template <typename A1>

    template <typename A1, typename A2>

    template <typename A1, typename A2, typename A3>

    template <typename A1, typename A2, typename A3, typename A4>

    template <typename A1, typename A2, typename A3, typename A4, typename A5>


    template <typename A1>

    template <typename A1, typename A2>

    template <typename A1, typename A2, typename A3>

    template <typename A1, typename A2, typename A3, typename A4>

    template <typename A1, typename A2, typename A3, typename A4, typename A5>


  private:
    Variable m_context;
    FunctionSignature m_sig;
    
    // This is used to make calling the function a less painful process

  };


  // I must hate life


  // STATIC FUNCTIONS WITH RETURN VALUE

  template<typename R>

  template<typename R, typename A1>

  template<typename R, typename A1, typename A2>

  template<typename R, typename A1, typename A2, typename A3>

  // STATIC FUNCTIONS WITHOUT RETURN VALUE

  template<typename A1>

  template<typename A1, typename A2>

  template<typename A1, typename A2, typename A3>

  template<typename A1, typename A2, typename A3, typename A4>

  template<typename A1, typename A2, typename A3, typename A4, typename A5>

  // METHODS WITH RETURN VALUE, NON CONST

  template<typename R, typename C>

  template<typename R, typename C, typename A1>

  // METHODS WITHOUT RETURN VALUE, NON CONST

  template<typename C>

  template<typename C, typename A1>

  template<typename C, typename A1, typename A2>

  // METHODS WITH RETURN VALUE, CONST

  template<typename R, typename C>

  template<typename R, typename C, typename A1>

  // METHODS WITHOUT RETURN VALUE, CONST

  template<typename C>

  template<typename C, typename A1>

  template<typename C, typename A1, typename A2>

  #undef HP
  #undef PE

  // FUNCTION BUILDERS ========================================================================================


  // Static function with return value

  
  {
  }

  {
  }

  {
  }

  template<typename FuncType, FuncType FuncPtr, typename R, typename A1, typename A2, typename A3>
  {
  }

  // Static function without return value

  #undef RV

  template<typename FuncType, FuncType FuncPtr>
  {
  }

  {
  }

  {
  }

  {
  }

  {
  }

  {
  }

  // Methods with return value, non -const

  #undef RV

  {
  }

  {
  }


  // Methods with return value, const

  {
  }

  {
  }


  // methods without return value, non-const

  #undef RV

  {
  }


  template<typename FuncType, FuncType FuncPtr, typename C, typename A1>
  {
  }

  template<typename FuncType, FuncType FuncPtr, typename C, typename A1, typename A2>
  {
  }


  // methods without return value, const

  #undef RV

  {
  }


  {
  }

  {
  }

  #undef TS
  #undef RV

  // CALLING FUNCTIONS =========================================================================================

  // Functions with return value

  template<typename A1>
  {

    Variable argStack[1];


  }

  template<typename A1, typename A2>
  {

    Variable argStack[2];


  }

  template<typename A1, typename A2, typename A3>
  {

    Variable argStack[3];


  }

  // Functions without return value

  template<typename A1>
  {

    Variable argStack[1];


  }

  template<typename A1, typename A2>
  {

    Variable argStack[2];


  }

  template<typename A1, typename A2, typename A3>
  {

    Variable argStack[3];


  }

  template<typename A1, typename A2, typename A3, typename A4>
  {

    Variable argStack[4];


  }

  template<typename A1, typename A2, typename A3, typename A4, typename A5>
  {

    Variable argStack[5];


  }


}
