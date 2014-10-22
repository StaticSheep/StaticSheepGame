/*****************************************************************
Filename: component.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "types/generic/Generic.h"

namespace Framework
{
  class GameSpace;
  class Factory;

  extern Factory* FACTORY;

  class GameComponent : public Generic
  {
    public:
      // Game Objects have full access to the component
      friend class GameObject;

      // Initialized upon Object creation
      // After all components are constructed
      virtual void Initialize() {};

      virtual void Remove() {};

      GameComponent() : Generic(eGameComponent) {};

      // Destructor, can be accessed from inherited classes
      virtual ~GameComponent() {};

      //virtual void Serialize(){};

	    // The handle to the owner of this component
      Handle owner;

      // Each type of component has it's own unique ID
      EComponent typeID;

      
      
      virtual bool operator==(const GameComponent& rhs) const;
      bool operator!=(const GameComponent& rhs) const;

      virtual GameComponent& operator=(const GameComponent& rhs);

      // Gets the owning game object
      GameObject* GetOwner();

      const TypeInfo* typeinfo;

      friend class TypeInfo;

  };

  
  


};
