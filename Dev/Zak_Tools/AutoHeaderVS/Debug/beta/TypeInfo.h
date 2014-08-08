/*****************************************************************
Filename: TypeInfo.h
Project: 

*****************************************************************/

#pragma once
#include <vector>

namespace Framework
{
  class TypeInfo;

  class Member
  {
  public:

  private:
    const char *m_name;
    unsigned m_offset;
    const TypeInfo *m_typeInfo;

    friend class TypeInfo;
  };

  // Serialization

  // Lua

  class TypeInfo
  {
  public:




    // Lua stuff





  private:
    unsigned m_size;
    std::string m_name;

    std::vector<Member> m_members;

    const char* m_metatable;

    SerializeCB m_serialize;
    DeserializeCB m_deserialize;

    ToLuaCB m_toLua;
    FromLuaCB m_fromLua;

    bool m_isPOD;

    friend class Serializer;
    friend class IntrospectionManager;
    friend class Factory;
  };

  template <typename T>
  {
  }

  template <typename T>
  {
  }

  template <typename T>
  {
    return data;
  }

  template <typename T>
  {
  }

  template <typename T>
  {
  }

  template <typename T>
  {
  }

  template <typename T>
  {
  }

  template <typename T>
  {
  }

  template <typename T>
  {
  }

  template <typename T>
  {
  }

  template <typename T>
  {
    *dest = newData;
  }

  template <typename T>
  {
  }

  template <typename T>
  {
  }

  template <typename T>
  {
  }

  template <typename T>
  {
  }

    template <typename T>
  {
  }

}
