/*****************************************************************
Filename: DefaultTypes.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"

#include "systems/anttweak/AntTweakModule.h"
#include "systems/anttweak/TweakHelper.h"

#include "components/sound/CSoundEmitter.h"
#include "components/sound/CSoundPlayer.h"
#include "components/sprites/CSprite.h"
#include "components/sprites/CAniSprite.h"
#include "components/rigidbody/CRigidBody.h"
#include "components/transform/CTransform.h"
#include "components/colliders/CBoxCollider.h"
#include "components/lua/CLuaComponent.h"
#include "components/gamepad/CGamePad.h"
#include "components/colliders/CCircleCollider.h"
#include "components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/CBullet_default.h"
#include "components/gameplay_scripts/CElevatorPlat.h"
#include "components/gameplay_scripts/CLevel1_Logic.h"
#include "components/gameplay_scripts/CGiantKillBox.h"
#include "components/gameplay_scripts/CGrinder.h"

namespace Framework
{
#define REGISTER_POD_FULL( TYPE ) \
  TYPE_REGISTER_POD( TYPE ); \
  TYPE_SET_SERIALIZER( TYPE, SerializePOD<TYPE> );\
  TYPE_SET_DESERIALIZER( TYPE, DeserializePOD<TYPE> )

  void EngineTypeRegistration()
  {

    TYPE_REGISTER_POD( int );
    TYPE_SET_SERIALIZER( int, SerializePOD<int> );
    TYPE_SET_DESERIALIZER( int, DeserializePOD<int> );
    TYPE_SET_TO_LUA( int, Lua::IntToLua );
    TYPE_SET_FROM_LUA( int, Lua::IntFromLua );
    TYPE_SET_TWEAK_TYPE( int, AntTweak::TW_TYPE_INT32 );

    TYPE_REGISTER_POD( unsigned int );
    TYPE_SET_TO_LUA( unsigned int, Lua::UIntToLua );
    TYPE_SET_FROM_LUA( unsigned int, Lua::IntFromLua );
    TYPE_SET_SERIALIZER(unsigned int, SerializePOD<unsigned int>);
    TYPE_SET_DESERIALIZER(unsigned int, DeserializePOD<unsigned int>);
    TYPE_SET_TWEAK_TYPE( unsigned int, AntTweak::TW_TYPE_UINT32 );

    TYPE_REGISTER_POD(void*);

    TYPE_REGISTER_POD(const char*);
    TYPE_SET_TO_LUA(char*, Lua::CharPToLua);
    TYPE_SET_FROM_LUA(char*, Lua::CharPFromLua);
    TYPE_SET_TWEAK_TYPE( char*, AntTweak::TW_TYPE_CDSTRING );

    TYPE_REGISTER_POD( float );
    TYPE_SET_SERIALIZER( float, SerializePOD<float> );
    TYPE_SET_DESERIALIZER( float, DeserializePOD<float> );
    TYPE_SET_TO_LUA( float, Lua::FloatToLua );
    TYPE_SET_FROM_LUA( float, Lua::FloatFromLua );
    TYPE_SET_TWEAK_TYPE( float, AntTweak::TW_TYPE_FLOAT );

    TYPE_REGISTER_POD( bool );
    TYPE_SET_SERIALIZER( bool, SerializePOD<bool> );
    TYPE_SET_DESERIALIZER( bool, DeserializePOD<bool> );
    TYPE_SET_TWEAK_TYPE( bool, AntTweak::TW_TYPE_BOOLCPP );


    TYPE_REGISTER( std::vector<int> );
    TYPE_SET_SERIALIZER( std::vector<int>, SerializeArray<int> );
    TYPE_SET_DESERIALIZER( std::vector<int>, DeserializeArray<int> );
    TYPE_SET_TWEAK_TYPE( std::vector<int>, AntTweak::TW_TYPE_STDVECTOR);

    TYPE_REGISTER( std::string );
    TYPE_SET_SERIALIZER( std::string, SerializeString );
    TYPE_SET_DESERIALIZER( std::string, DeserializeString );
    TYPE_SET_TO_LUA( std::string, Lua::StringToLua );
    TYPE_SET_FROM_LUA( std::string, Lua::StringFromLua );
    TYPE_SET_TWEAK( std::string, AntTweak::Tweaker::TweakString );
    TYPE_SET_TWEAK_TYPE( std::string, AntTweak::TW_TYPE_STDSTRING );
    

    TYPE_REGISTER( Handle );
    TYPE_SET_SERIALIZER( Handle, Handle::Serialize );
    TYPE_SET_TO_LUA(Handle, Lua::UIntToLua);
    TYPE_SET_FROM_LUA(Handle, Lua::IntFromLua);
    TYPE_SET_TWEAK_TYPE( Handle, AntTweak::TW_TYPE_HANDLE );

    TYPE_REGISTER( Generic );
    TYPE_SET_FROM_LUA( Generic, Lua::GenericObjectFromLua );
    TYPE_REGISTER_PTR( Generic* );
    TYPE_SET_FROM_LUA( Generic*, Lua::GenericObjectFromLua );

    TYPE_REGISTER( Member );
    TYPE_REGISTER_PTR( Member* );

    TYPE_REGISTER(Vec2);
    TYPE_ADD_MEMBER(Vec2, X, false, true, "X");
    TYPE_ADD_MEMBER(Vec2, Y, false, true, "Y");
    TYPE_SET_TO_LUA(Vec2, Vec2::ToLua);
    TYPE_SET_FROM_LUA(Vec2, Vec2::FromLua);

    TYPE_REGISTER(Vec3);
    TYPE_ADD_MEMBER(Vec3, x_);
    TYPE_ADD_MEMBER(Vec3, y_);
    TYPE_ADD_MEMBER(Vec3, z_);
    TYPE_SET_TWEAK_TYPE(Vec3, AntTweak::TW_TYPE_DIR3F);
    TYPE_SET_TO_LUA(Vec3, Vec3::ToLua);
    TYPE_SET_FROM_LUA(Vec3, Vec3::FromLua);

    TYPE_REGISTER(Vec4);
    TYPE_ADD_MEMBER(Vec4, R);
    TYPE_ADD_MEMBER(Vec4, G);
    TYPE_ADD_MEMBER(Vec4, B);
    TYPE_ADD_MEMBER(Vec4, A);
    TYPE_SET_TWEAK_TYPE(Vec4, AntTweak::TW_TYPE_COLOR4F);


    TYPE_REGISTER( GameComponent );
    TYPE_SET_FROM_LUA( GameComponent, Lua::GenericObjectFromLua );
    TYPE_SET_TWEAK_TYPE( GameComponent, AntTweak::TW_TYPE_COMPONENT );
    TYPE_REGISTER_PTR( GameComponent* );
    //TYPE_SET_TO_LUA( GameComponent*, Lua::GameComponentToLua);

    TYPE_REGISTER( GameObject );
    TYPE_ADD_MEMBER( GameObject, name, true, true, "Name", BUILD_FUNCTION(GameObject::TweakSetName));
    TYPE_ADD_MEMBER( GameObject, archetype, true, true, "Archetype");
    TYPE_SET_SERIALIZER( GameObject, GameObject::Serialize );
    TYPE_SET_DESERIALIZER( GameObject, GameObject::Deserialize );
    TYPE_SET_FROM_LUA( GameObject, Lua::GenericObjectFromLua );
    TYPE_SET_TWEAK( GameObject, GameObject::CustomTweak );
    TYPE_SET_TWEAK_TYPE( GameObject, AntTweak::TW_TYPE_OBJECT );
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
    TYPE_SET_TWEAK_TYPE( GameSpace, AntTweak::TW_TYPE_GAMESPACE );
    TYPE_SET_TWEAK( GameSpace, GameSpace::CustomTweak );
    TYPE_REGISTER_PTR( GameSpace* );
    //TYPE_SET_TO_LUA( GameSpace*, Lua::GameSpaceToLua);

    TYPE_REGISTER( Transform );
    Transform::RegisterMembers();
    TYPE_SET_FROM_LUA( Transform, Lua::GenericObjectFromLua );
    TYPE_SET_TWEAK_TYPE( Transform, AntTweak::TW_TYPE_COMPONENT );
    TYPE_SET_SERIALIZER( Transform, Transform::Serialize );
    TYPE_SET_DESERIALIZER( Transform, Transform::Deserialize );
    TYPE_SET_TWEAK( Transform, Transform::ToTweak );
    
	  TYPE_REGISTER(PlayerController);
	  TYPE_SET_TWEAK_TYPE(PlayerController, AntTweak::TW_TYPE_COMPONENT);
	  TYPE_ADD_MEMBER(PlayerController, playerNum, false, true, "Player Number");

	  TYPE_REGISTER(Bullet_Default);
	  TYPE_SET_TWEAK_TYPE(Bullet_Default, AntTweak::TW_TYPE_COMPONENT);

    TYPE_REGISTER(ElevatorPlat);
    TYPE_SET_TWEAK_TYPE(ElevatorPlat, AntTweak::TW_TYPE_COMPONENT);
    TYPE_ADD_MEMBER(ElevatorPlat, direction, false, true, "Elevator Direction");

    TYPE_REGISTER(Level1_Logic);
    TYPE_SET_TWEAK_TYPE(Level1_Logic, AntTweak::TW_TYPE_COMPONENT);
    TYPE_ADD_MEMBER(Level1_Logic, timeLimit, false, true, "KillBox Timer");
    TYPE_ADD_MEMBER(Level1_Logic, numOfPlayers, false, true, "# of players");

    TYPE_REGISTER(GiantKillBox);
    TYPE_SET_TWEAK_TYPE(GiantKillBox, AntTweak::TW_TYPE_COMPONENT);

    TYPE_REGISTER(Grinder);
    TYPE_SET_TWEAK_TYPE(Grinder, AntTweak::TW_TYPE_COMPONENT);

    TYPE_REGISTER( BoxCollider );
    TYPE_ADD_MEMBER( BoxCollider, m_width, false, true, "Width");
    TYPE_ADD_MEMBER( BoxCollider, m_height, false, true, "Height");
    TYPE_ADD_MEMBER(BoxCollider, m_materialName, false, true, "Material"); 
    TYPE_ADD_MEMBER(BoxCollider, m_hasCollisionCallback, false, true, "UsesCallback");
    TYPE_SET_TWEAK_TYPE(BoxCollider, AntTweak::TW_TYPE_COMPONENT);

    TYPE_REGISTER(CircleCollider);
    TYPE_ADD_MEMBER(CircleCollider, m_radius, false, true, "Radius");
    TYPE_ADD_MEMBER(CircleCollider, m_materialName, false, true, "Material");
    TYPE_ADD_MEMBER(CircleCollider, m_hasCollisionCallback, false, true, "UsesCallback");
    TYPE_SET_TWEAK_TYPE(CircleCollider, AntTweak::TW_TYPE_COMPONENT);

    TYPE_REGISTER(GamePad);
    TYPE_ADD_MEMBER(GamePad, GamepadIndex, false, true, "PadNumber", BUILD_FUNCTION(GamePad::EditorSetPad));
    TYPE_SET_TWEAK_TYPE(GamePad, AntTweak::TW_TYPE_COMPONENT);


    TYPE_REGISTER( Sprite );
    TYPE_ADD_MEMBER(Sprite, m_spriteName, false, true, "Texture", BUILD_FUNCTION(Sprite::TweakSetTexture));
    TYPE_ADD_MEMBER(Sprite, Color, false, true, "Color");
    TYPE_ADD_MEMBER( Sprite, Size, false, true, "Scale");
    TYPE_SET_TWEAK_TYPE( Sprite, AntTweak::TW_TYPE_COMPONENT );
    TYPE_SET_FROM_LUA( Sprite, Lua::GenericObjectFromLua );

    TYPE_REGISTER( AniSprite );
    TYPE_ADD_MEMBER(AniSprite, m_spriteName, false, true, "Texture", BUILD_FUNCTION(Sprite::TweakSetTexture));
    TYPE_ADD_MEMBER(AniSprite, m_frames, false, true, "Frames", BUILD_FUNCTION(AniSprite::Test));
    TYPE_ADD_MEMBER(AniSprite, m_startFrame, false, true, "StartFrame");
    TYPE_ADD_MEMBER(AniSprite, m_endFrame, false, true, "EndFrame");
    TYPE_ADD_MEMBER(AniSprite, m_frameRate, false, true, "FrameRate");
    TYPE_ADD_MEMBER(AniSprite, Color, false, true, "Color");
    TYPE_ADD_MEMBER( AniSprite, Size, false, true, "Scale");
    TYPE_SET_TWEAK_TYPE( AniSprite, AntTweak::TW_TYPE_COMPONENT );
    TYPE_SET_FROM_LUA( AniSprite, Lua::GenericObjectFromLua );

    TYPE_REGISTER( SoundEmitter );
    TYPE_SET_FROM_LUA(SoundEmitter, Lua::GenericObjectFromLua );

    TYPE_REGISTER( SoundPlayer );
    TYPE_SET_FROM_LUA(SoundPlayer, Lua::GenericObjectFromLua );

    TYPE_REGISTER( LuaComponent );
    TYPE_ADD_MEMBER( LuaComponent, name );
    TYPE_ADD_MEMBER( LuaComponent, loadCommand );
    TYPE_SET_TWEAK_TYPE( LuaComponent, AntTweak::TW_TYPE_LUACOMPONENT );
    TYPE_SET_FROM_LUA( LuaComponent, Lua::GenericObjectFromLua );
    TYPE_REGISTER_PTR( LuaComponent* );
    //TYPE_SET_TO_LUA( LuaComponent*, Lua::GameComponentToLua);

    TYPE_REGISTER( AntTweak::TBar );
    TYPE_REGISTER_PTR( AntTweak::TBar* );

    RegisterEnums();
  }
}