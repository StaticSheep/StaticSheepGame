#include <unordered_map>
/*****************************************************************
Filename: Hooks.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  class Handle;

  template <typename HF>
  struct Hook
  {
    Hook(EHooks type, const HF& fn);

    bool Hook::operator==(const Hook& rhs) const;

    const EHooks type;
    HF function;
    Handle owner;
  };

  template <typename HF>
  Hook<HF>::Hook(EHooks type, const HF& fn) :type(type), function(fn) {}

  template <typename HF>
  bool Hook<HF>::operator==(const Hook& rhs) const { return type == rhs.type; }
  
  class Collection
  {

  };

  template <typename HF>
  class HookCollection : public Collection
  {
  public:

    HookCollection<HF>(EHooks type) :type(type) {};

    void Subscribe(Handle owner, const HF& func);

    void Pull(void);
    template <typename Arg1>
    void Pull(Arg1 arg1);
    template <typename Arg1, typename Arg2>
    void Pull(Arg1 arg1, Arg2 arg2);

    std::unordered_multimap<unsigned int, Hook<HF>* > m_hooks;

    const EHooks type;
  };

  template <typename HF>
  void HookCollection<HF>::Subscribe(Handle owner, const HF& func)
  {
    // Creates a new hook
    Hook<HF>* newHook = new Hook<HF>(type, func);
    newHook->owner = owner;

    // Inserts the hook into the map
    m_hooks.insert( std::pair<Handle, Hook<HF>* >(owner, newHook));
  }

  template <typename HF>
  void HookCollection<HF>::Pull()
  {
    // Goes through every hook in the list and pulls them
    for (auto i = m_hooks.first; i != m_hooks.second; ++i)
      i->second->function();
  }

  template <typename HF>
  template <typename Arg1>
  void HookCollection<HF>::Pull(Arg1 arg1)
  {
    // Goes through every hook in the list and pulls them
    for (auto it = m_hooks.begin(); it != m_hooks.end(); ++it)
      it->second->function(arg1);
  }

  template <typename HF>
  template <typename Arg1, typename Arg2>
  void HookCollection<HF>::Pull(Arg1 arg1, Arg2 arg2)
  {
    // Goes through every hook in the list and pulls them
    for (auto it = m_hooks.begin(); it != m_hooks.end(); ++it)
      it->second->function(arg1, arg2);
  }
}