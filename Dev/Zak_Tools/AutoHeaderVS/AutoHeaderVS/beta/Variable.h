/*****************************************************************
Filename: Variable.h
*****************************************************************/

#pragma once

namespace Framework
{
  class TypeInfo;

  class Variable
  {
  public:






  private:
    void* m_data;

  // Cast helper exists to properly handle pointer types
  template <typename T>
  struct CastHelper
  };

  template <typename T>
  struct CastHelper<T* >
  };



}



