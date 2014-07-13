/*****************************************************************
Filename: DefaultTypes.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/



namespace Framework
{
#define REGISTER_POD_FULL( TYPE ) \
  TYPE_REGISTER_POD( TYPE ); \
  TYPE_SET_SERIALIZER( TYPE, SerializePOD<TYPE> );\
  TYPE_SET_DESERIALIZER( TYPE, DeserializePOD<TYPE> )

  void EngineTypeRegistration()
  {

    TYPE_REGISTER_POD( int );

    TYPE_REGISTER_POD( unsigned int );
    TYPE_SET_TO_LUA( unsigned int, Lua::UIntToLua );
    TYPE_SET_FROM_LUA( unsigned int, Lua::IntFromLua );

    TYPE_REGISTER_POD(const char*);
    TYPE_SET_TO_LUA(char*, Lua::CharPToLua);
    TYPE_SET_FROM_LUA(char*, Lua::CharPFromLua);

    TYPE_REGISTER_POD( float );

    TYPE_SET_SERIALIZER( int, SerializePOD<int> );
    TYPE_SET_DESERIALIZER( int, DeserializePOD<int> );
    TYPE_SET_TO_LUA( int, Lua::IntToLua );
    TYPE_SET_FROM_LUA( int, Lua::IntFromLua );

    TYPE_SET_SERIALIZER( float, SerializePOD<float> );
    TYPE_SET_DESERIALIZER( float, DeserializePOD<float> );
    TYPE_SET_TO_LUA( float, Lua::FloatToLua );
    TYPE_SET_FROM_LUA( float, Lua::FloatFromLua );

    TYPE_REGISTER_POD( bool );
    TYPE_SET_SERIALIZER( bool, SerializePOD<bool> );
    TYPE_SET_DESERIALIZER( bool, DeserializePOD<bool> );

    TYPE_REGISTER( std::vector<int> );
    TYPE_SET_SERIALIZER( std::vector<int>, SerializeArray<int> );
    TYPE_SET_DESERIALIZER( std::vector<int>, DeserializeArray<int> );

    TYPE_REGISTER( std::string );
    TYPE_SET_SERIALIZER( std::string, SerializeString );
    TYPE_SET_DESERIALIZER( std::string, DeserializeString );
    TYPE_SET_TO_LUA( std::string, Lua::StringToLua );
    TYPE_SET_FROM_LUA( std::string, Lua::StringFromLua );

    TYPE_REGISTER( Handle );
    TYPE_SET_SERIALIZER( Handle, Handle::Serialize );
    TYPE_SET_TO_LUA(Handle, Lua::UIntToLua);
    TYPE_SET_FROM_LUA(Handle, Lua::IntFromLua);

    TYPE_REGISTER( Generic );
    TYPE_REGISTER_PTR( Generic* );

    TYPE_REGISTER( Member );
    TYPE_REGISTER_PTR( Member* );

    TYPE_REGISTER( GameComponent );

    TYPE_REGISTER_PTR( GameComponent* );
    //TYPE_SET_TO_LUA( GameComponent*, Lua::GameComponentToLua);

    TYPE_REGISTER( GameObject );
    TYPE_ADD_MEMBER( GameObject, name );
    TYPE_ADD_MEMBER( GameObject, archetype );
    TYPE_SET_SERIALIZER( GameObject, GameObject::Serialize );
    TYPE_SET_DESERIALIZER( GameObject, GameObject::Deserialize );

    TYPE_REGISTER_PTR( GameObject* );
    //TYPE_SET_TO_LUA( GameObject*, Lua::GameObjectToLua );

    TYPE_REGISTER( Archetype );
    TYPE_ADD_MEMBER( Archetype, name );
    TYPE_ADD_MEMBER( Archetype, archetype );
    TYPE_SET_SERIALIZER( Archetype, Archetype::Serialize );
    TYPE_SET_DESERIALIZER( Archetype, Archetype::Deserialize );

    TYPE_REGISTER( GameSpace );
    TYPE_SET_SERIALIZER( GameSpace, GameSpace::Serialize );
    TYPE_SET_DESERIALIZER( GameSpace, GameSpace::Deserialize );

    TYPE_REGISTER_PTR( GameSpace* );
    //TYPE_SET_TO_LUA( GameSpace*, Lua::GameSpaceToLua);

    TYPE_REGISTER( Transform );
    TYPE_ADD_MEMBER(Transform, val1);
    TYPE_ADD_MEMBER(Transform, val2);
    TYPE_ADD_MEMBER(Transform, val3);

    TYPE_REGISTER( Tester );
    TYPE_ADD_MEMBER( Tester, testvalue1 );
    TYPE_ADD_MEMBER( Tester, testvalue2 );

    TYPE_REGISTER( LuaComponent );
    TYPE_ADD_MEMBER( LuaComponent, name );
    TYPE_ADD_MEMBER( LuaComponent, loadCommand );

    TYPE_REGISTER_PTR( LuaComponent* );
    //TYPE_SET_TO_LUA( LuaComponent*, Lua::GameComponentToLua);

    RegisterEnums();
  }
}