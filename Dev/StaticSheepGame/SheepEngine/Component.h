/*****************************************************************
Filename: component.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#ifndef GCOMPONENT_H
#define GCOMPONENT_H

namespace Framework
{

  // Forward Declaration, got this idea from Chris Peters
  class GameObject;

  class GameComponent {
    public:
      // Game Objects have full access to the component
      friend class GameObject;

      // Initialized upon Object creation
      // After all components are constructed
      virtual void Initialize(){};

      //virtual void Serialize(){};

      // Link to the object which owns the component
      GameObject* GetOwner() { return _owner; };

      // Each type of component has it's own unique ID
      size_t typeID;

    protected:
      // Destructor, can be accesd from inherited classes
      ~GameComponent();

    private:
      GameObject* _owner;
  };

};

#endif