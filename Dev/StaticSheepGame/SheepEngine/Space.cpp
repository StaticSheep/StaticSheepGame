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

  // Serialization routine
  void GameSpace::Serialize(File& file, Variable var)
  {
    SerializerData* sd = (SerializerData*)Serializer::Get()->GetUserData();

    std::vector<std::string>* objInstanceData = sd->instanceData;
    bool includeGeneric = sd->includeGeneric;
    bool allData = sd->saveAllData;
    bool standalone = sd->standalone;

    GameSpace* space = (GameSpace*)var.GetData();

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
        
        // The archetype was not found so we are going to print out every single member
        std::string instance;
        for (unsigned int j = 0; j < ecountComponents; ++j)
        {
          if (it->HasComponent((EComponent)j))
          {
            const TypeInfo* CType = GET_STR_TYPE(GET_ENUM(Component)->m_literals[j].c_str());

            instance = CType->Name();
            instance += ":";

            for (unsigned int m = 0; m < CType->GetMembers().size(); ++m)
            {
              const Member member = CType->GetMembers()[m];
              file.Write("  %s%s ", instance.c_str(), member.Name());

              Variable var(member.Type(), (char*)it->GetComponent(j) + member.Offset());

              var.Serialize(file);
            }

            // Move to the next component
          }

          // The object didn't have the component
        }

        // Since we just serialized all the data from the component, we can move on
        // to the next object that needs to be serialized
        continue;
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
          if (!member)
            assert(false);

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
        obj = FACTORY->LoadObjectFromArchetype(space, line.c_str());
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


}