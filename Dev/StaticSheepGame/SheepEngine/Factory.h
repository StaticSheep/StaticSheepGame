/*****************************************************************
Filename: Factory.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

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
    GameObject* LoadObjectFromArchetype(GameSpace* space, const char* name);

    void SaveSpaceToLevel(GameSpace* space, const char* name, std::vector<std::string>* objInstanceData = NULL, bool includeGeneric = false, bool allData = false);
    void SaveSpaceToLevel(GameSpace* space, const char* name, bool standalone);
    void LoadLevelToSpace(GameSpace* space, const char* name);

    static const std::string FileExtension;
    static const std::string ArchetypePrefix;
    static const std::string LevelPrefix;

    const Archetype& GetArchetype(std::string name);

  protected:
    ComponentCreator* m_componentCreators[ecountComponents];
    const TypeInfo* m_componentTypes[ecountComponents];

    std::hash_map<std::string, Archetype> ArchetypeMap;

  private:
    // Deserializes a component from a file
    GameComponent* DeserializeComponent(File& file, GameSpace* space);

    const Member *GetComponentMember(const char* instring, Variable& out);
    const Member *GetComponentMemberRecursive(const Member *member, Variable &val);

    //void LoadGenericObject(GameSpace* space, File& file);

    friend class GameObject;
    friend class GameSpace;
    friend class Archetype;
  };
}