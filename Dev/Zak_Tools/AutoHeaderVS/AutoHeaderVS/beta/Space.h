/*****************************************************************
Filename: Space.h
*****************************************************************/

#pragma once
#include "ObjectAllocator.h"
#include "HandleManager.h"
#include "Component.h"

namespace Framework
{
  class GameComponent;
  class GameObject;

	class GameSpace
	{
  public:




    // Creates a component of a specific type

    // Creates a component of a specific type

    // Creates an empty object

    


    // Called at the end of the GameLogic update

    // Serialization routine

    // Deserialization routine, var must be an empty object

    // Gets the object allocator for a type of components

    // Gets a specific component from an object


    HookManager hooks;


    


    // =================================================


    
    
  private:
    // The collection of all game objects in this game space
    ObjectAllocator m_objects;

    // The collection of all components in this game space
    ObjectAllocator m_components[ecountComponents];

    // The handle manager for this game space
    HandleManager m_handles;

    // Name of the space
    std::string m_name;

    // Global Unique ID counter
    unsigned int m_guid;

    bool m_shuttingDown;

    bool m_paused;
    bool m_hidden;


    struct SerializerData
    {
      std::vector<std::string>* instanceData;
      bool includeGeneric;
      bool saveAllData;
      bool standalone;
    };

    friend class Engine;
    friend class Factory;
	};

  // Updates all handles to ensure that the HandleManager has accurate
  // pointers to the data which the handle needs to point at
}
