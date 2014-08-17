#pragma once

#include <vector>
#include <hash_map>
#include <string>

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

  #include "EnumData.h"


  struct Enum
  {

    std::string m_name;
    std::vector<std::string> m_literals;
    std::hash_map<std::string, unsigned int> m_literalMap;


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

  #include "EnumData.h"



  #undef  E_NAME
  #define E_NAME(NAME)\
  template<>\
  void SerializeEnum<E##NAME>(File& file, Variable var);

  #include "EnumData.h"


  #undef  E_NAME
  #define E_NAME(NAME)\
  template<>\
  void DeserializeEnum<E##NAME>(File& file, Variable var);

  #include "EnumData.h"
}

