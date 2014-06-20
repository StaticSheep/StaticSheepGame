/*****************************************************************
Filename: component.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#ifndef GCOMPONENT_H
#define GCOMPONENT_H

#include "Space.h"

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

	    // The handle to the owner of this component
      Handle owner;

	    // The handle to the component itself
	    Handle self;

      // Each type of component has it's own unique ID
      size_t typeID;

      // Pointer to the space which the component belongs to
      Space* space;

    protected:
      // Destructor, can be accessed from inherited classes
      ~GameComponent();

  };

};

#endif