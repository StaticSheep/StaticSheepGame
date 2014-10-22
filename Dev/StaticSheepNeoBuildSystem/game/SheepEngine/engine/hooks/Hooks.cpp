/*****************************************************************
Filename: Hooks.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include <utility>

namespace Framework
{
  Hook::Hook(Handle owner, const Function& fn) : func(fn), owner(owner)
  {
  }

  bool Hook::operator==(const Hook& rhs) const
  {
    return owner == rhs.owner;
  }

  HookCollection::HookCollection(GameSpace* space)
    :space(space)
  {
  }
  

  HookCollection::~HookCollection()
  {
    if (m_hooks.size() == 0)
    {
      m_hooks.clear();
    }

    for (auto i = m_hooks.begin(); i != m_hooks.end(); ++i)
      delete i->second;

    m_hooks.clear();
  }

  void HookCollection::Add(Handle owner, const Function& fn)
  {
    // Creates a new hook
    Hook* newHook = new Hook(owner, fn);
    newHook->owner = owner;

    // Inserts the hook into the map
    m_hooks.insert( std::pair<Handle, Hook* >(owner, newHook));
  }

  void HookCollection::Remove(Handle owner)
  {
    delete m_hooks.find(owner)->second;
    m_hooks.erase(m_hooks.find(owner));
  }

  void HookCollection::Trigger()
  {
    if (m_hooks.size() == 0)
      return;
    // Goes through every hook in the list and pulls them
    for (auto i = m_hooks.begin(); i != m_hooks.end(); ++i)
    {
      i->second->func.Bind(space->GetHandles().Get(i->second->owner));
      i->second->func();
    }
  }

  HookManager::~HookManager()
  {
    ClearAll();
  }

  void HookManager::Verify(std::string& eventName)
  {
    if (HookMap.find(eventName) == HookMap.end())
    {
      HookCollection* hc = (HookCollection*)HookCollections.Allocate();
      new (hc) HookCollection(space);
      
      HookMap[eventName] = hc;
    }
  }


  void HookManager::Add(std::string eventName, Handle owner, const Function& func)
  {
    Verify(eventName);

    HookMap.at(eventName)->Add(owner, func);
  }

  void HookManager::Call(std::string eventName)
  {
    if (HookMap.find(eventName) != HookMap.end())
    {
      HookMap.at(eventName)->Trigger();
    }
  }

  void HookManager::Remove(std::string eventName, Handle owner)
  {
    if (HookMap.find(eventName) != HookMap.end())
    {
      HookMap.at(eventName)->Remove(owner);
    }
  }

  void HookManager::Clear(std::string eventName)
  {
    HookMap.at(eventName)->~HookCollection();
    HookMap.erase(HookMap.find(eventName));
  }

  void HookManager::ClearAll()
  {
    for(auto it = HookMap.begin(); it != HookMap.end(); ++it)
    {
      it->second->~HookCollection();
    }
    if (HookMap.size() > 0)
      HookMap.clear();
  }

  

}