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
  static GameComponent* BinaryComponentSearch(GameSpace* space, Handle* components, size_t name)
  {
    size_t begin = 0;
    size_t end = ecountComponents;

    while(begin < end)
    {
      size_t mid = (begin+end) / 2;
      if(space->GetHandles().GetAs<GameComponent>(components[mid])->typeID < name)
        begin = mid + 1;
      else
        end = mid;
    }

    if((begin < ecountComponents) && (space->GetHandles().GetAs<GameComponent>(components[begin])->typeID == name))
      return space->GetHandles().GetAs<GameComponent>(components[begin]);
    else
      return NULL;
  }

  //Binary search a sorted array of Objects
  static GameObject* BinaryChildSearch(GameSpace* space, ChildArray& children, size_t uid)
  {
    
    size_t begin = 0;
    size_t end = children.size();

    while(begin < end)
    {
      size_t mid = (begin + end) / 2;
      if (space->GetHandles().GetAs<GameObject>(children[mid])->guid < uid)
        begin = mid + 1;
      else
        end = mid;
    }

    if((begin < children.size()) && (space->GetHandles().GetAs<GameObject>(children[begin])->guid == uid))
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
    for (size_t i = 0; i < ecountComponents; ++i)
    {
      component = space->GetHandles().GetAs<GameComponent>(m_components[i]);
      component->owner = self;
      component->Initialize();
    }
  }

  bool GameObject::ObjectSorter(Handle left, Handle right)
  {
    return space->GetHandles().GetAs<GameObject>(left)->guid > space->GetHandles().GetAs<GameObject>(right)->guid;
  }

  bool GameObject::ComponentSorter(Handle left, Handle right)
  {
    return space->GetHandles().GetAs<GameComponent>(left)->typeID > space->GetHandles().GetAs<GameComponent>(right)->typeID;
  }

  /// <summary>
  /// Adds the component.
  /// </summary>
  /// <param name="component">The component.</param>
  void GameObject::AddComponent(GameComponent* component)
  {
    // Set the component's owner to ourself
    component->owner = self;
    m_components[component->typeID] = component->self;

    // Sort the component array so binary search can be used to find components quickly.
    std::sort(&m_components[0], &m_components[ecountComponents], std::bind(&GameObject::ComponentSorter, this, std::placeholders::_1, std::placeholders::_2));
  }

  /// <summary>
  /// Gets a component.
  /// </summary>
  /// <param name="typeId">The type identifier.</param>
  /// <returns></returns>
  GameComponent * GameObject::GetComponent(size_t typeId)
  {
    return BinaryComponentSearch(space, m_components, typeId);
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

    return BinaryChildSearch(space, m_children, uid);
  }

  /// <summary>
  /// Adds the child.
  /// </summary>
  /// <param name="obj">The object.</param>
  void GameObject::AddChild(Handle obj)
  {
    m_children.push_back(obj);

    if (!fastChildSearch)
      return;
    // Sort the child array so binary search can be used to find children quickly
    std::sort(m_children.begin(), m_children.end(), std::bind(&GameObject::ObjectSorter, this, std::placeholders::_1, std::placeholders::_2) );
  }

  /// <summary>
  /// Sets the parent.
  /// </summary>
  /// <param name="obj">The object.</param>
  void GameObject::SetParent(Handle obj)
  {
    // Set the parent
    m_parent = obj;

    // Add the child onto the parent
    space->GetHandles().GetAs<GameObject>(obj)->AddChild(self);
  }

  bool GameObject::HasComponent( EComponent type )
  {
    return m_components[type] != Handle::null;
  }

  GameComponent* GameObject::GetComponent(EComponent type)
  {
    return space->GetHandles().GetAs<GameComponent>(m_components[type]);
  }

  GameComponent* GameObject::GetComponent(const char *type)
  {
    if (GET_ENUM(Component)->IsAnEntry(type))
    {
      EComponent i = (EComponent)GET_ENUM(Component)->GetIndexFromString(type);
      return GetComponent(i);
    }

    return NULL;
  }

  void GameObject::Serialize( File& file, Variable var )
  {
    GameObject *o = &var.GetValue<GameObject>( );
    Serializer *s = Serializer::Get( );
    int& pad = s->GetPadLevel( );

    s->Padding( file, pad );
    file.Write( "GameObject\n" );
    s->Padding( file, pad++ );
    file.Write( "{\n" );
    s->Padding( file, pad );

    for(unsigned i = 0; i < ecountComponents; ++i)
    {
      EComponent type = (EComponent)i;
      if (o->HasComponent( type ))
      {
        const TypeInfo *typeInfo = FACTORY->GetComponentType( type );
        Variable v( typeInfo, o->GetComponent( type ) );
        v.Serialize( file );

        if(i != ecountComponents - 1)
        {
          file.Write( "\n" );
          s->Padding( file, pad );
        }
      }
    }

    s->Padding( file, --pad );
    file.Write( "}\n" );
  }

};