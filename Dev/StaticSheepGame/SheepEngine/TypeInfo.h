/*****************************************************************
Filename: TypeInfo.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once
#include <vector>

namespace Framework
{
  class TypeInfo;

  class Member
  {
  public:
    const TypeInfo *Type( void ) const;
    unsigned Offset( void ) const;
    const char *Name( void ) const;

  private:
    const char *m_name;
    unsigned m_offset;
    const TypeInfo *m_typeInfo;

    friend class TypeInfo;
  };

  typedef void (*SerializeCB)(File&, Variable);
  typedef void (*DeserializeCB)(File&, Variable);

  class TypeInfo
  {
  public:
    TypeInfo( );
    void Init( const char *name, unsigned size );
    void AddMember( const TypeInfo *typeInfo, const char *name, unsigned offset );
    const Member *GetMember( const char *memberName ) const;

    void SetSerializer(SerializeCB cb);
    void SetDeserializer(DeserializeCB cb);

    void Serialize(File& file, Variable var) const;
    void Deserialize(File& file, Variable var) const;



    unsigned Size( void ) const;
    const char *Name( void ) const;
    inline bool IsPOD( void ) const { return m_isPOD; };

   /* void *(*New)( void );
    void (*Copy)( void *data, const void *src );
    void (*Delete)( void *data );
    void (*NewCopy)( void **dest, const void *src );
    void (*PlacementNew)( void *data );
    void (*PlacementDelete)( void *data );
    void (*PlacementCopy)( void *data, const void *src );*/

  private:
    unsigned m_size;
    std::string m_name;
    std::vector<Member> m_members;

    SerializeCB m_serialize;
    DeserializeCB m_deserialize;
    bool m_isPOD;

    friend class Serializer;
    friend class IntrospectionManager;
    friend class Factory;
  };

  template <typename T>
  inline T *Cast( void *data )
  {
    return reinterpret_cast<T *>(data);
  }

  template <typename T>
  inline const T *Cast( const void *data )
  {
    return reinterpret_cast<const T *>(data);
  }

  template <typename T>
  inline void *New( void )
  {
    T *data = (T *)malloc( sizeof( T ) );
    new (data) T( );
    return data;
  }

  template <typename T>
  inline void *PODNew( void )
  {
    return malloc( sizeof( T ) );
  }

  template <typename T>
  inline void PlacementNew( void *data )
  {
    new (data) T( );
  }

  DISABLE_WARNING(4100)
  template <typename T>
  inline void PODPlacementNew( void *data )
  {
  }
  END_DISABLE()

  template <typename T>
  inline void Copy( void *dest, const void *src )
  {
    *(Cast<T>( dest )) = *(Cast<T>( src ));
  }

  template <typename T>
  inline void PODCopy( void *dest, const void *src )
  {
    std::memcpy( dest, src, sizeof( T ) );
  }

  template <typename T>
  inline void PlacementCopy( void *data, const void *src )
  {
    new (data) T( *(Cast<T>( src )) );
  }

  template <typename T>
  inline void PODPlacementCopy( void *data, const void *src )
  {
    std::memcpy( data, src, sizeof( T ) );
  }

  template <typename T>
  inline void NewCopy( void **dest, const void *src )
  {
    T *newData = (T *)malloc( sizeof( T ) );
    new (newData) T( *Cast<T>( src ) );
    *dest = newData;
  }

  template <typename T>
  inline void PODNewCopy( void **dest, const void *src )
  {
    *dest = malloc( sizeof( T ) );
    std::memcpy( *dest, src, sizeof( T ) );
  }

  template <typename T>
  inline void Delete( void *data )
  {
    Cast<T>( data )->~T( );
    free( data );
  }

  template <typename T>
  inline void PODDelete( void *data )
  {
    free( data );
  }

  template <typename T>
  inline void PlacementDelete( void *data )
  {
    Cast<T>( data )->~T( );
  }

  DISABLE_WARNING(4100)
    template <typename T>
  inline void PODPlacementDelete( void *data )
  {
  }
  END_DISABLE()

}