/*****************************************************************
Filename: Hooks.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

namespace Framework
{
  Hook::Hook(Handle owner, const Function& fn) : func(fn), owner(owner)
  {
  }

  bool Hook::operator==(const Hook& rhs) const
  {
    return owner == rhs.owner;
  }


  void HookCollection::Add(Handle owner, const Function& fn)
  {
    // Creates a new hook
    Hook* newHook = new Hook(owner, fn);
    newHook->owner = owner;

    // Inserts the hook into the map
    m_hooks.insert( std::pair<Handle, Hook* >(owner, newHook));
  }

  void HookCollection::Trigger()
  {
    // Goes through every hook in the list and pulls them
    for (auto i = m_hooks.begin(); i != m_hooks.end(); ++i)
      i->second->func();
  }

  void HookManager::Verify(std::string eventName)
  {
    if (HookMap.find(eventName) == HookMap.end())
      HookMap[eventName] = 
  }


  void HookManager::Add(std::string eventName, Handle owner, const Function& func)
  {
    Verify(eventName);

    HookMap[eventName].Add(owner, func);
  }

}