/*****************************************************************
Filename: TypeInfo.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once
#include <vector>

namespace Framework
{
  class TypeInfo;

  class Member
  {
  public:
    const TypeInfo *Type( void ) const;
    unsigned Offset( void ) const;
    const char *Name( void ) const;

  private:
    const char *m_name;
    unsigned m_offset;
    const TypeInfo *m_typeInfo;

    friend class TypeInfo;
  };

  typedef void (*SerializeCB)(File&, Variable);
  typedef void (*DeserializeCB)(File&, Variable);

  class TypeInfo
  {
  public:
    TypeInfo( );
    void Init( const char *name, unsigned size );
    void AddMember( const TypeInfo *typeInfo, const char *name, unsigned offset );
    const Member *GetMember( const char *memberName ) const;

    void SetSerializer(SerializeCB cb);
    void SetDeserializer(DeserializeCB cb);

    void Serialize(File& file, Variable var) const;
    void Deserialize(File& file, Variable var) const;



    unsigned Size( void ) const;
    const char *Name( void ) const;
    inline bool IsPOD( void ) const { return m_isPOD; };

  private:
    unsigned m_size;
    std::string m_name;
    std::vector<Member> m_members;

    SerializeCB m_serialize;
    DeserializeCB m_deserialize;
    bool m_isPOD;

    friend class Serializer;
    friend class IntrospectionManager;
    friend class Factory;
  };

}