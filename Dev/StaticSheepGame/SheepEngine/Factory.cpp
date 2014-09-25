/*****************************************************************
Filename: Factory.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include <time.h>
#include <fstream>

namespace Framework
{
  Factory *FACTORY = NULL;

  const std::string Factory::SpaceFileExtension = ".space";
  const std::string Factory::LevelFileExtension = ".level";
  const std::string Factory::ArchetypeFileExtension = ".arche";
  const std::string Factory::ArchetypePrefix = "ach_";

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
    FACTORY = nullptr;
  }

  /// <summary>
  /// Registers a component creator.
  /// </summary>
  /// <param name="creator">The creator.</param>
  /// <param name="type">The type.</param>
  void Factory::RegisterComponentCreator(ComponentCreator& creator, const TypeInfo* type)
  {
    m_componentCreators[creator.typeID] = &creator;
    m_componentTypes[creator.typeID] = type;
  }

  /// <summary>
  /// Gets the type of a component.
  /// </summary>
  /// <param name="type">The type.</param>
  /// <returns></returns>
  const TypeInfo* Factory::GetComponentType(EComponent type)
  {
    return m_componentTypes[type];
  }

  DISABLE_WARNING(4996)
    /// <summary>
    /// Gets the component member.
    /// </summary>
    /// <param name="instring">The instring.</param>
    /// <param name="out">The out.</param>
    /// <returns></returns>
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

  /// <summary>
  /// Gets the component member. recursive.
  /// </summary>
  /// <param name="member">The member.</param>
  /// <param name="val">The value.</param>
  /// <returns></returns>
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

    /// <summary>
    /// Deserializes a component.
    /// </summary>
    /// <param name="file">The file.</param>
    /// <param name="space">The space.</param>
    /// <returns></returns>
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

  /// <summary>
  /// Saves the object to archetype.
  /// </summary>
  /// <param name="obj">The object.</param>
  /// <param name="name">The name.</param>
  void Factory::SaveObjectToArchetype(GameObject* obj, const char* name)
  {
    File file; // File to save to
    std::string filepath = name;

    if (filepath.substr(0, ArchetypePrefix.length()) != ArchetypePrefix)
    {
      filepath = ArchetypePrefix + name;
    }

    filepath += ArchetypeFileExtension;

    file.Open(filepath.c_str(), FileAccess::Write); // Open the file

    // Serialize the object
    GET_TYPE(GameObject)->Serialize(file, *obj);

    file.Close(); // Force close, will save now

    if (ArchetypeMap.find(obj->archetype) == ArchetypeMap.end())
      ArchetypeMap[obj->archetype].CopyObject(obj);
    
  }

  /// <summary>
  /// load object from archetype.
  /// </summary>
  /// <param name="space">The space.</param>
  /// <param name="name">The name.</param>
  /// <returns></returns>
  GameObject* Factory::LuaLoadObjectFromArchetype(GameSpace* space, const char* name)
  {
    return FACTORY->LoadObjectFromArchetype(space, name);
  }

  /// <summary>
  /// Loads an object from a archetype.
  /// </summary>
  /// <param name="space">The space.</param>
  /// <param name="name">The name.</param>
  /// <returns>The object</returns>
  GameObject* Factory::LoadObjectFromArchetype(GameSpace* space, const char* filepath)
  {
    File file; // File to load from
    std::string archetype = filepath;

    archetype = archetype.substr(archetype.find_last_of('\\') + 1, archetype.length() - archetype.find_last_of('\\') - 1);
    archetype = archetype.substr(0, archetype.find_first_of('.'));

    // Quickly grab the archetype from our map if it exists
    const Archetype& aType = GetArchetype(archetype);

    // Check to see if the archetype is valid
    if (&aType != &Archetype::null)
    {
      // Make the object!
      GameObject* obj = aType.CreateObject(space);
      
      return obj;
    }


    if (!File::FileExists(filepath))
    {
      return nullptr;
    }

    file.Open(filepath, FileAccess::Read);

    ErrorIf(!file.Validate(), "Factory", "Invalid file!");

    // Create an empty object
    GameObject* obj = space->CreateEmptyObject();
    Variable var = *obj; // Set the object as a variable

    // Deserialize the file into the object
    GET_TYPE(GameObject)->Deserialize(file, var);

    obj->archetype = archetype;

    // The archetype was not found, so we will save the object into our map
    if (ArchetypeMap.find(archetype) == ArchetypeMap.end())
      ArchetypeMap[archetype].CopyObject(obj);

    file.Close();

    return obj;
  }

  GameObject* Factory::LoadObjectFromArchetype(GameSpace* space, const Archetype& archetype)
  {
    return archetype.CreateObject(space);
  }

  /// <summary>
  /// Gets an archetype.
  /// </summary>
  /// <param name="name">The name.</param>
  /// <returns>Const Archetype reference</returns>
  const Archetype& Factory::GetArchetype(std::string name)
  {
    if (ArchetypeMap.find(name) != ArchetypeMap.end())
      return ArchetypeMap[name];

    return Archetype::null;
  }

  /// <summary>
  /// Saves the archetype to file.
  /// </summary>
  /// <param name="archetype">The archetype.</param>
  void Factory::SaveArchetypeToFile(const Archetype& archetype)
  {
    File file; // File to load from
    std::string filepath = archetype.archetype;

    //Check if we need to do any trimming
    if (filepath.substr(0, ArchetypePrefix.length()) != ArchetypePrefix)
    {
      // Add the prefix on if it's not there
      filepath = ArchetypePrefix + archetype.archetype;
    }

    filepath += ArchetypeFileExtension;

    file.Open(filepath.c_str(), FileAccess::Write);

    ErrorIf(!file.Validate(), "Factory", "Invalid file!");

    Variable var = archetype;
    var.Serialize(file);

    file.Close();
  }

  /// <summary>
  /// Saves the archetype to file.
  /// </summary>
  /// <param name="name">The name.</param>
  void Factory::SaveArchetypeToFile(std::string name)
  {
    const Archetype& archetype = GetArchetype(name);

    if (&archetype != &Archetype::null)
      SaveArchetypeToFile(archetype);
  }

  /// <summary>
  /// Loads the archetype from a file.
  /// </summary>
  /// <param name="name">The name.</param>
  /// <returns>Archetype was loaded</returns>
  bool Factory::LoadArchetypeFromFile(const char* name)
  {
    File file; // File to load from
    std::string filepath = name;
    std::string archetype = name;

    //Check if we need to do any trimming
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

    // Quickly grab the archetype from our map if it exists
    const Archetype& aType = GetArchetype(archetype);

    // Check to see if the archetype is valid
    if (&aType != &Archetype::null)
    {
      // Make the object!
      return true;
    }

    filepath += ArchetypeFileExtension;

    if (!File::FileExists(filepath.c_str()))
    {
      return false;
    }

    file.Open(filepath.c_str(), FileAccess::Read);

    ErrorIf(!file.Validate(), "Factory", "Invalid file!");

    Archetype buffer;
    Variable var = buffer;
    Archetype::Deserialize(file, var);
    buffer.archetype = archetype;

    ArchetypeMap[buffer.archetype] = buffer;

    return true;
  }






  // Backups a file into the backup folder
  static void StoreBackup(const char* filepath)
  {
    if (File::FileExists(filepath))
    {
      time_t t = time(0);
      struct tm * now = localtime(&t);

      std::string backUpFile = "backup\\";
      backUpFile += filepath;
      backUpFile += "." + std::to_string(now->tm_mon) + "_" + std::to_string(now->tm_mday) + "_" +
        std::to_string(now->tm_hour) + "_" + std::to_string(now->tm_min) + "_" + std::to_string(now->tm_sec) + ".backup";

      std::ifstream src;
      src.open(filepath);
      std::ofstream dest;
      dest.open(backUpFile.c_str());
      dest << src.rdbuf();
    }
  }

  /// <summary>
  /// Saves the space to a space file.
  /// If you wish to save as a standalone level file, the third paramater should be a boolean marked as true.
  /// </summary>
  /// <param name="space">The space to save.</param>
  /// <param name="name">The name of the level.</param>
  /// <param name="objInstanceData">Game object instance data list. This is a vector of strings which contain instance parameters defined as such:
  /// {COMPONENT}:{DATA} You can use additional :'s for structs.
  /// Example: Transform:value1</param>
  /// By default data which has been modified on an archetype is not saved. By adding instance data parameters to this you can tell the serialized
  /// to save specific parameters of the object.
  /// <param name="includeGeneric">If this is true, then objects which are not from an archetype will be serialized inside of the level file</param>
  /// <param name="allData">If this is true, then every modified value of an archetype will be saved</param>
  void Factory::SaveSpaceToFile(GameSpace* space, const char* name, std::vector<std::string>* objInstanceData, bool includeGeneric, bool allData)
  {
    File file; // File to save the space to
    std::string filepath = name + SpaceFileExtension;
    GameSpace::SerializerData extraData;

    extraData.instanceData = objInstanceData;
    extraData.includeGeneric = includeGeneric;
    extraData.saveAllData = allData;
    extraData.standalone = false;


    StoreBackup(filepath.c_str());
    file.Open(filepath.c_str(), FileAccess::Write); // Open the file

    Serializer::Get()->SetUserData(&extraData);

    Variable(*space).Serialize(file);

    Serializer::Get()->SetUserData(NULL);

    file.Close();
  }

  /// <summary>
  /// Saves the space to a space file.
  /// This version allows you to save the space as a stand alone level file.
  /// Stand alone level files do not rely upon archetypes, instead it saves every single object
  /// individually with all data intact. The actual archetype names are saved inside of the object
  /// so in case you ever want to revert back to a non-standalone version, that is still possible.
  /// WARNING: This can result in a very large file
  /// </summary>
  /// <param name="space">The space.</param>
  /// <param name="name">The name.</param>
  /// <param name="standalone">Stand alone mode?</param>
  void Factory::SaveSpaceToFile(GameSpace* space, const char* name, bool standalone)
  {
    File file; // File to save the space to
    std::string filepath = name + SpaceFileExtension;
    GameSpace::SerializerData extraData;
    
    extraData.instanceData = NULL;
    extraData.includeGeneric = false;
    extraData.saveAllData = false;
    extraData.standalone = standalone;

    StoreBackup(filepath.c_str());

    file.Open(filepath.c_str(), FileAccess::Write); // Open the file

    Serializer::Get()->SetUserData(&extraData);

    Variable(*space).Serialize(file);

    Serializer::Get()->SetUserData(NULL);

    file.Close();
  }


  /// <summary>
  /// Loads a space file.
  /// </summary>
  /// <param name="space">The space.</param>
  /// <param name="name">The name.</param>
  GameSpace* Factory::LoadSpace(const char* filepath)
  {
    File file; // File to save the space to

    file.Open(filepath, FileAccess::Read); // Open the file

    ErrorIf(!file.Validate(), "Factory - Load Level", "Invalid level: %s", filepath);

    std::string spaceName = file.GetLine('\n');

    if (ENGINE->GetSpace(spaceName.c_str()) != nullptr)
    {
      file.Close();
      return nullptr;
    }

    GameSpace* space = ENGINE->CreateSpace(spaceName.c_str());

    Variable(*space).Deserialize(file);

    file.Close();


    return space;
  }


  void Factory::SaveLevel(const char* name)
  {
    File file;

    std::string levelPath = name;
    levelPath += LevelFileExtension;

    file.Open(levelPath.c_str(), FileAccess::Write);

    ErrorIf(!file.Validate(), "Level Factory", "Failed to save a level? %s", levelPath.c_str());

    for (size_t i=0; i < ENGINE->Spaces().size(); ++i)
    {
      GameSpace* sp = ENGINE->Spaces()[i];
      file.Write("%s\n", sp->GetName().c_str());
      SaveSpaceToFile(sp, sp->GetName().c_str(), nullptr, true, true);
    }

    file.Close();
  }

  void Factory::LoadLevel(const char* filePath, void(*cb)(GameSpace* space))
  {
    File file;

    file.Open(filePath, FileAccess::Read);

    ErrorIf(!file.Validate(), "Level Factory", "Greg's a nerd");

    std::string spaceName;

    while (file.Validate())
    {
      spaceName = file.GetLine('\n');
      if (spaceName.length() > 0)
      {
        spaceName += SpaceFileExtension;
        GameSpace* sp;
        sp = LoadSpace(spaceName.c_str());
        if (cb)
          cb(sp);
      }
    }

    file.Close();

  }






  

}