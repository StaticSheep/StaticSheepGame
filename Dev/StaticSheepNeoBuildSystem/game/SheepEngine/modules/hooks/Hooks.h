#include <unordered_map>
/*****************************************************************
Filename: Hooks.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include <boost/unordered_map.hpp>

namespace Framework
{

  struct Hook
  {
    Hook(Handle owner, const Function& fn);

    bool Hook::operator==(const Hook& rhs) const;

    Function func;
    Handle owner;
  };

  class GameSpace;

  class HookCollection
  {
  public:

    HookCollection() {};
    HookCollection(GameSpace* space);
    ~HookCollection();

    void Add(Handle owner, const Function& fn);

    void Remove(Handle owner);

    void Trigger(void);
    template <typename Arg1>
    void Trigger(Arg1 arg1);
    template <typename Arg1, typename Arg2>
    void Trigger(Arg1 arg1, Arg2 arg2);

    GameSpace* space;

    std::unordered_multimap<unsigned int, Hook* > m_hooks;
  };

  template <typename Arg1>
  void HookCollection::Trigger(Arg1 arg1)
  {
    // Goes through every hook in the list and pulls them
    for (auto it = m_hooks.begin(); it != m_hooks.end(); ++it)
    {
      it->second->func.Bind(space->GetHandles().Get(it->second->owner));
      it->second->func(arg1);
    }
  }

  template <typename Arg1, typename Arg2>
  void HookCollection::Trigger(Arg1 arg1, Arg2 arg2)
  {
    // Goes through every hook in the list and pulls them
    for (auto it = m_hooks.begin(); it != m_hooks.end(); ++it)
    {
      it->second->func.Bind(space->GetHandles().Get(it->second->owner));
      it->second->func(arg1, arg2);
    }
  }


  class HookManager
  {
  public:
    HookManager() : HookCollections(sizeof(HookCollection), 10) {};
    ~HookManager();

    void Add(std::string hook, Handle owner, const Function& func);
    void Remove(std::string hook, Handle owner);

    void Call(std::string hook);

    template <typename Arg1>
    void Call(std::string hook, Arg1 arg1);

    template <typename Arg1, typename Arg2>
    void Call(std::string hook, Arg1 arg1, Arg2 arg2);

    template <typename Arg1, typename Arg2, typename Arg3>
    void Call(std::string hook, Arg1 arg1, Arg2 arg2, Arg3 arg3);

    void Clear(std::string hook);
    void ClearAll();

    GameSpace* space;

  private:
    void Verify(std::string hook);

    ObjectAllocator HookCollections;
    boost::unordered::unordered_map<std::string, HookCollection*> HookMap;
  };

  template <typename Arg1>
  void HookManager::Call(std::string hook, Arg1 arg1)
  {
    if (HookMap.find(eventName) != HookMap.end())
    {
      HookMap.at(eventName)->Trigger(arg1);
    }
  }

  template <typename Arg1, typename Arg2>
  void HookManager::Call(std::string hook, Arg1 arg1, Arg2 arg2)
  {
    if (HookMap.find(eventName) != HookMap.end())
    {
      HookMap.at(eventName)->Trigger(arg1, arg2);
    }
  }

  template <typename Arg1, typename Arg2, typename Arg3>
  void HookManager::Call(std::string hook, Arg1 arg1, Arg2 arg2, Arg3 arg3)
  {
    if (HookMap.find(eventName) != HookMap.end())
    {
      HookMap.at(eventName)->Trigger(arg1, arg2, arg3);
    }
  }
}