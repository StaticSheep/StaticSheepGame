/*****************************************************************
Filename: Factory.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "engine/introspection/enum/Enum.h"

namespace Framework
{
  class Factory;
  extern Factory* FACTORY;

#define REGISTER_COMPONENT(C) \
  static ComponentCreatorType<C> C##Creator( e##C ); \
  FACTORY->RegisterComponentCreator( C##Creator, GET_TYPE(C) )

  class Factory
  {
  public:
    Factory();
    ~Factory();

    const TypeInfo* GetComponentType(EComponent type);
    void RegisterComponentCreator(ComponentCreator& creator, const TypeInfo* type);

    void SaveObjectToArchetype(GameObject* obj, const char* name);


    // Creates an object of from an archetype. [Loads the archetype if required]
    GameObject* LoadObjectFromArchetype(GameSpace* space, const char* name);
	  GameObject* LoadObjectFromArchetypeFP(GameSpace* space, const char* name);
    // Creates an object from a virtual archetype.
    GameObject* LoadObjectFromArchetype(GameSpace* space, const Archetype& archetype);
    // Preloads an archetype of a specified name
    bool LoadArchetypeFromFile(const char* name);

    void SaveArchetypeToFile(std::string);
    void SaveArchetypeToFile(const Archetype& archetype);

    void SaveSpaceToFile(GameSpace* space, const char* name, std::vector<std::string>* objInstanceData = NULL, bool includeGeneric = false, bool allData = false);
    void SaveSpaceToFile(GameSpace* space, const char* name, bool standalone);
    // Saves a space to an explicit filepath. This should be used with caution.
    void SaveSpaceToFilePath(GameSpace* space, const char* filepath);

    GameSpace* LoadSpaceFilePath(const char* filepath);
    GameSpace* LoadSpace(const char* name);

    // Saves all of the spaces into a level file
    void SaveLevel(const char* filePath);
    // Loads a level file, which loads multiple spaces
    void LoadLevel(const char* filePath, void(*cb)(GameSpace* space));

    static const std::string ArchetypeFileExtension;
    static const std::string ArchetypePrefix;
    static const std::string SpaceFileExtension;
    static const std::string LevelFileExtension;

    static const std::string SpaceFilePath;
    static const std::string LevelFilePath;
    static const std::string ArchetypeFilePath;

    static GameObject* LuaLoadObjectFromArchetype(GameSpace* space, const char* name);

    const Archetype& GetArchetype(std::string name);

  protected:
    ComponentCreator* m_componentCreators[ecountComponents];
    const TypeInfo* m_componentTypes[ecountComponents];

    std::hash_map<std::string, Archetype> ArchetypeMap;

  private:
    // Deserializes a component from a file
    GameComponent* DeserializeComponent(File& file, GameSpace* space);

    const Member* GetComponentMember(const char* instring, Variable& out);
    const Member* GetComponentMemberRecursive(const Member *member, Variable &val);

    //void LoadGenericObject(GameSpace* space, File& file);

    friend class GameObject;
    friend class GameSpace;
    friend class Archetype;
  };
}