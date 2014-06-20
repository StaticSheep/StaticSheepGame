/*****************************************************************
Filename: ComponentFactory.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#ifndef COMPCREATOR
#define COMPCREATOR

#include "Utilities.h"
#include "Space.h"

namespace Framework
{

  // Forward Declaration
  class GameComponent;

  class ComponentCreator
  {
    public:
      // Creator Constructor
      ComponentCreator(size_t type, unsigned int size) : typeID(type), m_size(size) {};

      // Type of component this constructor creates
      size_t typeID;

      // Creation function, abstract
      virtual GameComponent* Create(Space* space) = 0;
	  
	    // Removal function, abstract
	    virtual void Remove(Space* space, GameComponent* component) = 0;

	    // The size in bytes of the component
	    unsigned int m_size;
  };

  // Templated version of the Creator, used to have a copy
  // Of the creator for every single type of component
  template<typename T>
  class ComponentCreatorType : public ComponentCreator
  {
    ComponentCreatorType(size_t type) : ComponentCreator(type, sizeof(T)) {}

    // Override the create function with the type we want to make
    virtual GameComponent* Create(Space* space)
    {
      // Get a pointer to the ObjectAllocator for this type of component
		  ObjectAllocator* componentList = space->GetComponents(typeID);

      // Allocate a new component from the allocator
      T* component = (T*)componentList->Allocate();

      // Initialize the component
      new (component) T;

      // Setup the internals in the component
      component->self = space->m_handles.Insert(component); // Push a handle into the HandleManager
      component->space = space; // Reference back to the space
      component->typeID = typeID; // Set the componenets type

      // Sync the handles
      space->SyncHandles<T>(*componentList);
      
      return component;
    }
  };


};

//#define RegisterComponent(type) Core->AddComponentCreator(Hash(#type), new ComponentCreatorType<type>(Hash(#type)))

#endif