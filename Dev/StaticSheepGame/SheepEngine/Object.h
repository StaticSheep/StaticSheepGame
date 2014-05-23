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
  typedef std::vector<GameComponent*> ComponentArray;
  typedef std::vector<std::weak_ptr<GameObject>> ChildArray;

  class GameObject
  {
    public:
      //friend Factorywhateverimcallingit

      // Get a component based on the type
      GameComponent* GetComponent(size_t type);

      // Get the component already typed
      template<typename T>
      T* GetComponentType(size_t type);

      // Initialization function of the Object
      void Initialize();

      // Marks the object to be deleted
      void Destroy();

      // Adds a component onto an object from the serializer
      void AddComponent(size_t typeID, GameComponent* component);

      // Get the objects unique ID
      size_t GetID() const {return _uid;}

      // Get the objects archetype ID
      size_t GetArchetype() const {return _archetype;}

      // Get the vector of child objects
      ChildArray& GetChildren() { return _children; }

      // Get the parent of the object
      std::shared_ptr<GameObject> GetParent() { return _parent.lock(); }

      // Gets a specific child object
      std::shared_ptr<GameObject> GetChild(size_t uid);

      // Adds a child onto the object
      void AddChild(GameObject &obj);
      void AddChild(std::shared_ptr<GameObject> obj);

      // Parents the object to another
      void SetParent(GameObject &obj);
      void SetParent(std::shared_ptr<GameObject> obj);

      // If true, GetChild is enabled and the children list will be sorted
      bool fastChildSearch;

      // @TODO: Move into private and abstract out
      //Decide
      GameObject();
      GameObject(size_t uid);
      GameObject(size_t uid, size_t archetype);
      //Decide
      ~GameObject();

      std::weak_ptr<GameObject> self;

    private:

      // Vector of the components belonging to this object
      ComponentArray _components;
      typedef ComponentArray::iterator ComponentIt;

      // Vector of the childen belonging to this object
      ChildArray _children;
      typedef ChildArray::iterator ChildrenIt;

      // The parent of the object;
      std::weak_ptr<GameObject> _parent;

      
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