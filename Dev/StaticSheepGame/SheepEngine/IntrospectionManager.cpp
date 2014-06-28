/*****************************************************************
Filename: IntrospectionManager.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "IntrospectionManager.h"
#include <vector>
#include <string>

namespace Framework
{
  const TypeInfo* IntrospectionManager::GetType(const char* typeName) const
  {
    std::string typeNameStr = typeName;

    if (m_typeMap.find(typeNameStr) != m_typeMap.end())
    {
      return m_typeMap.at(typeNameStr);
    }

    return nullptr;
  }

  IntrospectionManager::IntrospectionManager( )
  {
  }

  IntrospectionManager::~IntrospectionManager( )
  {
  }


#define REGISTER_POD_FULL( TYPE ) \
  TYPE_REGISTER_POD( TYPE ); \
  TYPE_SET_SERIALIZER( TYPE, SerializePOD<TYPE> );\
  TYPE_SET_DESERIALIZER( TYPE, DeserializePOD<TYPE> )

  void EngineTypeRegistration()
  {
    REGISTER_POD_FULL( int );

    REGISTER_POD_FULL( unsigned int );

    REGISTER_POD_FULL( bool );

    REGISTER_POD_FULL( float );

    REGISTER_POD_FULL( double );

    TYPE_REGISTER( std::vector<int> );
    TYPE_SET_SERIALIZER( std::vector<int>, SerializeArray<int> );
    TYPE_SET_DESERIALIZER( std::vector<int>, DeserializeArray<int> );

    TYPE_REGISTER( std::string );
    TYPE_SET_SERIALIZER( std::string, SerializeString );
    TYPE_SET_DESERIALIZER( std::string, DeserializeString );

    TYPE_REGISTER( GameObject );
    TYPE_ADD_MEMBER( GameObject, name );
    TYPE_ADD_MEMBER( GameObject, m_archetype );
    TYPE_SET_SERIALIZER( GameObject, GameObject::Serialize );
    TYPE_SET_DESERIALIZER( GameObject, GameObject::Deserialize );

    TYPE_REGISTER( GameSpace );
    TYPE_SET_SERIALIZER( GameSpace, GameSpace::Serialize );
    TYPE_SET_DESERIALIZER( GameSpace, GameSpace::Deserialize );

    TYPE_REGISTER( Transform );
    TYPE_ADD_MEMBER(Transform, val1);
    TYPE_ADD_MEMBER(Transform, val2);
    TYPE_ADD_MEMBER(Transform, val3);

    TYPE_REGISTER( Tester );
    TYPE_ADD_MEMBER( Tester, testvalue1 );
    TYPE_ADD_MEMBER( Tester, testvalue2 );
  }
}