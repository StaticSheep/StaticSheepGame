/*****************************************************************
Filename: Object.h
*****************************************************************/

#pragma once

#include "Component.h"
#include <vector>
#include <memory>

namespace Framework
{

  // Forward Declaration
  class GameObject;
  class LuaComponent;

  // So life is easier
  typedef Handle* ComponentArray;
  typedef std::vector<Handle> ChildArray;

  class GameObject : public Generic
  {
    public:
      friend class Factory;

      // Adds a component to an object

      // Marks the object to be destroyed at the end of the frame

      // Gets the archetype of the object

      // Gets a vector of handles which are the objects children

      // Gets the parent of this object

      // Gets a specific child based on a GUID
      
      // Adds a child to the object

      // Sets the objects parent

      // If true, GetChild is enabled and the children list will be sorted
      bool fastChildSearch;



      // Checks to see if the object owns a specific component type

      // Checks to see if the object owns a specific component type

      // Serialization routine

      // Deserialization routine, var must be an empty object

      // Gets a component properly typed





      // Used from lua

      // The objects Globally Unique ID
      size_t guid;

      // The name of the object
      std::string name;

      // Name of the archetype
      std::string archetype;

      // Initializes the object, engine use only


    private:

      // Active flag, if the object is inactive it will be removed
      bool m_active;

      // Internally used to sort the children objects

      // Vector of the components belonging to this object
      Handle m_components[ecountComponents];

      // Vector of all lua components
      std::vector<Handle> m_luaComponents;

      // Vector of the children belonging to this object
      std::vector<Handle> m_children;
      typedef std::vector<Handle>::iterator ChildrenIt;

      // The parent of the object;
      Handle m_parent;

      friend class Factory;
      friend class GameSpace;
      friend class Archetype;
  };

  


};
#define Has(type) GetComponent<type>(e##type)

