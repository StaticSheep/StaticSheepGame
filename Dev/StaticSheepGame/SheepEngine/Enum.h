#pragma once

#include <vector>
#include <hash_map>
#include <string>

/** BEGIN FIX **/
namespace std                                                                                 
{                                                                                             
  template<> struct hash< std::string >                                                       
  {                                                                                           
    size_t operator()( const std::string& x ) const                                           
    {                                                                                         
      return hash< const char* >()( x.c_str() );                                              
    }                                                                                         
  };                                                                                          
}          
/** END FIX **/

#define STRINGIZE( _ ) #_

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
    Enum( const char *name);
    Enum( const char *name, const char **literals );
    Enum( const char *name, const std::vector<std::string>& literals );

    std::string m_name;
    std::vector<std::string> m_literals;
    std::hash_map<std::string, unsigned> m_literalMap;

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

  #include "EnumData.h"


#define GET_ENUM( NAME ) \
  (&Framework::Enum##NAME)

}
