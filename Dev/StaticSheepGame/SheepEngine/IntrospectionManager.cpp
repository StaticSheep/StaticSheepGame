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

  const std::hash_map<std::string, TypeInfo*>& IntrospectionManager::GetTypeMap() const
  {
    return m_typeMap;
  }

  const TypeInfo* IntrospectionManager::GetType(const char* typeName) const
  {
    std::string typeNameStr = typeName;

    if (m_typeMap.find(typeNameStr) != m_typeMap.end())
    {
      return m_typeMap.at(typeNameStr);
    }

    return nullptr;
  }

  const TypeInfo* IntrospectionManager::GetType(std::string& typeName) const
  {
    if (m_typeMap.find(typeName) != m_typeMap.end())
    {
      return m_typeMap.at(typeName);
    }

    return nullptr;
  }

  IntrospectionManager::IntrospectionManager( )
  {
  }

  IntrospectionManager::~IntrospectionManager( )
  {
  }
}

