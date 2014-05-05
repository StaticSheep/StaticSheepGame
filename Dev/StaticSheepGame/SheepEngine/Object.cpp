/*****************************************************************
Filename: Object.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "Object.h"
#include <algorithm>

namespace Framework
{
  //Used to sort components using their type Id.
  struct ComponentSorter
  {
    bool operator()(GameComponent* left, GameComponent* right) const
    {
      return left->typeID < right->typeID;
    }
  };

  // Used to sort objects by their UID
  struct ObjectSorter
  {
    bool operator()(std::weak_ptr<GameObject> left, std::weak_ptr<GameObject> right) const
    {
      return left.lock().get()->GetID() < right.lock().get()->GetID();
    }
  };

  //Binary search a sorted array of components. yo thanks chris peters
  static GameComponent* BinaryComponentSearch(ComponentArray& components, size_t name)
  {
    size_t begin = 0;
    size_t end = components.size();

    while(begin < end)
    {
      size_t mid = (begin+end) / 2;
      if(components[mid]->typeID < name)
        begin = mid + 1;
      else
        end = mid;
    }

    if((begin < components.size()) && (components[begin]->typeID == name))
      return components[begin];
    else
      return NULL;
  }

  //Binary search a sorted array of Objects
  static std::shared_ptr<GameObject> BinaryChildSearch(ChildArray& children, size_t uid)
  {
    size_t begin = 0;
    size_t end = children.size();

    while(begin < end)
    {
      size_t mid = (begin + end) / 2;
      if(children[mid].lock()->GetID() < uid)
        begin = mid + 1;
      else
        end = mid;
    }

    if((begin < children.size()) && (children[begin].lock()->GetID() == uid))
      return children[begin].lock();
    else
      return std::shared_ptr<GameObject>();
  }

  
  /// <summary>
  /// Initializes each of the objects components seperate from the
  /// constructor initialization.
  /// </summary>
  void GameObject::Initialize()
  {
    for(ComponentIt it = _components.begin(); it != _components.end(); ++it)
    {	
      (*it)->_owner = this;
      (*it)->Initialize();
    }
  }


  /// <summary>
  /// Adds A new component to the GameObject
  /// </summary>
  /// <param name="typeId">The type identifier.</param>
  /// <param name="component">The component.</param>
  void GameObject::AddComponent(size_t typeId, GameComponent* component)
  {
    //Store the components type Id
    component->typeID = typeId;
    _components.push_back(component);

    //Sort the component array so binary search can be used to find components quickly.
    std::sort(_components.begin(), _components.end(), ComponentSorter() );
  }

  /// <summary>
  /// Gets the component.
  /// </summary>
  /// <param name="typeId">The type identifier.</param>
  /// <returns></returns>
  GameComponent * GameObject::GetComponent(size_t typeId)
  {
    return BinaryComponentSearch(_components, typeId);
  }

  /// <summary>
  /// Gets a specific child object based on a UID
  /// </summary>
  /// <param name="uid">The uid.</param>
  /// <returns></returns>
  std::shared_ptr<GameObject> GameObject::GetChild(size_t uid)
  {
    if (!fastChildSearch)
      return std::shared_ptr<GameObject>();

    return BinaryChildSearch(_children, uid);
  }

  void GameObject::AddChild(GameObject& obj)
  {
    _children.push_back(std::weak_ptr<GameObject>(obj.self));
    
    if (!fastChildSearch)
      return;
    // Sort the child array so binary search can be used to find children quickly
    std::sort(_children.begin(), _children.end(), ObjectSorter() );
  }


  /// <summary>
  /// Marks the object for deletion
  /// </summary>
  void GameObject::Destroy()
  { 
    // @TODO: Tell the object manager (Space?Layer?) to delete this when possible
  }

  GameObject::GameObject()
  {
    fastChildSearch = false;
    _uid = 0;
    // @TODO: Decide if archetype should be hashed string, integer, enum, or string
    _archetype = 0;
  }

  GameObject::GameObject(size_t uid)
  {
    fastChildSearch = false;
    _uid = uid;
    // @TODO: Decide if archetype should be hashed string, integer, enum, or string
    _archetype = 0;
  }

  GameObject::~GameObject()
  {
    // @TODO: Delete every component
  }


};