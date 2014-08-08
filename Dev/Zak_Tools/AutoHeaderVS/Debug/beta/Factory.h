/*****************************************************************
Filename: Factory.h
Project: 

*****************************************************************/

#pragma once

namespace Framework
{
  class Factory;
  extern Factory* FACTORY;


  class Factory
  {
  public:





    static const std::string ArchetypeFileExtension;
    static const std::string ArchetypePrefix;
    static const std::string LevelFileExtension;



  protected:
    ComponentCreator* m_componentCreators[ecountComponents];
    const TypeInfo* m_componentTypes[ecountComponents];

    std::hash_map<std::string, Archetype> ArchetypeMap;

  private:
    // Deserializes a component from a file



    friend class GameObject;
    friend class GameSpace;
    friend class Archetype;
  };
}
