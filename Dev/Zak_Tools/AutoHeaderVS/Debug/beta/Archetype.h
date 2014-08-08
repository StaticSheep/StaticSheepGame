/*****************************************************************
Filename: Archetype.h
Project: 

*****************************************************************/

#pragma once

namespace Framework
{
  class Archetype
  {
  public:

    std::string name;
    std::string archetype;

    template<typename T>

    


    // Serialization routine


    static const Archetype null;


  private:


    GameComponent* m_components[ecountComponents];

    std::vector<LuaComponent*> m_luaComponents;
    

    friend class Factory;
    friend class GameSpace;
  };

  template<typename T>
  {
  }
}
