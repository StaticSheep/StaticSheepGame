/*****************************************************************
Filename: component.h
Project: 

*****************************************************************/

#pragma once

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



      // Destructor, can be accessed from inherited classes


	    // The handle to the owner of this component
      Handle owner;

      // Each type of component has it's own unique ID
      EComponent typeID;

      
      


      // Gets the owning game object

      const TypeInfo* typeinfo;

  };

  
  


};

