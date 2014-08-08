/*****************************************************************
Filename: ComponentFactory.h
Project: 

*****************************************************************/

#ifndef COMPCREATOR
#define COMPCREATOR

#include "Utilities.h"
#include "Space.h"

namespace Framework
{

  // Forward Declaration
  class GameComponent;

  struct ComponentCreator
  {
    // Creator Constructor

    // Type of component this constructor creates
    EComponent typeID;

    // Creation function, abstract

    // Simply creates a new component
	  
	  // Removal function, abstract

	  // The size in bytes of the component
	  unsigned int m_size;
  };

  // template version of the Creator, used to have a copy
  // Of the creator for every single type of component
  template<typename T>
  struct ComponentCreatorType : public ComponentCreator
  {

    // Override the create function with the type we want to make
    {
      // Get a pointer to the ObjectAllocator for this type of component

      // Allocate a new component from the allocator

      // Zero it out

      // Initialize the component

      // Setup the internals in the component
      component->space = space; // Reference back to the space
      component->typeID = typeID; // Set the components type

      // Sync the handles
      
      return component;
    }

    {
      // Allocate a new component

      // Set the type
      comp->typeID = typeID; // Set the components type

      // Send back
      return comp;
    }

    {

      // Call the destructor on the component

      // Resync handles
    }
  };


};


#endif
