/*****************************************************************
Filename: Object.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#ifndef GOBJECT_H
#define GOBJECT_H

#include "Component.h"
#include <vector>
#include <memory>

namespace Framework
{

  // Forward Declaration
  class GameObject;

  // So life is easier
  typedef std::vector<Handle> ComponentArray;
  typedef std::vector<Handle> ChildArray;

  class GameObject
  {
    public:
      //friend Factorywhateverimcallingit

      /// <summary>
      /// Adds A new component to the GameObject
      /// </summary>
      /// <param name="typeId">The type identifier.</param>
      /// <param name="component">The component.</param>
      void AddComponent(size_t typeID, GameComponent* component);

      /// <summary>
      /// Gets the component.
      /// </summary>
      /// <param name="typeId">The type identifier.</param>
      /// <returns></returns>
      GameComponent* GetComponent(size_t type);

      /// <summary>
      /// Gets a component and returns it typed correctly
      /// </summary>
      /// <param name="typeId">The type identifier.</param>
      /// <returns></returns>
      template<typename T>
      T* GetComponentType(size_t type);

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
      /// Gets the Unique ID;
      /// </summary>
      /// <returns></returns>
      size_t GetID() const {return _uid;}

      /// <summary>
      /// Gets the archetype.
      /// </summary>
      /// <returns></returns>
      size_t GetArchetype() const {return _archetype;}

      /// <summary>
      /// Gets the array of children (Handles)
      /// </summary>
      /// <returns></returns>
      ChildArray& GetChildren() { return _children; }

      /// <summary>
      /// Gets the parent object
      /// </summary>
      /// <returns></returns>
      GameObject* GetParent() { return space->GetHandles().GetAs<GameObject>(_parent); }

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

      // @TODO: Move into private and abstract out
      //Decide
      GameObject();
      // GameObject(size_t uid);
      // GameObject(size_t uid, size_t archetype);
      //Decide
      ~GameObject();

      Handle self;

    private:

      bool ObjectSorter(Handle left, Handle right);
      bool ComponentSorter(Handle left, Handle right);

      // Vector of the components belonging to this object
      ComponentArray _components;
      typedef ComponentArray::iterator ComponentIt;

      // Vector of the children belonging to this object
      ChildArray _children;
      typedef ChildArray::iterator ChildrenIt;

      // The parent of the object;
      Handle _parent;

      Space* space;
      
      uint64_t _uidTest;
      size_t _uid;
      size_t _archetype;

  };

  
  template<typename T>
  T* GameObject::GetComponentType(size_t typeId)
  {
    return static_cast<T*>( GetComponent(typeId) );
  }


};

// This lets you do Object->Has(Trasform)
#define Has(type) GetComponentType(CT_##type)

#endif