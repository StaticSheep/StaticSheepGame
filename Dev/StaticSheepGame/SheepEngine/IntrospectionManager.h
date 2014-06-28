/*****************************************************************
Filename: IntrospectionManager.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
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

  void EngineTypeRegistration(void);

  template <typename T>
  struct EmptyType
  {
  };

  class IntrospectionManager
  {
  public:
    static IntrospectionManager *Get( void )
    {
      static IntrospectionManager instance;
      return &instance;
    }

    template <typename T>
    const TypeInfo *GetType( void ) const;
    const TypeInfo *GetType( const char *typeName ) const;
    template <typename T>
    const TypeInfo *RegisterType( unsigned size, const char *name, bool isPOD );

  private:
    std::hash_map<std::string, TypeInfo*> m_typeMap;

    IntrospectionManager( );
    ~IntrospectionManager( );
  };

  template <typename T>
  const TypeInfo *IntrospectionManager::GetType( void ) const
  {
    static TypeInfo instance;
    return &instance;
  }

  template <typename T>
  const TypeInfo *IntrospectionManager::RegisterType( unsigned size, const char *name, bool isPOD )
  {
    TypeInfo *typeInfo = (TypeInfo *)GetType<T>( );
    typeInfo->Init( name, size );
    typeInfo->m_isPOD = isPOD;

    m_typeMap[name] = typeInfo;
    return typeInfo;
  }
}