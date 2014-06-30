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
    // ...
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
    const TypeInfo* GetType(void) const;
    const TypeInfo* GetType(const char* typeName) const;
    const TypeInfo* GetType(std::string& typeName) const;
    template <typename T>
    const TypeInfo *RegisterType(unsigned int size, const char* name, const char* mt, bool isPOD );

    const std::hash_map<std::string, TypeInfo*>& GetTypeMap() const;

  private:
    std::hash_map<std::string, TypeInfo*> m_typeMap;

    IntrospectionManager( );
    ~IntrospectionManager( );
  };

  template <typename T>
  const TypeInfo* IntrospectionManager::GetType( void ) const
  {
    static TypeInfo instance;
    return &instance;
  }

  template <typename T>
  struct HasDefaultCtor
  {
    template <typename U>
    static int32 SFINAE( decltype( U( ) ) * );
    template <typename U>
    static int8 SFINAE( ... );

    static const bool value = sizeof( SFINAE<T>( NULL ) ) == sizeof( int32 );
  };

  template <typename T>
  struct HasCopyCtor
  {
    static T MakeT( void );

    template <typename U>
    static int32 SFINAE( decltype( U( MakeT( ) ) ) * );
    template <typename U>
    static int8 SFINAE( ... );

    static const bool value = sizeof( SFINAE<T>( NULL ) ) == sizeof( int32 );
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
    static T Get( void )
    {
      return v;
    };
  };

  template <class T, T v>
  struct SetFunc<false, T, v>
  {
    static T Get( void )
    {
      return NULL;
    };
  };

  template <typename T>
  const TypeInfo *IntrospectionManager::RegisterType(unsigned int size, const char* name, const char* mt, bool isPOD)
  {
    TypeInfo *typeInfo = (TypeInfo *)GetType<T>( );
    typeInfo->Init( name, size );
    typeInfo->m_isPOD = isPOD;

    if(isPOD)
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
      typeInfo->New = SetFunc<HasDefaultCtor<T>::value, void *(*)( void ), &New<new_type>>::Get( );
      typeInfo->PlacementNew = SetFunc<HasDefaultCtor<T>::value, void (*)( void * ), &PlacementNew<new_type>>::Get( );
      typeInfo->Copy = SetFunc<HasCopyCtor<T>::value, void (*)( void *, const void * ), &Copy<new_type>>::Get( );
      typeInfo->Delete = Delete<T>;
      typeInfo->PlacementCopy = SetFunc<HasCopyCtor<T>::value, void (*)( void *, const void * ), &PlacementCopy<new_type>>::Get( );
      typeInfo->PlacementDelete = PlacementDelete<T>;
      typeInfo->NewCopy = SetFunc<HasCopyCtor<T>::value, void (*)( void **, const void * ), &NewCopy<new_type>>::Get( );
    }

    typeInfo->m_metatable = mt;
    m_typeMap[name] = typeInfo;
    return typeInfo;
  }
}



