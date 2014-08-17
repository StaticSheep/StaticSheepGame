/*****************************************************************
Filename: IntrospectionManager.h
*****************************************************************/

#pragma once
#include <string>
#include "TypeInfo.h"
#include <hash_map>


namespace Framework
{
  typedef signed char	int8;
  typedef signed short int16;
  typedef signed int int32;

  

  template <typename T>
  struct EmptyType
  {
    // ...
  };

  class IntrospectionManager


  private:
    std::hash_map<std::string, TypeInfo*> m_typeMap;

  };


  template <typename T>
  struct HasDefaultCtor
  {

  };

  template <typename T>
  struct HasCopyCtor
  {


  };

  template <bool B, typename T = void>
  struct disable_if {
    typedef T type;
  };

  template <typename T>
  struct disable_if<true,T> {
  };

  template <bool, class T, T v>
  struct SetFunc
  };

  template <class T, T v>
  struct SetFunc<false, T, v>
  };

}




