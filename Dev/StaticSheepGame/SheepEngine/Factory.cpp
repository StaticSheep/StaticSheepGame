/*****************************************************************
Filename: Factory.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

namespace Framework
{
  Factory *FACTORY = NULL;

  const std::string FileExtension(".sheep");

  const std::string ArchetypePrefix = "ach_";
  const std::string LevelPrefix = "lvl_";

  Factory::Factory()
  {
    for (unsigned int i = 0; i < ecountComponents; ++i)
    {
      m_componentCreators[i] = nullptr;
      m_componentTypes[i] = nullptr;
    }
    FACTORY = this;
  }

  Factory::~Factory()
  {

  }

  void Factory::RegisterComponentCreator(ComponentCreator& creator, const TypeInfo* type)
  {
    m_componentCreators[creator.typeID] = &creator;
    m_componentTypes[creator.typeID] = type;
  }

  const TypeInfo* Factory::GetComponentType(EComponent type)
  {
    return m_componentTypes[type];
  }

  DISABLE_WARNING(4996)
  const Member* Factory::GetComponentMember(const char *instring, Variable& out)
  {
    char data[256];

    // Create a copy of the component name
    strcpy(data, instring);
    std::string componentName = strtok(data, ":");

    // Get a pointer to the typeinfo for the component
    const TypeInfo* componentType = GET_STR_TYPE(componentName.c_str());

    // Turn out into the game object
    GameObject* o = out.GetValue<GameObject*>(); // assume out holds game object ptr

    // Turn out into a variable for the component
    out = Variable(componentType, o->GetComponent(componentName.c_str()));

    // Get the member of the component we are looking for
    const Member* member = out.GetTypeInfo()->GetMember(strtok(NULL, ":"));

    // Finally get the actual variable which is the member we are looking for
    out = Variable(member->Type(), PTR_ADD(out.GetData(), member->Offset()));

   

    // Now that we have done that, we enter our recursive loop to strip off
    // any remaining :'s from the instring
    return GetComponentMemberRecursive(member, out);
  }

  const Member* Factory::GetComponentMemberRecursive(const Member *member, Variable &val)
  {
    
    // Check to see if there is anything left on the token list
    const char *memberNamePtr = strtok( NULL, ":" );

    // Nothing left, we found the member we want
    if(!memberNamePtr)
      return member;

    // Set the member name we are looking for
    std::string memberName = memberNamePtr;

    // Find the member of the the member
    const Member *deepMember = member->Type()->GetMember(memberName.c_str());
    // Get the type of the member
    const TypeInfo *deepMemberType = deepMember->Type();
    // memberName should == deepMemberType->Name()

    // Get a variable of the member we just got
    val = Variable(deepMemberType, PTR_ADD(val.GetData(), deepMember->Offset()));

    // And recursion
    return GetComponentMemberRecursive( deepMember, val );
  }

  END_DISABLE()

  GameComponent* Factory::DeserializeComponent(File& file, GameSpace* space)
  {
    // Get the generic serializer
    Serializer* s = Serializer::Get( );

    // Peek into the file to see if we can see a type
    const TypeInfo* info = s->PeekType(file);

    // No type, exit
    if(!info)
      return nullptr;

    // Variables we are going to use
    GameComponent* c;
    Variable var;

    // Get the component enum to check
    Enum* e = GET_ENUM(Component);

    // Check to see if the type we read is a component
    if (e->IsAnEntry(info->Name()))
    {
      // Get the type of component we need to make
      EComponent type = (EComponent)e->GetIndexFromString(info->Name());
      // Create a blank component in the space
      c = space->CreateComponent(type);
      // Make a variable from the component
      var = Variable(info, c);
    }
    else
      return NULL;

    // Set our start level
    int startLevel = s->m_paddingLevel;

    // Iterate through all member variables until our padding is back to the start
    do
    {
      // Peek to see the next possible member
      const Member* mem = s->PeekMember(file, info->m_members, startLevel);
      if (mem) // We have a member, great
      {
        // Create a variable from the member
        Variable member(mem->Type(), PTR_ADD(var.GetData(), mem->Offset()));
        // Deserialize the variable
        member.Deserialize(file);
      }
    } while(s->m_paddingLevel > startLevel);

    return c;
  }

  void Factory::SaveObjectToArchetype(GameObject* obj, const char* name)
  {
    File file; // File to save to
    std::string filepath = name;

    if (filepath.substr(0, ArchetypePrefix.length()) != ArchetypePrefix)
    {
      filepath = ArchetypePrefix + name;
    }

    filepath += FileExtension;

    file.Open(filepath.c_str(), FileAccess::Write); // Open the file

    // Serialize the object
    GET_TYPE(GameObject)->Serialize(file, *obj);

    file.Close(); // Force close, will save now
  }

  GameObject* Factory::LoadObjectFromArchetype(GameSpace* space, const char* name)
  {
    File file; // File to load from
    std::string filepath = name;
    std::string archetype = name;

    if (filepath.substr(0, ArchetypePrefix.length()) != ArchetypePrefix)
    {
      // Add the prefix on if it's not there
      filepath = ArchetypePrefix + name;
    }
    else
    {
      // Remove the prefix if it is there from the actual archetype name
      archetype = archetype.substr(ArchetypePrefix.length(), archetype.length() - ArchetypePrefix.length());
    }

    filepath += FileExtension;

    file.Open(filepath.c_str(), FileAccess::Read);

    if (!file.Validate())
      assert(false);

    // Create an empty object
    GameObject* obj = space->CreateEmptyObject();
    Variable var = *obj; // Set the object as a variable

    // Deserialize the file into the object
    GET_TYPE(GameObject)->Deserialize(file, var);

    obj->m_archetype = archetype;

    return obj;
  }

  // Transform:testvalue
  void Factory::SaveSpaceToLevel(GameSpace* space, const char* name, std::vector<std::string>* objInstanceData, bool includeGeneric, bool allData)
  {
    File file; // File to save the space to
    std::string filepath = LevelPrefix + name + FileExtension;

    file.Open(filepath.c_str(), FileAccess::Write); // Open the file

    for (auto it = space->m_objects.begin<GameObject>(); it != space->m_objects.end<GameObject>(); ++it)
    {

      if (it->m_archetype.length() == 0 && includeGeneric)
      {
        // We couldn't find an archetype, so we are going to
        // serialize the object into the level

        // Set the variable var to the game object pointer
        Variable var = *it;

        it->Serialize(file, var);
        continue;
      }

      // Write the name of the archetype
      file.Write("ach_%s\n", it->m_archetype.c_str());

      // Get the name member of the object
      const Member* nameMember = GET_TYPE(GameObject)->GetMember("name");

      // Pad and write the name of the member
      file.Write("  %s ", nameMember->Name());

      // Create a variable and write to the file
      Variable name(nameMember->Type(), (char*)it + nameMember->Offset());
      name.GetTypeInfo()->Serialize(file, name);

      if (allData)
      {
        std::string instance;

        for (unsigned int j = 0; j < ecountComponents; ++j)
        {
          if (it->HasComponent((EComponent)j))
          {
            const TypeInfo* CType = GET_STR_TYPE(GET_ENUM(Component)->m_literals[0].c_str());

            instance = CType->Name();
            instance += ":";

            for (unsigned int m = 0; m < CType->m_members.size(); ++m)
            {
              const Member member = CType->m_members[m];
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
          const Member *member = GetComponentMember(instance, var);
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

    file.Close();
  }


  void Factory::LoadLevelToSpace(GameSpace* space, const char* name)
  {
    File file; // File to save the space to
    std::string filepath = LevelPrefix + name + FileExtension;

    file.Open(filepath.c_str(), FileAccess::Read); // Open the file

    assert(file.Validate());

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
      if (typeinfo && typeinfo->m_name == "GameObject")
      {
        file.SeekByOffset(-int(line.length()) - 2);
        // Create an empty object
        obj = space->CreateEmptyObject();
        Variable var = *obj; // Set the object as a variable

        // Deserialize the file into the object
        GET_TYPE(GameObject)->Deserialize(file, var);
        continue;
      }

      // If we didn't hit a component, check to see if by chance we hit an archetype
      if (!GET_ENUM(Component)->IsAnEntry(line.c_str()) && line.substr(0, ArchetypePrefix.length()) == ArchetypePrefix)
      {
        // Oh we did, goodie
        obj = LoadObjectFromArchetype(space, line.c_str());
        continue;
      }

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
      const Member* member = GetComponentMember(line.c_str(), var);

      // Deserialize the member
      member->Type()->Deserialize(file, var);
    }
  }

}