#include <unordered_map>
/*****************************************************************
Filename: Hooks.h
*****************************************************************/

#pragma once

namespace Framework
{

  struct Hook
  {


    Function func;
    Handle owner;
  };

  class GameSpace;

  class HookCollection
  {
  public:





    GameSpace* space;

    std::unordered_multimap<unsigned int, Hook* > m_hooks;
  };




  class HookManager
  {
  public:





    GameSpace* space;

  private:

    ObjectAllocator HookCollections;
    std::unordered_map<std::string, HookCollection*> HookMap;
  };


}
