/*****************************************************************
Filename: Variable.h
Project: 
Author(s): Zachary Nawar (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  namespace AntTweak
  {
    class TBar;
  }

  class TypeInfo;

  class Variable
  {
  public:
    // Default Variable constructor
    Variable();
    // Creates a variable, from a Type, and data pointer
    Variable(const TypeInfo* typeInfo, void* data);
    // Creates a variable from a Type, and const data pointer
    Variable(const TypeInfo* typeInfo, const void* data);
    // Creates a variable from another variable
    Variable(const Variable& rhs);
    // Creates a variable from a reference of a type
    template <typename T>
    Variable(const T& rhs);
    // Creates a variable from a const-pointer to a type
    template <typename T>
    Variable(const T* rhs);
    // Creates a variable from a pointer to a type
    template <typename T>
    Variable(T* rhs);

    void* GetData(void) const;
    const TypeInfo* GetTypeInfo(void) const;
    bool IsValid(void) const;

    template <typename T>
    void SetData(T data);

    template <typename T>
    T& GetValue(void);
    template <typename T>
    const T& GetValue(void) const;

    void ToLua(lua_State* L) const;
    void FromLua(lua_State* L, int index);
    
    void Tweak(AntTweak::TBar* bar, const char* tempLabel, const char* label);

    void PlacementNew(void);
    void PlacementDelete(void);
    void Delete(void);
    void Serialize(File& file) const;
    void Deserialize(File& file);
    Variable& operator=(const Variable& rhs);

  private:
    void* m_data;
    const TypeInfo *m_typeInfo;
  };

  template <typename T>
  void Variable::SetData(T data)
  {
    *((T*)m_data) = data; 
  }

  template <typename T>
  Variable::Variable(const T& rhs) : m_data((T* )&rhs), m_typeInfo(GET_TYPE(T))
  {
  }

  template <typename T>
  Variable::Variable(const T* rhs) : m_data((T* )rhs), m_typeInfo(GET_TYPE(T* ))
  {
  }

  template <typename T>
  Variable::Variable(T* rhs) : m_data((T* )rhs), m_typeInfo(GET_TYPE(T* ))
  {
  }

  // Cast helper exists to properly handle pointer types
  template <typename T>
  struct CastHelper
  {
    static T& Cast(void *& data) { return *(T* &)(data); }
  };

  template <typename T>
  struct CastHelper<T* >
  {
    static T* & Cast(void *& data) { return (T* &)data; }
  };

  template <typename T>
  T& Variable::GetValue(void)
  {
    return CastHelper<T>::Cast(m_data);
  }

  template <typename T>
  const T& Variable::GetValue(void) const
  {
    return CastHelper<T>::Cast(m_data);
  }

}

