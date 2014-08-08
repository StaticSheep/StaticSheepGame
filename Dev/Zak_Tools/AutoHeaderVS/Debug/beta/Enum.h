#pragma once

#include <vector>
#include <hash_map>
#include <string>

namespace Framework
{

  #undef E_NAME
  #undef E_ENTRY
  #undef E_END

    enum E##NAME {

    e##ENTRY,

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
    extern "C" Enum Enum##NAME; \

  #undef  E_ENTRY

  #undef  E_END

  #include "EnumData.h"


  template <typename EnumTYpe>

  #undef  E_NAME
  template<>\

  #include "EnumData.h"

  template <typename EnumTYpe>

  #undef  E_NAME
  template<>\

  #include "EnumData.h"
}

