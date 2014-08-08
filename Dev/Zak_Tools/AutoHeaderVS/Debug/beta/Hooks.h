#include <unordered_map>
/*****************************************************************
Filename: Hooks.h
Project: 

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




    template <typename Arg1>
    template <typename Arg1, typename Arg2>

    GameSpace* space;

    std::unordered_multimap<unsigned int, Hook* > m_hooks;
  };

  template <typename Arg1>
  {
    // Goes through every hook in the list and pulls them
    {
    }
  }

  template <typename Arg1, typename Arg2>
  {
    // Goes through every hook in the list and pulls them
    {
    }
  }


  class HookManager
  {
  public:



    template <typename Arg1>


    GameSpace* space;

  private:

    ObjectAllocator HookCollections;
    std::unordered_map<std::string, HookCollection*> HookMap;
  };

  template <typename Arg1>
  {
    {
    }
  }

}
