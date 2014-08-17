/*****************************************************************
Filename: Archetype.h
*****************************************************************/

#pragma once

namespace Framework
{
  class Archetype
  {
  public:

    std::string name;
    std::string archetype;


    




    static const Archetype null;


  private:


    GameComponent* m_components[ecountComponents];

    std::vector<LuaComponent*> m_luaComponents;
    

    friend class Factory;
    friend class GameSpace;
  };

}
