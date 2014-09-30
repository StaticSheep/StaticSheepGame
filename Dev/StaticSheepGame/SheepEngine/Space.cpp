/*****************************************************************
Filename: Space.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "Space.h"
#include "Object.h"
#include "File.h"
#include "Variable.h"
#include "SheepPhysics.h"

#include <iostream>
#include "AntTweakModule.h"

namespace Framework
{

  GameSpace::GameSpace() :
    m_objects(sizeof(GameObject), 10),
    m_shuttingDown(false),
    m_paused(false),
    m_hidden(false),
    m_valid(true),
    m_guid(0),
	m_pSpace(nullptr)
  {
    for(unsigned i = 0; i < ecountComponents; ++i)
    {
      if (FACTORY->m_componentCreators[i])
        m_components[i].Initialize(FACTORY->m_componentCreators[i]->m_size, 10);
    }
    
    hooks.space = this;
  }

  GameSpace::~GameSpace()
  {
    m_objects.~ObjectAllocator();

    for(unsigned i = 0; i < ecountComponents; ++i)
    {
      if (FACTORY->m_componentCreators[i])
        m_components[i].Clear();
    }

    if (tweakHandle != Handle::null)
    {
      AntTweak::TBar* objBar = ATWEAK->GetBar(tweakHandle);
      ATWEAK->RemoveBar(objBar);
    }

	if (m_pSpace)
		PHYSICS->DeleteSpace(m_pSpace);

	m_pSpace = nullptr;

    // Should probably clean stuff up
  }

  GameComponent* GameSpace::CreateComponent(EComponent type)
  {
    return FACTORY->m_componentCreators[type]->Create(this);
  }

  GameComponent* GameSpace::CreateComponent(size_t type)
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
    m_handles.SyncHandles<GameObject>(m_objects);

    return object;
  }

  GameObject* GameSpace::GetGameObject(Handle handle)
  {
    return m_handles.GetAs<GameObject>(handle);
  }

  void GameSpace::RemoveGameObject(GameObject* object)
  {
    //std::cout << "Removing game object: " << object->self.operator size_t() << "\n";

    // Removes all components
    object->~GameObject();

    Lua::CallFunc(ENGINE->Lua(), "RemoveGameObject", m_name, object->self.operator size_t());

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

#if USE_ANTTWEAKBAR
    UpdateTweakBar();
#endif
  }

  HandleManager& GameSpace::GetHandles()
  {
    return m_handles;
  }

  void GameSpace::Cleanup()
  {
    std::vector<Handle> removeList;

    for (unsigned int i = 0; i < m_objects.Size(); ++i)
    {
      GameObject* obj = (GameObject*)m_objects[i];
      if (!obj->m_active || m_shuttingDown)
      {
        removeList.push_back(obj->self);   
      } // End object not active loop
    }

    for (unsigned int i = 0; i < removeList.size(); ++i)
    {
      RemoveGameObject(this->GetGameObject(removeList[i]));
    }

    removeList.clear();
  }

  // Serialization routine
  void GameSpace::Serialize(File& file, Variable var)
  {
    SerializerData* sd = (SerializerData*)Serializer::Get()->GetUserData();

    std::vector<std::string>* objInstanceData = sd->instanceData;
    bool includeGeneric = sd->includeGeneric;
    bool allData = sd->saveAllData;
    bool standalone = sd->standalone;

    GameSpace* space = (GameSpace*)var.GetData();

    file.Write("%s\n", space->GetName().c_str());

    for (auto it = space->m_objects.begin<GameObject>(); it != space->m_objects.end<GameObject>(); ++it)
    {

      if (it->archetype.length() == 0 && includeGeneric || standalone)
      {
        // We couldn't find an archetype, so we are going to
        // serialize the object into the level

        // Set the variable var to the game object pointer
        Variable var = *it;

        it->Serialize(file, var);
        continue;
      }

      // Write the name of the archetype
      file.Write("ach_%s\n", it->archetype.c_str());

      // Check to see if we have the object's archetypes cached in our map
      const Archetype& objectArchetype = FACTORY->GetArchetype(it->archetype);

      if (&objectArchetype == &Archetype::null || objectArchetype.name != it->name)
      {
        // Get the name member of the object
        const Member* nameMember = GET_TYPE(GameObject)->GetMember("name");

        // Pad and write the name of the member
        file.Write("  %s ", nameMember->Name());

        // Create a variable and write to the file
        Variable name(nameMember->Type(), (char*)it + nameMember->Offset());
        name.GetTypeInfo()->Serialize(file, name);
      }

      // All data saving routines
      if (allData)
      {

        // Our object archetype is valid
        if (&objectArchetype != &Archetype::null)
        {
          // We have the archetype so we will only serialize the differences
          objectArchetype.SerializeDifferences(it, file);
          continue;
        }
      }

      // We are provided instance data to save
      if (objInstanceData)
      {
        for (unsigned j = 0; j < objInstanceData->size(); ++j)
        {
          // Set the variable var to the game object pointer
          Variable var = &(*it);

          // Pull the instance from the vector
          // We have to use the operator[] bit to avoid some un-necessary casting
          const char *instance = objInstanceData->operator[](j).c_str();

          // Now lets pull out the member
          const Member *member = FACTORY->GetComponentMember(instance, var);

          ErrorIf(!member, "Space serialization", "Member is invalid!");

          // Variable var now is the member variable we wanted
          // Write the name of the variable
          file.Write("  %s ", instance);
          // Serialize the variable
          var.Serialize(file);
        }
      }

    } // end for

  }

  // Deserialization routine, var must be an empty object
  void GameSpace::Deserialize(File& file, Variable var)
  {
    GameSpace* space = (GameSpace*)var.GetData();

    // Pointer to use
    GameObject* obj = nullptr;

    const TypeInfo* typeinfo;

    for (;;)
    {
      // Read in a line
      std::string line = file.GetLine(" \n:");

      // Check to see if we read the entire file
      if (!file.Validate())
        break;

      // Skip white spaces
      if (line.empty())
        continue;

      typeinfo = GET_STR_TYPE(line.c_str());
      if (typeinfo && strcmp(typeinfo->Name(), "GameObject") == 0 )
      {
        file.SeekByOffset(-int(line.length()) - 2);
        // Create an empty object
        obj = space->CreateEmptyObject();
        Variable var = *obj; // Set the object as a variable

        // Deserialize the file into the object
        GET_TYPE(GameObject)->Deserialize(file, var);

        if (obj->archetype.length() > 0)
          if (&FACTORY->GetArchetype(obj->archetype) == &Archetype::null)
            FACTORY->ArchetypeMap[obj->archetype].CopyObject(obj);

        continue;
      }

      // If we didn't hit a component, check to see if by chance we hit an archetype
      if (!GET_ENUM(Component)->IsAnEntry(line.c_str()) && line.substr(0, FACTORY->ArchetypePrefix.length()) == FACTORY->ArchetypePrefix)
      {
        // Oh we did, goodie
        std::string aName = line;
        aName = FACTORY->ArchetypeFilePath + aName.substr(FACTORY->ArchetypePrefix.length(), line.length() - FACTORY->ArchetypePrefix.length());
        aName += FACTORY->ArchetypeFileExtension;
        obj = FACTORY->LoadObjectFromArchetype(space, aName.c_str());
        continue;
      }

      if (obj == nullptr)
        continue;

      if (line == "name")
      {
        // Get the name member of the object
        const Member* nameMember = GET_TYPE(GameObject)->GetMember("name");

        // Create a variable and write to the variable
        Variable name(nameMember->Type(), (char*)obj + nameMember->Offset());
        name.GetTypeInfo()->Deserialize(file, name);
        continue;
      }

      // Get the rest of the instance data from the archetype
      line += ":" + file.GetLine(" ");

      // Read the instance data in now
      Variable var = obj;

      // Get the member
      const Member* member = FACTORY->GetComponentMember(line.c_str(), var);

      // Deserialize the member
      member->Type()->Deserialize(file, var);
    }
  }

  const std::string GameSpace::GetName() const
  {
    return m_name;
  }

  void GameSpace::SetPaused(bool paused)
  {
    m_paused = paused;

    Lua::CallFunc(ENGINE->Lua(), "PauseGameSpace", m_name, paused);
  }

  bool GameSpace::Paused()
  {
    return m_paused;
  }

  void GameSpace::SetHidden(bool hidden)
  {
    m_hidden = hidden;

    // Lua hide
  }

  bool GameSpace::Hidden()
  {
    return m_hidden;
  }

  void GameSpace::Clear()
  {
    for (unsigned int i = 0; i < m_objects.Size(); ++i)
    {
      GameObject* obj = (GameObject*)m_objects[i];
      obj->m_active = false;
    }
  }

  GameObject* GameSpace::CreateObjectFromArchetype(const char* name)
  {
    return FACTORY->LoadObjectFromArchetype(this, name);
  }

  GameSpace* GameSpace::CopyGameSpace(const char* new_name)
  { 
    // Make the new space
    //GameSpace* space = ENGINE->CreateSpace(new_name);

    //for (GameObject* it = m_objects.begin<GameObject>(); it != m_objects.end<GameObject>(); ++it)
    //{
    //  // Create a new game object and copy over all data
    //  GameObject* obj = space->CreateEmptyObject();

    //  GET_TYPE(GameObject)->Copy(obj, it);

    //  obj->Initialize();
    //}

    FACTORY->SaveSpaceToFile(this, "temp_space", true);

    GameSpace* space = FACTORY->LoadSpace("temp_space");

    return space;
  }

  
  void GameSpace::Tweak()
  {
    GET_TYPE(GameSpace)->Tweak(nullptr, this, nullptr, nullptr);
  }

}