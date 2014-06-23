/*****************************************************************
Filename: Object.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "Object.h"
#include <algorithm>
#include <functional>
#include <iterator>

namespace Framework
{

  //Binary search a sorted array of components. yo thanks chris peters
  static GameComponent* BinaryComponentSearch(Space* space, ComponentArray& components, size_t name)
  {
    size_t begin = 0;
    size_t end = components.size();

    while(begin < end)
    {
      size_t mid = (begin+end) / 2;
      if(space->GetHandles().GetAs<GameComponent>(components[mid])->typeID < name)
        begin = mid + 1;
      else
        end = mid;
    }

    if((begin < components.size()) && (space->GetHandles().GetAs<GameComponent>(components[begin])->typeID == name))
      return space->GetHandles().GetAs<GameComponent>(components[begin]);
    else
      return NULL;
  }

  //Binary search a sorted array of Objects
  static GameObject* BinaryChildSearch(Space* space, ChildArray& children, size_t uid)
  {
    
    size_t begin = 0;
    size_t end = children.size();

    while(begin < end)
    {
      size_t mid = (begin + end) / 2;
      if (space->GetHandles().GetAs<GameObject>(children[mid])->GetID() < uid)
        begin = mid + 1;
      else
        end = mid;
    }

    if((begin < children.size()) && (space->GetHandles().GetAs<GameObject>(children[begin])->GetID() == uid))
      return space->GetHandles().GetAs<GameObject>(children[begin]);
    else
      return NULL;
      
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

  GameObject::~GameObject()
  {
    // @TODO: Delete every component
  }

  
  /// <summary>
  /// Initializes each of the objects components separate from the
  /// constructor initialization.
  /// </summary>
  void GameObject::Initialize()
  {
    GameComponent* component;
    for(ComponentIt it = _components.begin(); it != _components.end(); ++it)
    {
      component = space->GetHandles().GetAs<GameComponent>(*it);
      component->owner = self;
      component->Initialize();
    }
  }

  bool GameObject::ObjectSorter(Handle left, Handle right)
  {
    return space->GetHandles().GetAs<GameObject>(left)->GetID() > space->GetHandles().GetAs<GameObject>(right)->GetID();
  }

  bool GameObject::ComponentSorter(Handle left, Handle right)
  {
    return space->GetHandles().GetAs<GameComponent>(left)->typeID > space->GetHandles().GetAs<GameComponent>(right)->typeID;
  }

  /// <summary>
  /// Adds the component.
  /// </summary>
  /// <param name="typeId">The type identifier.</param>
  /// <param name="component">The component.</param>
  void GameObject::AddComponent(size_t typeId, GameComponent* component)
  {
    // Store the components type Id
    component->typeID = typeId;
    // Set the component's owner to ourself
    component->owner = self;
    _components.push_back(component->self);

    // Sort the component array so binary search can be used to find components quickly.
    std::sort(_components.begin(), _components.end(), std::bind(&GameObject::ComponentSorter, this, std::placeholders::_1, std::placeholders::_2));
  }

  /// <summary>
  /// Gets a component.
  /// </summary>
  /// <param name="typeId">The type identifier.</param>
  /// <returns></returns>
  GameComponent * GameObject::GetComponent(size_t typeId)
  {
    return BinaryComponentSearch(space, _components, typeId);
  }

  /// <summary>
  /// Gets the child.
  /// </summary>
  /// <param name="uid">The uid.</param>
  /// <returns></returns>
  GameObject* GameObject::GetChild(size_t uid)
  {
    if (!fastChildSearch)
      return NULL;

    return BinaryChildSearch(space, _children, uid);
  }

  /// <summary>
  /// Adds the child.
  /// </summary>
  /// <param name="obj">The object.</param>
  void GameObject::AddChild(Handle obj)
  {
    _children.push_back(obj);

    if (!fastChildSearch)
      return;
    // Sort the child array so binary search can be used to find children quickly
    std::sort(_children.begin(), _children.end(), std::bind(&GameObject::ObjectSorter, this, std::placeholders::_1, std::placeholders::_2) );
  }

  /// <summary>
  /// Sets the parent.
  /// </summary>
  /// <param name="obj">The object.</param>
  void GameObject::SetParent(Handle obj)
  {
    // Set the parent
    _parent = obj;

    // Add the child onto the parent
    space->GetHandles().GetAs<GameObject>(obj)->AddChild(self);
  }


};