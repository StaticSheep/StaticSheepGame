/*****************************************************************
Filename: Space.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "Space.h"
#include "Object.h"

namespace Framework
{

  GameSpace::GameSpace() :
    m_objects(sizeof(GameObject))
  {
    for(unsigned i = 0; i < ecountComponents; ++i)
    {
      if (FACTORY->m_componentCreators[i])
        m_components[i].Initialize(FACTORY->m_componentCreators[i]->m_size, 10);
    }
  }

  GameSpace::~GameSpace()
  {
    // Should probably clean stuff up
  }

  GameComponent* GameSpace::CreateComponent(EComponent type)
  {
    return FACTORY->m_componentCreators[type]->Create(this);
  }

  ObjectAllocator* GameSpace::GetComponents(EComponent type)
  {
    return &m_components[type];
  }

  GameComponent* GameSpace::GetComponent(EComponent type, Handle owner)
  {
    return GetHandles().GetAs<GameObject>(owner)->GetComponent(type);
  }

  void GameSpace::RemoveComponent(GameComponent* comp)
  {
    FACTORY->m_componentCreators[comp->typeID]->Remove(this, comp);
  }

  void GameSpace::RemoveComponent(Handle handle)
  {
    GameComponent* comp = GetHandles().GetAs<GameComponent>(handle);
    RemoveComponent(comp);
  }

  GameObject* GameSpace::CreateEmptyObject()
  {
    // Allocate space for a new object
    GameObject* object = (GameObject*)m_objects.Allocate();

    // Re-construct the game object
    new (object) GameObject();

    // Register the object inside of the handle manager
    object->self = GetHandles().Insert(object);
    object->space = this;
    object->guid = m_guid++;
    object->m_active = true;

    // Check to see if we need to sync the GameObject handles
    SyncHandles<GameObject>(m_objects);

    return object;
  }

  void GameSpace::RemoveGameObject(GameObject* object)
  {
    // Remove the object from the handle manager
    GetHandles().Remove(object->self);

    // Remove the object from the object list
    GameObject* moved = (GameObject*)m_objects.Free(object);

    // When we freed the object the right most element
    // was moved to take the old objects place so now
    // we have to update the handle manager and tell it
    // to point at the right place in memory
    if (moved)
      GetHandles().Update(moved, moved->self);
  }

  HandleManager& GameSpace::GetHandles()
  {
    return m_handles;
  }

  void GameSpace::Cleanup()
  {
    for (unsigned int i = 0; i < m_objects.Size(); ++i)
    {
      GameObject* obj = (GameObject*)m_objects[i];
      if (!obj->m_active)
      {
        // If the object is not active, time to destroy it
        for (unsigned int j = 0; j < ecountComponents - 1; ++j)
        {
          // Get the enum type of the component
          EComponent type = (EComponent)j;

          // Check to see if the Object has that type of component
          if (obj->HasComponent(type))
          {
            // Get the component
            GameComponent* comp = obj->GetComponent(type);
            comp->Remove(); // Remove the component

            // Free the component and update any handles
            GameComponent* moved = (GameComponent*)GetComponents(type)->Free(comp);
            if (moved)
              m_handles.Update(moved, moved->self);
          }
        } // End component loop

        RemoveGameObject(obj);
      } // End object not active loop
    }
  }



}