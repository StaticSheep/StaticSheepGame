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

E_NAME(Component)
  E_ENTRY(Transform)
  E_ENTRY(Tester)
  E_ENTRY(Sprite)
  E_ENTRY(GamePad)
  E_ENTRY(SoundEmitter)
  E_ENTRY(SoundPlayer)
  E_ENTRY(LuaComponent)
  E_ENTRY(countComponents)
E_END(Component)