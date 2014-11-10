/*****************************************************************
Filename: TypeInfo.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once
#include <vector>
#include "modules/functionbinding/Function.h"

namespace Framework
{
  
  namespace AntTweak
  {
    class TBar;
    typedef enum engineTwType
    {
      TW_TYPE_UNDEF   = 0,
#ifdef __cplusplus
      TW_TYPE_BOOLCPP = 1,
#endif // __cplusplus
      TW_TYPE_BOOL8   = 2,
      TW_TYPE_BOOL16,
      TW_TYPE_BOOL32,
      TW_TYPE_CHAR,
      TW_TYPE_INT8,
      TW_TYPE_UINT8,
      TW_TYPE_INT16,
      TW_TYPE_UINT16,
      TW_TYPE_INT32,
      TW_TYPE_UINT32,
      TW_TYPE_FLOAT,
      TW_TYPE_DOUBLE,
      TW_TYPE_COLOR32,    // 32 bits color. Order is RGBA if API is OpenGL or Direct3D10, and inversed if API is Direct3D9 (can be modified by defining 'colorOrder=...', see doc)
      TW_TYPE_COLOR3F,    // 3 floats color. Order is RGB.
      TW_TYPE_COLOR4F,    // 4 floats color. Order is RGBA.
      TW_TYPE_CDSTRING,   // Null-terminated C Dynamic String (pointer to an array of char dynamically allocated with malloc/realloc/strdup)
      TW_TYPE_STDSTRING = (0x2fff0000+sizeof(std::string)),  // C++ STL string (std::string)
      TW_TYPE_QUAT4F = TW_TYPE_CDSTRING+2, // 4 floats encoding a quaternion {qx,qy,qz,qs}
      TW_TYPE_QUAT4D,     // 4 doubles encoding a quaternion {qx,qy,qz,qs}
      TW_TYPE_POS3F,
      TW_TYPE_DIR3F,      // direction vector represented by 3 floats
      TW_TYPE_DIR3D,       // direction vector represented by 3 doubles

      TW_TYPE_STDVECTOR,
      TW_TYPE_HANDLE,
      TW_TYPE_COMPONENT,
      TW_TYPE_LUACOMPONENT,
      TW_TYPE_OBJECT,
      TW_TYPE_GAMESPACE
    } engineTwType;


    typedef void(*SetCallback)(const void* value, void* clientData);
    typedef void(*GetCallback)(void* value, void* clientData);
  }

  // Forward declaration of the typeinfo class
  class TypeInfo;

  class Member
  {
  public:
    // The type of the member
    const TypeInfo* Type(void) const;
    // Memory offset of the member in the class
    unsigned Offset(void) const;
    // Name of the member
    const char* Name(void) const;
    // Uses AntTweakBar
    bool CanTweak(void) const;
    // Tweak Label
    const char* TweakLabel(void) const;
    // Whether or not to auto-create a lua setter/getter
    bool AutoLua(void) const;
    // Custom Callbacks for AntTweak Setters/Getters
    Function TweakSetCB(void) const;
    Function TweakGetCB(void) const;

  private:
    const char *m_name;
    unsigned m_offset;
    const TypeInfo *m_typeInfo;

    const char* m_tweakLabel;

    // Uses AntTweakBar
    bool m_tweak;
    Function m_tweakGetCB;
    Function m_tweakSetCB;

    // AUtomatic lua setter/getter
    bool m_autoLua;

    friend class TypeInfo;
  };

  // Serialization
  typedef void (*SerializeCB)(File&, Variable);
  typedef void (*DeserializeCB)(File&, Variable);

  // Lua
  typedef void (*ToLuaCB)(lua_State*, Variable&);
  typedef void (*FromLuaCB)(lua_State*, int, Variable*);

  // AntTweakBar callback
  typedef void (*ToTweakCB) (AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label);

  class TypeInfo
  {
  public:
    TypeInfo( );
    // Initilization routine
    void Init( const char *name, unsigned size );
    // Adds a member to the type
    void AddMember( const TypeInfo *typeInfo, const char *name, unsigned offset, bool autoLua=true, bool tweak=false,
      const char* tweakLabel=nullptr, Function setCB = Function(), Function getCB = Function());
    // Gets a member from the type
    const Member *GetMember( const char *memberName ) const;

    // Sets a custom serializer callback
    void SetSerializer(SerializeCB cb);
    // Sets a custom deserializer callback
    void SetDeserializer(DeserializeCB cb);

    // Serializes a variable of this type into a file
    void Serialize(File& file, Variable var) const;
    // Deserializes a file into a variable of this type
    void Deserialize(File& file, Variable var) const;

    // Fills out an AntTweak bar with this type
    void Tweak(AntTweak::TBar* bar, Variable var, const char* tempLabel, const char* label) const;

    // Gets a list of all members in the type
    const std::vector<Member>& GetMembers() const;

    // Gets the AntTweakBar enum Type
    AntTweak::engineTwType GetAType(void) const;
    // Sets the AntTweakBar enum Type
    void SetAType(AntTweak::engineTwType);

    // Lua stuff
    // The (name of) lua meta table for this class
    const char* LuaMetaTable(void) const;
    // Sends a variable of this type to lua
    void ToLua(lua_State* L, Variable var) const;
    // Gets a variable of this type from lua, from the given stack index
    void FromLua(lua_State* L, int index, Variable* var) const;
    
    // Sets a ToLua call back function
    void SetToLua(ToLuaCB cb);
    // Sets a FromLua callback function
    void SetFromLua(FromLuaCB cb);
    // Sets how AntTweak bar makes a bar for this type
    void SetToTweak(ToTweakCB cb);

    // Gets the size (in bytes) of this type
    unsigned Size( void ) const;
    // Gets the name of this type
    const char *Name( void ) const;
    // returns TRUE if this type is PLAIN-OLD-DATA otherwise false
    inline bool IsPOD( void ) const { return m_isPOD; };

    // Checks to see if this type has a custom lua callback
    bool HasToLuaCB(void) const { return m_toLua != nullptr; };

    // Makes a new <whatever this type is>
    void *(*New)( void );
    // Copies the type from once destination to another
    void (*Copy)( void *data, const void *src );
    // Deletes the given void* as if it is this type
    void (*Delete)( void *data );
    // Creates a new <whatever this type is> using a copy constructor from the given source
    void (*NewCopy)( void **dest, const void *src );
    // Does a placement New operator for this type on the given data location
    void (*PlacementNew)( void *data );
    // Does a placement Delete operator for this type on the given data location
    void (*PlacementDelete)( void *data );
    // DOes a placement New copy-constructor for this type on the given data location, using the given source
    void (*PlacementCopy)( void *data, const void *src );

  private:
    // Actual size of the type
    unsigned m_size;
    // Actual name of the type
    std::string m_name;

    // AntTweakBar type
    AntTweak::engineTwType m_aType;

    // List of the memebrs
    std::vector<Member> m_members;

    // Lua metatable name
    const char* m_metatable;

    // Pointers to the serialization callback functions
    SerializeCB m_serialize;
    DeserializeCB m_deserialize;

    // Pointer to the lua callback functions
    ToLuaCB m_toLua;
    FromLuaCB m_fromLua;

    // Pointer to the AntTweakbar Callback
    ToTweakCB m_toTweak;

    // Is this type POD?
    bool m_isPOD;

    friend class Serializer;
    friend class IntrospectionManager;
    friend class Factory;
  };

  // Reinterprets a void* into a given type
  template <typename T>
  inline T* Cast( void* data )
  {
    return reinterpret_cast<T *>(data);
  }

  // Reinterprets a void* into a given type (Const)
  template <typename T>
  inline const T* Cast( const void* data )
  {
    return reinterpret_cast<const T*>(data);
  }

  // Constructs a new <type> and uses a placement new operator
  template <typename T>
  inline void* New( void )
  {
    T *data = (T*)malloc( sizeof(T) );
    new (data) T();
    return data;
  }

  // Constructs a new <type> (For POD)
  template <typename T>
  inline void* PODNew(void)
  {
    return malloc( sizeof(T) );
  }

  // Placement new on <type>
  template <typename T>
  inline void PlacementNew(void* data)
  {
    new (data) T( );
  }

  DISABLE_WARNING(4100)
  // There is no such thing as a placement new on POD
  template <typename T>
  inline void PODPlacementNew(void* data)
  {
  }
  END_DISABLE()

  // Copies data from src->dest as if it is the type T
  // Using the assignment operator
  template <typename T>
  inline void Copy(void* dest, const void* src)
  {
    *(Cast<T>( dest )) = *(Cast<T>( src ));
  }

  // Plain old data copy from src->dest (memcpy)
  template <typename T>
  inline void PODCopy(void* dest, const void* src)
  {
    std::memcpy( dest, src, sizeof( T ) );
  }

  // Placement new, with a copy constructor using src
  template <typename T>
  inline void PlacementCopy(void* data, const void* src)
  {
    new (data) T( *(Cast<T>( src )) );
  }

  // POD placement copy is just memcpy
  template <typename T>
  inline void PODPlacementCopy(void* data, const void* src)
  {
    std::memcpy( data, src, sizeof( T ) );
  }

  // Creates a new <type> into dest, using src for a copy constructor
  template <typename T>
  inline void NewCopy(void** dest, const void* src)
  {
    T* newData = (T*)malloc( sizeof(T) );
    new (newData) T( *Cast<T>( src ) );
    *dest = newData;
  }

  // POD Copy constructor just allocates and uses memcpy
  template <typename T>
  inline void PODNewCopy( void** dest, const void* src )
  {
    *dest = malloc( sizeof( T ) );
    std::memcpy( *dest, src, sizeof( T ) );
  }

  // Deletes a data as if it were <type>
  template <typename T>
  inline void Delete( void* data )
  {
    Cast<T>( data )->~T( );
    free( data );
  }

  // POD delete is just (free)
  template <typename T>
  inline void PODDelete( void* data )
  {
    free( data );
  }

  // Equality Comparison
  template <typename T>
  inline bool MetaCompare(const T* a, const T* b)
  {
    return true;
  }

  // Placement delete (deconstructor) on data
  template <typename T>
  inline void PlacementDelete( void* data )
  {
    Cast<T>( data )->~T( );
  }

  DISABLE_WARNING(4100)
    template <typename T>
  inline void PODPlacementDelete( void* data )
  {
  }
  END_DISABLE()

}