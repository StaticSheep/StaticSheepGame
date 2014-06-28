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

  void EngineTypeRegistration()
  {
    TYPE_REGISTER_POD( int );
    TYPE_SET_SERIALIZER( int, SerializePOD<int> );
    TYPE_SET_DESERIALIZER( int, DeserializePOD<int> );

    TYPE_REGISTER_POD( unsigned int );

    TYPE_REGISTER_POD( int * );
    TYPE_REGISTER_POD( const char * );

    TYPE_REGISTER_POD( float );
    TYPE_SET_SERIALIZER( float, SerializePOD<float> );
    TYPE_SET_DESERIALIZER( float, DeserializePOD<float> );

    TYPE_REGISTER( std::vector<int> );
    TYPE_SET_SERIALIZER( std::vector<int>, SerializeArray<int> );
    TYPE_SET_DESERIALIZER( std::vector<int>, DeserializeArray<int> );

    TYPE_REGISTER_POD( bool );
    TYPE_SET_SERIALIZER( bool, SerializePOD<bool> );
    TYPE_SET_DESERIALIZER( bool, DeserializePOD<bool> );

    TYPE_REGISTER( std::string );
    TYPE_SET_SERIALIZER( std::string, SerializeString );
    TYPE_SET_DESERIALIZER( std::string, DeserializeString );

    TYPE_REGISTER( GameObject );
    TYPE_SET_SERIALIZER( GameObject, GameObject::Serialize );

    TYPE_REGISTER( Transform );
    TYPE_ADD_MEMBER(Transform, val1);
    TYPE_ADD_MEMBER(Transform, val2);
    TYPE_ADD_MEMBER(Transform, val3);
  }
}