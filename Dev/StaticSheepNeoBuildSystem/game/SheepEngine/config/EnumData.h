/*****************************************************************
Filename: EnumData.h
Project: 
Author(s): Zachary Nawar (Primary)

This is the enumeration manifest file where special enumerations
are processed by the engine. Essentially this is what happens to
every single enumeration declared here.

(Where *** is what is inside E_NAME() )
(Where $$$ is what is inside E_ENTRY() )

- Creates a C++ enumeration E*** 
  - Each Entry inside of the enumeration is e$$$
- Creates an instance of the Enum class named ENUM***

The Enum class is cool because it contains a hash map of $$$
to the corresponding C++ enumeration index

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

E_NAME(Generics)
  E_ENTRY(GameComponent)
  E_ENTRY(GameObject)
  E_ENTRY(Abstract)
E_END(Generics)

E_NAME(Weapons)
  E_ENTRY(Pistol)
  E_ENTRY(Automatic)
  E_ENTRY(Shotgun)
  E_ENTRY(NoWeapon) //keep this at the end
E_END(Weapons)


E_NAME(Component)
  E_ENTRY(Transform)
  E_ENTRY(Tester)
  E_ENTRY(Sprite)
  E_ENTRY(Camera)
  E_ENTRY(AniSprite)
  E_ENTRY(PointLight)
  E_ENTRY(GamePad)
  E_ENTRY(BoxCollider)
  E_ENTRY(CircleCollider)
  E_ENTRY(SoundEmitter)
  E_ENTRY(SoundPlayer)
  E_ENTRY(PlayerController)
  E_ENTRY(Bullet_Default)
  E_ENTRY(ElevatorPlat)
  E_ENTRY(Level1_Logic)
  E_ENTRY(GiantKillBox)
  E_ENTRY(Grinder)
  E_ENTRY(Explosion)
  E_ENTRY(WeaponPickup)
  E_ENTRY(WarningText)
  E_ENTRY(BackgroundPan)
  E_ENTRY(Cheats)
  E_ENTRY(SlotMachine)
  E_ENTRY(AimingArrow)



  E_ENTRY(LuaComponent)
  E_ENTRY(countComponents) //keep this at the end
E_END(Component)