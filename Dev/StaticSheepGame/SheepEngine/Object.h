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

namespace Framework
{

  // Forward Declaration
  class GameObject;

  // So life is easier
  typedef Handle* ComponentArray;
  typedef std::vector<Handle> ChildArray;

  class GameObject
  {
    public:
      friend class Factory;

      /// <summary>
      /// Adds A new component to the GameObject
      /// </summary>
      /// <param name="typeId">The type identifier.</param>
      /// <param name="component">The component.</param>
      void AddComponent(GameComponent* component);

      /// <summary>
      /// Gets the component.
      /// </summary>
      /// <param name="typeId">The type identifier.</param>
      /// <returns></returns>
      GameComponent* GetComponent(size_t type);

      /// <summary>
      /// Initializes each of the objects components separate from the
      /// constructor initialization.
      /// </summary>
      void Initialize();

      /// <summary>
      /// Marks the object for deletion
      /// </summary>
      void Destroy();

      /// <summary>
      /// Gets the archetype.
      /// </summary>
      /// <returns></returns>
      const std::string& GetArchetype() const {return m_archetype;}

      /// <summary>
      /// Gets the array of children (Handles)
      /// </summary>
      /// <returns></returns>
      ChildArray& GetChildren() { return m_children; }

      /// <summary>
      /// Gets the parent object
      /// </summary>
      /// <returns></returns>
      GameObject* GetParent() { return space->GetHandles().GetAs<GameObject>(m_parent); }

      /// <summary>
      /// Gets a specific child object based on a UID
      /// </summary>
      /// <param name="uid">The uid.</param>
      /// <returns></returns>
      GameObject* GetChild(size_t uid);

      /// <summary>
      /// Adds the child.
      /// </summary>
      /// <param name="obj">The child handle</param>
      void AddChild(Handle obj);

      /// <summary>
      /// Sets the parent.
      /// </summary>
      /// <param name="obj">The parent handle.</param>
      void SetParent(Handle obj);

      // If true, GetChild is enabled and the children list will be sorted
      bool fastChildSearch;

      GameObject();

      ~GameObject();

      // Self handle
      Handle self;

      bool HasComponent(EComponent type);

      GameComponent* GetComponent(EComponent type);
      GameComponent* GetComponent(const char *type);

      static void Serialize(File& file, Variable var);

      template <typename T>
      T *GetComponent( EComponent type );

      GameSpace* space;
      size_t guid;

    private:

      bool m_active;

      bool ObjectSorter(Handle left, Handle right);
      bool ComponentSorter(Handle left, Handle right);

      // Vector of the components belonging to this object
      Handle m_components[ecountComponents];

      // Vector of the children belonging to this object
      ChildArray m_children;
      typedef ChildArray::iterator ChildrenIt;

      // The parent of the object;
      Handle m_parent;

      std::string m_archetype;

      friend class GameSpace;
  };

  
  template<typename T>
  T* GameObject::GetComponent(EComponent type)
  {
    return static_cast<T*>( GetComponent(type) );
  }


};

// This lets you do Object->Has(Trasform)
#define Has(type) GetComponent(e##type)
