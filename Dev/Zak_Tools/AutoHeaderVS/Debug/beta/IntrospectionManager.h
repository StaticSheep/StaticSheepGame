/*****************************************************************
Filename: IntrospectionManager.h
Project: 

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
  {
  public:
    {
      static IntrospectionManager instance;
      return &instance;
    }

    template <typename T>
    template <typename T>


  private:
    std::hash_map<std::string, TypeInfo*> m_typeMap;

  };

  template <typename T>
  {
    static TypeInfo instance;
    return &instance;
  }

  template <typename T>
  struct HasDefaultCtor
  {
    template <typename U>
    template <typename U>

  };

  template <typename T>
  struct HasCopyCtor
  {

    template <typename U>
    template <typename U>

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
  {
    {
      return v;
    };
  };

  template <class T, T v>
  struct SetFunc<false, T, v>
  {
    {
      return NULL;
    };
  };

  template <typename T>
  {
    typeInfo->m_isPOD = isPOD;

    {
      typeInfo->New = PODNew<T>;
      typeInfo->Copy = PODCopy<T>;
      typeInfo->NewCopy = PODNewCopy<T>;
      typeInfo->Delete = PODDelete<T>;
      typeInfo->PlacementNew = PODPlacementNew<T>;
      typeInfo->PlacementDelete = PODPlacementDelete<T>;
      typeInfo->PlacementCopy = PODPlacementCopy<T>;
    }
    else
    {
      typedef std::conditional<HasDefaultCtor<T>::value, T, EmptyType<T>>::type new_type;
      typeInfo->Delete = Delete<T>;
      typeInfo->PlacementDelete = PlacementDelete<T>;
    }

    typeInfo->m_metatable = mt;
    m_typeMap[name] = typeInfo;
    return typeInfo;
  }
}




