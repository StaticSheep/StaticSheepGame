/*****************************************************************
Filename: Enum.h
Project:
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include <vector>
#include <hash_map>
#include <string>

#include "engine/core/Macros.h"
#include "modules/serializer/file/File.h"
#include "modules/introspection/variable/Variable.h"

namespace Framework
{

  #undef E_NAME
  #undef E_ENTRY
  #undef E_END

  #define E_NAME( NAME ) \
    enum E##NAME {

  #define E_ENTRY( ENTRY ) \
    e##ENTRY,

  #define E_END( NAME ) \
  };

  #include "config/EnumData.h"

  void RegisterEnums();

  struct Enum
  {
    Enum( const char *name);
    Enum( const char *name, const char **literals );
    Enum( const char *name, const std::vector<std::string>& literals );

    std::string m_name;
    std::vector<std::string> m_literals;
    std::hash_map<std::string, unsigned int> m_literalMap;

    bool IsAnEntry( const char *entryName );
    unsigned GetIndexFromString( const char *entryName );

  };

  // Declare Enum objects
  #undef  E_NAME
  #define E_NAME( NAME ) \
    extern "C" Enum Enum##NAME; \
    __pragma( comment ( linker, STRINGIZE( /INCLUDE:_##Enum##NAME ) ) )

  #undef  E_ENTRY
  #define E_ENTRY( ENTRY )

  #undef  E_END
  #define E_END( NAME )

  #include "config/EnumData.h"


  template <typename EnumTYpe>
  void SerializeEnum(File& file, Variable var);

  #undef  E_NAME
  #define E_NAME(NAME)\
  template<>\
  void SerializeEnum<E##NAME>(File& file, Variable var);

  #include "config/EnumData.h"

  template <typename EnumTYpe>
  void DeserializeEnum(File& file, Variable var);

  #undef  E_NAME
  #define E_NAME(NAME)\
  template<>\
  void DeserializeEnum<E##NAME>(File& file, Variable var);

  #include "config/EnumData.h"
}
