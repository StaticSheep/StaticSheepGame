/*****************************************************************
Filename: Variable.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "precompiled.h"

namespace Framework
{
  Variable::Variable( )
    : m_data( NULL )
    , m_typeInfo( NULL )
  {
  }

  Variable::Variable( const TypeInfo *typeInfo, void *data )
    : m_data( data )
    , m_typeInfo( typeInfo )
  {
  }

  Variable::Variable( const TypeInfo* typeInfo, const void* data )
    : m_data( const_cast<void*>(data) )
    , m_typeInfo( typeInfo )
  {
  }

  Variable::Variable( const Variable& rhs )
  {
    m_data = rhs.m_data;
    m_typeInfo = rhs.m_typeInfo;
  }

  void *Variable::GetData( void ) const
  {
    return m_data;
  }

  const TypeInfo *Variable::GetTypeInfo( void ) const
  {
    return m_typeInfo;
  }

  bool Variable::IsValid( void ) const
  {
    return m_typeInfo != NULL && m_data != NULL;
  }

  void Variable::PlacementNew( void )
  {
    m_typeInfo->PlacementNew( m_data );
  }

  void Variable::PlacementDelete( void )
  {
    m_typeInfo->PlacementDelete( m_data );
  }

  void Variable::Delete( void )
  {
    m_typeInfo->PlacementDelete( m_data );
    m_typeInfo->Delete( m_data );
  }

  void Variable::Serialize( File& file ) const
  {
    m_typeInfo->Serialize( file, *this );
  }

  void Variable::Deserialize( File& file )
  {
    m_typeInfo->Deserialize( file, *this );
  }

  Variable& Variable::operator=( const Variable& rhs )
  {
    m_data = rhs.m_data;
    m_typeInfo = rhs.m_typeInfo;

    return *this;
  }

  void Variable::ToLua(lua_State* L) const
  {
    m_typeInfo->ToLua(L, *this);
  }

  void Variable::FromLua(lua_State* L, int index)
  {
    m_typeInfo->FromLua(L, index, this);
  }

  void Variable::Tweak(AntTweak::TBar* bar, const char* tempLabel, const char* label)
  {
    m_typeInfo->Tweak(bar, *this, tempLabel, label);
  }
}
