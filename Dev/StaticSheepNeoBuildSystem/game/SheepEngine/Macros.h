/*****************************************************************
Filename: Macros.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#define STRINGIZE( _ ) #_

#define GET_ENUM( NAME ) \
  (&Framework::Enum##NAME)


#define GET_OFFSET_OF( TYPE, MEMBER ) \
  ((unsigned)(&((((TYPE *)NULL))->MEMBER)))

// Special data types
#define TYPE_REGISTER( T ) \
  Framework::IntrospectionManager::Get( )->RegisterType<Framework::RemoveQualifiers<T>::type>( sizeof( T ), #T, STRINGIZE(__##T##_MT), false )

#define TYPE_SET_TWEAK_TYPE( T, TYPE) \
  ((Framework::TypeInfo *)GET_TYPE( T ))->SetAType( TYPE )

#define TYPE_SET_TWEAK(T, FUNCTION) \
  ((Framework::TypeInfo *)GET_TYPE( T ))->SetToTweak( FUNCTION )

// Plain old data types
#define TYPE_REGISTER_POD( T ) \
  Framework::IntrospectionManager::Get( )->RegisterType<Framework::RemoveQualifiers<T>::type>( sizeof( T ), #T, STRINGIZE(__##T##_MT), true )

#define TYPE_REGISTER_PTR(T) \
  Framework::IntrospectionManager::Get( )->RegisterType<Framework::RemoveQualifiers<T>::type>( sizeof(T), #T, GET_TYPE(RemovePtr<RemoveQualifiers<T>::type>::type)->LuaMetaTable(), true)

#define GET_TYPE( T ) \
  Framework::IntrospectionManager::Get( )->GetType<Framework::RemoveQualifiers<T>::type>( )

#define GET_TYPE_OF( OBJECT ) \
  Framework::IntrospectionManager::Get( )->GetType<Framework::RemoveQualifiers<decltype( OBJECT )>::type>( )

#define GET_TYPE_OF_MEMBER( T, MEMBER ) \
  Framework::RemoveQualifiers<Framework::StripMemberness<decltype( &T::MEMBER )>::type>::type

#define GET_STR_TYPE( STR ) \
  Framework::IntrospectionManager::Get( )->GetType( STR )

// AddMember([TYPE], [MEMBER], [AutoLua Set/Get], [Use AntTweakBar], [Custom AntTweakBar Label], [CustomSet Callback], [CustomGet Callback]
#define TYPE_ADD_MEMBER( T, MEMBER, ... ) \
  ((Framework::TypeInfo *)GET_TYPE( T ))->AddMember( GET_TYPE( GET_TYPE_OF_MEMBER( T, MEMBER ) ), #MEMBER, GET_OFFSET_OF( T, MEMBER ), __VA_ARGS__ )

#define TYPE_SET_SERIALIZER( T, SERIALIZER ) \
  ((Framework::TypeInfo *)GET_TYPE( T ))->SetSerializer( SERIALIZER )

#define TYPE_SET_DESERIALIZER( T, DESERIALIZER ) \
  ((Framework::TypeInfo *)GET_TYPE( T ))->SetDeserializer( DESERIALIZER )

#define BUILD_FUNCTION(FN) \
  Framework::BuildFunction<decltype( &FN ), &FN>( &FN )

#define BIND_FUNCTION(L, FN) \
  static Framework::Function FN##Function = BUILD_FUNCTION(FN); \
  Framework::Lua::BindFunctionToLua(L, &FN, #FN##Function)

// Explicit bind
#define BIND_FUNCTION_EX(L, FN, NAME, ...) \
  static Framework::Function NAME##Function = BUILD_FUNCTION(FN); \
  Framework::Lua::BindFunctionToLua(L, &NAME##Function, #NAME, __VA_ARGS__)

#define BIND_FUNCTION_TABLE(L, FN, NAME, TABLE) \
  static Framework::Function TABLE##NAME##Function = BUILD_FUNCTION(FN); \
  Framework::Lua::BindFunctionToLua(L, &TABLE##NAME##Function, #NAME, #TABLE)

#define CREATE_TABLE(L, NAME) \
  Framework::Lua::CreateNewGTable(L, #NAME)

#define TYPE_SET_TO_LUA(T, CB)\
  ((Framework::TypeInfo*)GET_TYPE(T))->SetToLua(CB)

#define TYPE_SET_FROM_LUA(T, CB)\
  ((Framework::TypeInfo*)GET_TYPE(T))->SetFromLua(CB)


// Add a pointer by an integral offset (bytes)
#define PTR_ADD( PTR, OFFSET ) \
  ((char *)(PTR) + (OFFSET))

// Subtract a pointer by an integral offset (bytes)
#define PTR_SUB( PTR, OFFSET ) \
  ((char *)(PTR) - (OFFSET))


// Don't forget to use END_DISABLE
#define DISABLE_WARNING( WARN_ID ) \
  __pragma( warning( push ) ) \
  __pragma( warning( disable : WARN_ID ) )

// Disables a warning without the push call
#define PERM_DSIABLE_WARNING( WARN_ID ) \
  __pragma( warning( disable : WARN_ID ) )

#define END_DISABLE( ) \
  __pragma( warning( pop ) )

