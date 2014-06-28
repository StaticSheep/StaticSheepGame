/*****************************************************************
Filename: Variable.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  class TypeInfo;

  class Variable
  {
  public:
    Variable();
    Variable(const TypeInfo *typeInfo, void *data);
    Variable(const Variable& rhs);
    template <typename T>
    Variable(const T& rhs);
    template <typename T>
    Variable(const T* rhs);
    template <typename T>
    Variable(T* rhs);

    void *GetData(void) const;
    const TypeInfo *GetTypeInfo(void) const;
    bool IsValid(void) const;

    template <typename T>
    T& GetValue(void);
    template <typename T>
    const T& GetValue(void) const;

    void PlacementNew(void);
    void PlacementDelete(void);
    void Delete(void);
    void Serialize(File& file) const;
    void Deserialize(File& file);
    Variable& operator=(const Variable& rhs);

  private:
    void *m_data;
    const TypeInfo *m_typeInfo;
  };

  template <typename T>
  Variable::Variable(const T& rhs)
    : m_data((T* )&rhs)
    , m_typeInfo(GET_TYPE(T))
  {
  }

  template <typename T>
  Variable::Variable(const T* rhs)
    : m_data((T* )rhs)
    , m_typeInfo(GET_TYPE(T* ))
  {
  }

  template <typename T>
  Variable::Variable(T* rhs)
    : m_data((T* )rhs)
    , m_typeInfo(GET_TYPE(T* ))
  {
  }

  // Cast helper exists to properly handle pointer types
  template <typename T>
  struct CastHelper
  {
    static T& Cast(void *& data)
    {
      return *(T* &)(data);
    }
  };

  template <typename T>
  struct CastHelper<T* >
  {
    static T* & Cast(void *& data)
    {
      return (T* &)data;
    }
  };

  template <typename T>
  T& Variable::GetValue(void)
  {
    assert(GET_TYPE(T) == m_typeInfo);
    return CastHelper<T>::Cast(m_data);
  }

  template <typename T>
  const T& Variable::GetValue(void) const
  {
    return CastHelper<T>::Cast(m_data);
  }

}


