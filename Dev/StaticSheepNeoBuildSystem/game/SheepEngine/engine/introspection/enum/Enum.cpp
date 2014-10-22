
#include "pch/precompiled.h"
#include "Enum.h"


namespace Framework
{
  /// <summary>
  /// Initializes a new instance of the <see cref="Enum"/> struct.
  /// </summary>
  /// <param name="name">The name.</param>
  Enum::Enum( const char *name )
    : m_name( name )
  {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="Enum"/> struct.
  /// </summary>
  /// <param name="name">The name.</param>
  /// <param name="literals">The literals.</param>
  Enum::Enum( const char *name, const char **literals )
    : m_name( name )
  {
    for(auto i = literals; *i; ++i)
    {
      m_literals.emplace_back( *i );
      m_literalMap[*i] = m_literals.size() - 1;
    }

    // Erase the count_Name enum
    m_literals.pop_back();
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="Enum"/> struct.
  /// </summary>
  /// <param name="name">The name.</param>
  /// <param name="literals">The literals.</param>
  Enum::Enum( const char *name, const std::vector<std::string>& literals )
    : m_name( name )
    , m_literals( literals )
  {
    for(unsigned i = 0; i < literals.size( ); ++i)
      m_literalMap[literals[i].c_str( )] = i;
  }



  /// <summary>
  /// Determines whether the entryNameStr is a member of the enumeration.
  /// </summary>
  /// <param name="entryNameStr">The entry name string.</param>
  /// <returns></returns>
  bool Enum::IsAnEntry( const char *entryNameStr )
  {
    std::string entryName = entryNameStr;

    return m_literalMap.count(entryName) != 0;

    //return m_literalMap.find(entryName.c_str( )) != m_literalMap.end();
  }

  /// <summary>
  /// Gets the index from string.
  /// </summary>
  /// <param name="entryNameStr">The entry name string.</param>
  /// <returns></returns>
  unsigned int Enum::GetIndexFromString( const char *entryNameStr )
  {
    std::string entryName = entryNameStr;

    return m_literalMap[entryName];
  }

  #undef  E_NAME
  #define E_NAME( NAME )\
    template <>\
    void SerializeEnum<E##NAME>( File& file, Variable var ) { \
    Enum *e = GET_ENUM(NAME); \
    file.Write(STRINGIZE(E##NAME)); \
    file.Write(" "); \
    file.Write("\"");  \
    file.Write(e->m_literals[var.GetValue<E##NAME>()].c_str() ); \
    file.Write("\"");  \
    file.Write("\n"); }

  #include "engine/config/EnumData.h"

    // Create deserialization from file routines
  #undef  E_NAME
  #define E_NAME( NAME )  \
    template <>   \
    void DeserializeEnum<E##NAME>( File& file, Variable var ) {  \
    Enum *e = GET_ENUM(NAME); \
    const TypeInfo *typeFromFile =     \
    Serializer::Get()->PeekType( file, Serializer::Get()->GetPadLevel() );   \
    char temp[256]; \
    file.Read("%*[^\"]\"");  \
    file.Read("%[^\"]\"", temp); \
    var.GetValue<E##NAME>() = (E##NAME)e->GetIndexFromString(temp); }

  #include "engine/config/EnumData.h"

  // Create type info registration
  #undef  E_NAME
  #define E_NAME( NAME ) \
  TYPE_REGISTER( E##NAME );  \
  TYPE_SET_SERIALIZER( E##NAME, SerializeEnum<E##NAME> ); \
  TYPE_SET_DESERIALIZER( E##NAME, DeserializeEnum<E##NAME> );

  void RegisterEnums()
  {
    #include "engine/config/EnumData.h"
  }

  #undef  E_NAME
  #define E_NAME( NAME ) \
    const char *k_##NAME##literalArray[] = {

  #undef  E_ENTRY
  #define E_ENTRY( ENTRY ) \
  #ENTRY,

  #undef  E_END
  #define E_END( NAME ) \
    NULL \
    };

  #include "engine/config/EnumData.h"

  #undef  E_NAME
  #define E_NAME( NAME ) \
    Enum Enum##NAME( \
  #NAME,

  #undef  E_ENTRY
  #define E_ENTRY( ENTRY )

  #undef  E_END
  #define E_END( NAME ) \
    k_##NAME##literalArray );

  #include "engine/config/EnumData.h"

  

};