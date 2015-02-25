/*****************************************************************
Filename: DefaultTypes.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"

#include "systems/anttweak/AntTweakModule.h"
#include "systems/anttweak/TweakHelper.h"
#include "systems/input/devices/GamepadInput.h"

#include "components/sound/CSoundEmitter.h"
#include "components/sound/CSoundPlayer.h"
#include "components/sprites/CSprite.h"
#include "components/sprites/CAniSprite.h"
#include "Components/lights/CPointLight.h"
#include "Components/camera/CCamera.h"
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
#include "components/gameplay_scripts/CLevel1_Lighting.h"
#include "components/gameplay_scripts/CGiantKillBox.h"
#include "components/gameplay_scripts/CGrinder.h"
#include "components/gameplay_scripts/CExplosion.h"
#include "components/gameplay_scripts/CWeaponPickup.h"
#include "types/weapons/WPistol.h"
#include "types/weapons/WShotgun.h"
#include "types/weapons/WAutomatic.h"
#include "types/weapons/WLaser.h"
#include "types/powerUps/PDamage.h"
#include "types/powerUps/PShield.h"
#include "components/gameplay_scripts/CWarningText.h"
#include "components/gameplay_scripts/CBackgroundPan.h"
#include "components/gameplay_scripts/CCheats.h"
#include "components/slotmachine/slotmachine.h"
#include "components/gameplay_scripts/CAimingArrow.h"
#include "components/gameplay_scripts/CDashEffect.h"
#include "components/particles/CParticleSystem.h"
#include "components/particles/CParticleCircleEmitter.h"
#include "components/particles/CParticleBoxEmitter.h"
#include "components/particles/Particles.h"
#include "components/gameplay_scripts/CAsteroid.h"
#include "components/gameplay_scripts/CPowerupPickup.h"


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
    TYPE_SET_TO_LUA(bool, Lua::BoolToLua);
    TYPE_SET_FROM_LUA(bool, Lua::BoolFromLua);
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

    TYPE_REGISTER(LightColor);
    TYPE_ADD_MEMBER(LightColor, R, false, true, "Red");
    TYPE_ADD_MEMBER(LightColor, G, false, true, "Green");
    TYPE_ADD_MEMBER(LightColor, B, false, true, "Blue");
    TYPE_ADD_MEMBER(LightColor, A, false, true, "Intensity"); 

    TYPE_REGISTER(LightVec3);
    TYPE_ADD_MEMBER(LightVec3, X, false, true, "Constant");
    TYPE_ADD_MEMBER(LightVec3, Y, false, true, "Linear");
    TYPE_ADD_MEMBER(LightVec3, Z, false, true, "Quadratic");

    TYPE_REGISTER( GameComponent );
    TYPE_SET_FROM_LUA( GameComponent, Lua::GenericObjectFromLua );
    TYPE_SET_TWEAK_TYPE( GameComponent, AntTweak::TW_TYPE_COMPONENT );
    TYPE_REGISTER_PTR( GameComponent* );
    //TYPE_SET_TO_LUA( GameComponent*, Lua::GameComponentToLua);

    TYPE_REGISTER( GameObject );
    TYPE_ADD_MEMBER( GameObject, name, true, true, "Name",
      BUILD_FUNCTION(GameObject::TweakSetName));
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

    TYPE_REGISTER(Level1_Lighting);
    TYPE_SET_TWEAK_TYPE(Level1_Lighting, AntTweak::TW_TYPE_COMPONENT);

    TYPE_REGISTER(GiantKillBox);
    TYPE_SET_TWEAK_TYPE(GiantKillBox, AntTweak::TW_TYPE_COMPONENT);

    TYPE_REGISTER(Grinder);
    TYPE_SET_TWEAK_TYPE(Grinder, AntTweak::TW_TYPE_COMPONENT);

    TYPE_REGISTER(Explosion);
    TYPE_SET_TWEAK_TYPE(Explosion, AntTweak::TW_TYPE_COMPONENT);

    TYPE_REGISTER(WarningText);
    TYPE_SET_TWEAK_TYPE(WarningText, AntTweak::TW_TYPE_COMPONENT);

    TYPE_REGISTER(BackgroundPan);
    TYPE_SET_TWEAK_TYPE(BackgroundPan, AntTweak::TW_TYPE_COMPONENT);
    TYPE_ADD_MEMBER(BackgroundPan, rate, false, true, "Pan Speed");

    TYPE_REGISTER(WeaponPickup);
    TYPE_SET_TWEAK_TYPE(WeaponPickup, AntTweak::TW_TYPE_COMPONENT);
    TYPE_ADD_MEMBER(WeaponPickup, weaponID, false, true, "Weapon Type");
    TYPE_ADD_MEMBER(WeaponPickup, weaponNum, false, true, "Weapon Num");

    TYPE_REGISTER(PowerupPickup);
    TYPE_SET_TWEAK_TYPE(PowerupPickup, AntTweak::TW_TYPE_COMPONENT);
    TYPE_ADD_MEMBER(PowerupPickup, powerNum, false, true, "Power Number");

    TYPE_REGISTER(Cheats);
    TYPE_SET_TWEAK_TYPE(Cheats, AntTweak::TW_TYPE_COMPONENT);
    TYPE_ADD_MEMBER(Cheats, enabled, false, true, "Enable?");

    TYPE_REGISTER(AimingArrow);
    TYPE_SET_TWEAK_TYPE(AimingArrow, AntTweak::TW_TYPE_COMPONENT);
    TYPE_ADD_MEMBER(AimingArrow, arrowColor, false, true, "Color");

    TYPE_REGISTER(DashEffect);
    TYPE_SET_TWEAK_TYPE(DashEffect, AntTweak::TW_TYPE_COMPONENT);

    TYPE_REGISTER(Asteroid);
    TYPE_SET_TWEAK_TYPE(Asteroid, AntTweak::TW_TYPE_COMPONENT);

    TYPE_REGISTER(Pistol);
    TYPE_REGISTER(Shotgun);
    TYPE_REGISTER(Automatic);
    TYPE_REGISTER(Laser);

    TYPE_REGISTER(DamageBoost);
    TYPE_REGISTER(Shield);

    TYPE_REGISTER( BoxCollider );
    TYPE_ADD_MEMBER(BoxCollider, m_width, false, true, "Width",
      BUILD_FUNCTION(RigidBody::UpdateWidth));
    TYPE_ADD_MEMBER(BoxCollider, m_height, false, true, "Height",
      BUILD_FUNCTION(RigidBody::UpdateHeight));
    TYPE_ADD_MEMBER(BoxCollider, m_materialName, false, true, "Material"); 
    TYPE_ADD_MEMBER(BoxCollider, m_hasCollisionCallback, false, true, "UsesCallback");
    TYPE_ADD_MEMBER(BoxCollider, m_CollisionGroup, false, true, "CollisionGroup");
    TYPE_SET_TWEAK_TYPE(BoxCollider, AntTweak::TW_TYPE_COMPONENT);

    TYPE_REGISTER(CircleCollider);
    TYPE_ADD_MEMBER(CircleCollider, m_radius, false, true, "Radius");
    TYPE_ADD_MEMBER(CircleCollider, m_materialName, false, true, "Material");
    TYPE_ADD_MEMBER(CircleCollider, m_hasCollisionCallback, false, true, "UsesCallback");
    TYPE_ADD_MEMBER(CircleCollider, m_CollisionGroup, false, true, "CollisionGroup");
    TYPE_SET_TWEAK_TYPE(CircleCollider, AntTweak::TW_TYPE_COMPONENT);

    TYPE_REGISTER(GamePad);
    TYPE_ADD_MEMBER(GamePad, GamepadIndex, false, true, "PadNumber",
      BUILD_FUNCTION(GamePad::EditorSetPad));
    TYPE_SET_TWEAK_TYPE(GamePad, AntTweak::TW_TYPE_COMPONENT);


    TYPE_REGISTER( Sprite );
    TYPE_ADD_MEMBER(Sprite, m_spriteName, false, true, "Texture",
      BUILD_FUNCTION(Sprite::TweakSetTexture));
    TYPE_ADD_MEMBER(Sprite, Color, false, true, "Color");
    TYPE_ADD_MEMBER( Sprite, Size, false, true, "Size");
    TYPE_ADD_MEMBER(Sprite, m_flipX, false, true, "FlipX");
    TYPE_ADD_MEMBER(Sprite, m_flipY, false, true, "FlipY");
    TYPE_ADD_MEMBER(Sprite, m_uvScale, false, true, "Tiled");
    TYPE_SET_TWEAK_TYPE( Sprite, AntTweak::TW_TYPE_COMPONENT );
    TYPE_SET_FROM_LUA( Sprite, Lua::GenericObjectFromLua );

    TYPE_REGISTER(PointLight);
    TYPE_ADD_MEMBER(PointLight, m_brightness, false, true, "LightColor");
    TYPE_ADD_MEMBER(PointLight, m_attenuation, false, true, "Attenuation");
    TYPE_SET_TWEAK_TYPE(PointLight, AntTweak::TW_TYPE_COMPONENT);
    TYPE_SET_FROM_LUA(PointLight, Lua::GenericObjectFromLua);

    TYPE_REGISTER( AniSprite );
    TYPE_ADD_MEMBER(AniSprite, m_spriteName, false, true, "Texture",
      BUILD_FUNCTION(Sprite::TweakSetTexture));
    TYPE_ADD_MEMBER(AniSprite, m_frames, false, true, "Frames",
      BUILD_FUNCTION(AniSprite::Test));
    TYPE_ADD_MEMBER(AniSprite, m_startFrame, false, true, "StartFrame",
      BUILD_FUNCTION(AniSprite::TweakStartFrame));
    TYPE_ADD_MEMBER(AniSprite, m_endFrame, false, true, "EndFrame",
      BUILD_FUNCTION(AniSprite::TweakEndFrame));
    TYPE_ADD_MEMBER(AniSprite, m_frameRate, false, true, "FrameRate");
    TYPE_ADD_MEMBER(AniSprite, m_loop, false, true, "Animation Loops");
    TYPE_ADD_MEMBER(AniSprite, m_paused, false, true, "Pause");
    TYPE_ADD_MEMBER(AniSprite, m_flipX, false, true, "FlipX");
    TYPE_ADD_MEMBER(AniSprite, m_flipY, false, true, "FlipY");
    TYPE_ADD_MEMBER(AniSprite, Color, false, true, "Color");
    TYPE_ADD_MEMBER( AniSprite, Size, false, true, "Scale");
    TYPE_SET_TWEAK_TYPE( AniSprite, AntTweak::TW_TYPE_COMPONENT );
    TYPE_SET_FROM_LUA( AniSprite, Lua::GenericObjectFromLua );

    TYPE_REGISTER(ParticleSystem);
    TYPE_ADD_MEMBER(ParticleSystem, textureName, false, true, "ParticleTexture");
    TYPE_ADD_MEMBER(ParticleSystem, particleLife, false, true, "ParticleLifetime");
    TYPE_ADD_MEMBER(ParticleSystem, directionEase, false, true, "DirectionEase");
    TYPE_ADD_MEMBER(ParticleSystem, direction, false, true, "ParticleDirection", 
      BUILD_FUNCTION(ParticleSystem::TweakSetDirection));
    TYPE_ADD_MEMBER(ParticleSystem, angularVelocity, false, true, "ParticleAngularVelocity");
    TYPE_ADD_MEMBER(ParticleSystem, scaleEase, false, true, "ScaleEase");
    TYPE_ADD_MEMBER(ParticleSystem, scale, false, true, "ParticleScale");
    TYPE_ADD_MEMBER(ParticleSystem, colorEase, false, true, "ColorEase");
    TYPE_ADD_MEMBER(ParticleSystem, color, false, true, "ParticleColor");
    TYPE_ADD_MEMBER(ParticleSystem, speedEase, false, true, "SpeedEase");
    TYPE_ADD_MEMBER(ParticleSystem, speed, false, true, "ParticleSpeed");
    
    TYPE_SET_TWEAK_TYPE(ParticleSystem, AntTweak::TW_TYPE_COMPONENT);

    TYPE_REGISTER(ParticleOptionShort<float>);
    TYPE_ADD_MEMBER(ParticleOptionShort<float>, m_startMin, false, true, "Minimum");
    TYPE_ADD_MEMBER(ParticleOptionShort<float>, m_startMax, false, true, "Maximum");

    TYPE_REGISTER(ParticleOption<float>);
    TYPE_ADD_MEMBER(ParticleOption<float>, m_startMin, false, true, "Start Min");
    TYPE_ADD_MEMBER(ParticleOption<float>, m_startMax, false, true, "Start Max");
    TYPE_ADD_MEMBER(ParticleOption<float>, m_endMin, false, true, "End Min");
    TYPE_ADD_MEMBER(ParticleOption<float>, m_endMax, false, true, "End Max");

    TYPE_REGISTER(ParticleOption<Vec3>);
    TYPE_ADD_MEMBER(ParticleOption<Vec3>, m_startMin, false, true, "Start Min");
    TYPE_ADD_MEMBER(ParticleOption<Vec3>, m_startMax, false, true, "Start Max");
    TYPE_ADD_MEMBER(ParticleOption<Vec3>, m_endMin, false, true, "End Min");
    TYPE_ADD_MEMBER(ParticleOption<Vec3>, m_endMax, false, true, "End Max");

    TYPE_REGISTER(ParticleOption<Vec4>);
    TYPE_ADD_MEMBER(ParticleOption<Vec4>, m_startMin, false, true, "Start Min");
    TYPE_ADD_MEMBER(ParticleOption<Vec4>, m_startMax, false, true, "Start Max");
    TYPE_ADD_MEMBER(ParticleOption<Vec4>, m_endMin, false, true, "End Min");
    TYPE_ADD_MEMBER(ParticleOption<Vec4>, m_endMax, false, true, "End Max");

    TYPE_REGISTER(ParticleCircleEmitter);
    TYPE_ADD_MEMBER(ParticleCircleEmitter, m_innerRadius, false, true, "Inner Radius");
    TYPE_ADD_MEMBER(ParticleCircleEmitter, m_outerRadius, false, true, "Outer Radius");
    TYPE_ADD_MEMBER(ParticleCircleEmitter, m_spawnOffset, false, true, "Spawn Location Offset");
    TYPE_ADD_MEMBER(ParticleCircleEmitter, m_amount, false, true, "Spawn Amount");
    TYPE_ADD_MEMBER(ParticleCircleEmitter, m_rate, false, true, "Spawn Delay");
    TYPE_ADD_MEMBER(ParticleCircleEmitter, spawning, false, true, "Constant Spawning");
    TYPE_ADD_MEMBER(ParticleCircleEmitter, timedSpawning, false, true, "Timed Spawning");
    TYPE_ADD_MEMBER(ParticleCircleEmitter, timed, false, true, "Time");
    TYPE_ADD_MEMBER(ParticleCircleEmitter, outward, false, true, "Spawn Outwards");
    TYPE_ADD_MEMBER(ParticleCircleEmitter, inward, false, true, "Spawn Inwards");

    TYPE_REGISTER(ParticleBoxEmitter);
    TYPE_ADD_MEMBER(ParticleBoxEmitter, m_width, false, true, "Box Width");
    TYPE_ADD_MEMBER(ParticleBoxEmitter, m_height, false, true, "Box Height");
    TYPE_ADD_MEMBER(ParticleBoxEmitter, m_spawnOffset, false, true, "Spawn Offset");
    TYPE_ADD_MEMBER(ParticleBoxEmitter, m_amount, false, true, "Particle Amount");
    TYPE_ADD_MEMBER(ParticleBoxEmitter, m_rate, false, true, "Spawn Delay!");
    TYPE_ADD_MEMBER(ParticleBoxEmitter, spawning, false, true, "Constant Spawning?");
    TYPE_ADD_MEMBER(ParticleBoxEmitter, timedSpawning, false, true, "Timed Spawning?");
    TYPE_ADD_MEMBER(ParticleBoxEmitter, timed, false, true, "Time!");

    TYPE_REGISTER(Camera);
    TYPE_ADD_MEMBER(Camera, m_active, false, true, "Active",
      BUILD_FUNCTION(Camera::TweakSetActive));
    TYPE_ADD_MEMBER(Camera, m_FOV, false, true, "FOV",
      BUILD_FUNCTION(Camera::TweakSetFov));
    TYPE_SET_TWEAK_TYPE(Camera, AntTweak::TW_TYPE_COMPONENT);

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

    TYPE_REGISTER(SlotMachine);
    TYPE_SET_TWEAK_TYPE(SlotMachine, AntTweak::TW_TYPE_COMPONENT);

    TYPE_ADD_MEMBER(SlotMachine, m_stopTexture, false, true, "Still Texture",
      BUILD_FUNCTION(SlotMachine::TweakSetupSlots));
    TYPE_ADD_MEMBER(SlotMachine, m_spinTexture, false, true, "Spin Texture",
      BUILD_FUNCTION(SlotMachine::TweakSetupSlots));
    TYPE_ADD_MEMBER(SlotMachine, m_slotBackTexture, false, true, "Back Texture",
      BUILD_FUNCTION(SlotMachine::TweakSetupSlots));

    TYPE_ADD_MEMBER(SlotMachine, numSlots, false, true, "# Reels",
      BUILD_FUNCTION(SlotMachine::TweakSetNumSlots)); 

    TYPE_ADD_MEMBER(SlotMachine, slotOptions, false, true, "# Options",
      BUILD_FUNCTION(SlotMachine::TweakSetupSlots));

    TYPE_ADD_MEMBER(SlotMachine, startSpeed, false, true, "Start Speed");

    TYPE_ADD_MEMBER(SlotMachine, slotSize, false, true, "SlotScale");
    TYPE_ADD_MEMBER(SlotMachine, slotMargin, false, true, "Margin");
    TYPE_ADD_MEMBER(SlotMachine, slotOffset, false, true, "Offset");
    TYPE_ADD_MEMBER(SlotMachine, slotBackSize, false, true, "BackingScale");

    TYPE_ADD_MEMBER(SlotMachine, startTime, true, true, "Time");
    TYPE_ADD_MEMBER(SlotMachine, bonusTime, true, true, "Extra Time");

    TYPE_ADD_MEMBER(SlotMachine, useSpriteColor, false, true, "Use Sprite Color");
    TYPE_ADD_MEMBER(SlotMachine, backingColor, true, true, "BackColor");


    TYPE_REGISTER( AntTweak::TBar );
    TYPE_REGISTER_PTR( AntTweak::TBar* );

    TYPE_REGISTER(GamePadInput);
    TYPE_REGISTER_PTR(GamePadInput*);


    RegisterEnums();
  }
}