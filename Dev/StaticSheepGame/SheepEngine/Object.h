/*****************************************************************
Filename: Object.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "Component.h"
#include <vector>
#include <memory>

namespace AntTweak
{
  class TBar;
}

namespace Framework
{

  class TweakObjComp
  {
  public:
    GenericLookup gl;
    EComponent compType;
    std::string luaCompName;
  };

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
      void AddComponent(GameComponent* component);

      // Removes a component on the object
      void RemoveComponent(EComponent type);

      // Marks the object to be destroyed at the end of the frame
      void Destroy();

      // Gets the archetype of the object
      const std::string& GetArchetype() const {return archetype;}

      // Gets a vector of handles which are the objects children
      ChildArray& GetChildren() { return m_children; }

      // Gets the parent of this object
      GameObject* GetParent() { return space->GetHandles().GetAs<GameObject>(m_parent); }

      // Gets a specific child based on a GUID
      GameObject* GetChild(size_t guid);
      
      // Adds a child to the object
      void AddChild(Handle obj);

      // Sets the objects parent
      void SetParent(Handle obj);

      // If true, GetChild is enabled and the children list will be sorted
      bool fastChildSearch;

      GameObject();
      
      

      ~GameObject();

      // Checks to see if the object owns a specific component type
      bool HasComponent(EComponent type) const;

      // Checks to see if the object owns a specific component type
      bool HasComponent(size_t type) const;


      // Copies one object into this object
      void Copy(GameObject& rhs);
      

      // Serialization routine
      static void Serialize(File& file, Variable var);

      // Deserialization routine, var must be an empty object
      static void Deserialize(File& file, Variable var);

      // AntTweak bar creator
      static void CustomTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label);

      // AntTweak custom callback for changing the anttweak bar window's name
      //static void TweakUpdate

      // Opens a bar to tweak the object
      void TweakObject(void);
      // Updates the teak bar for the object
      void UpdateTweakBar(void);

      // Gets a component properly typed
      template <typename T>
      T *GetComponent( EComponent type );

      GameComponent* GetComponent(unsigned int type);
      GameComponent* GetComponent(const char* type);

      LuaComponent* GetLuaComponent(unsigned int index);

      Handle GetComponentHandle(EComponent type);
      Handle GetComponentHandle(const char* type);


      // Used from lua
      void LuaGetComponent(size_t type);
      bool LuaHasComponent(size_t type);

      // The objects Globally Unique ID
      size_t guid;

      // The name of the object
      std::string name;

      // Name of the archetype
      std::string archetype;

      // Initializes the object, engine use only
      virtual void Initialize();

      //GameObject& operator=(const GameObject& rhs);

      // Handle to the AntTweakbar for tweaking this object
      Handle tweakHandle;
      // Pointer to the GenericLookup struct used for spaces finding this object
      GenericLookup* tweakLookup;
      // Are we choosing a component to add?
      bool tweakListComponents;
      // Are we deleting a component?
      bool tweakDeleteComponent;
      // Are we really deleting the object
      bool tweakDeleteObject;
      // Pointer to a vector full of clientdata pointers for adding a component through tweakbar
      std::vector<TweakObjComp*>* tweakCCompCallbacks;
      std::vector<TweakObjComp*>* tweakLuaCompCallbacks;

    private:

      // Active flag, if the object is inactive it will be removed
      bool m_active;

      // Internally used to sort the children objects
      bool ObjectSorter(Handle left, Handle right);

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

  
  template<typename T>
  T* GameObject::GetComponent(EComponent type)
  {
    return space->GetHandles().GetAs<T>(m_components[type]);
  }


};

// This lets you do Object->Has(Trasform)
#define Has(type) GetComponent<type>(e##type)
