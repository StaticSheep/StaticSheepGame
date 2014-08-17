/*****************************************************************
Filename: Function.h
*****************************************************************/

#pragma once

namespace Framework
{

  // Static function with a return value god help ============================================================








  // Call static function without a return value ============================================================











  // Function that returns something ============================================================






  // Function that returns nothing ============================================================




  //  ============================================================ ============================================================

  class Function
  {
  public:


    // Binding ----------------------


    bool IsMethod() const; // Method = class function

    // Static function with return values ===================================================





    
    // Static function without a return value ===================================================







    // Class methods with return values! non-const ====================================================



    // Class methods with OUT return values! non-const ====================================================




    // Class methods with return values! const ====================================================



    // Class methods with OUT return values! const ====================================================






    // Operators ======================================================================















  private:
    Variable m_context;
    FunctionSignature m_sig;
    
    // This is used to make calling the function a less painful process

  };


  // I must hate life

  #define HP void (*helper)(Variable*, void*, Variable*, size_t)
  #define PE :m_sig(fn), m_callHelper(helper) {}

  // STATIC FUNCTIONS WITH RETURN VALUE





  // STATIC FUNCTIONS WITHOUT RETURN VALUE






  // METHODS WITH RETURN VALUE, NON CONST



  // METHODS WITHOUT RETURN VALUE, NON CONST




  // METHODS WITH RETURN VALUE, CONST



  // METHODS WITHOUT RETURN VALUE, CONST




  #undef HP
  #undef PE

  // FUNCTION BUILDERS ========================================================================================


  // Static function with return value



  // Methods with return value, non -const




  // methods without return value, non-const

  #undef RV





  // methods without return value, const


  #undef TS
  #undef RV

  // CALLING FUNCTIONS =========================================================================================

  // Functions with return value




  // Functions without return value







}
