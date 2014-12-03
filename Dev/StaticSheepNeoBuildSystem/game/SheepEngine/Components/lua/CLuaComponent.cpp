/*****************************************************************
Filename: CLuaComponent.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"

#include "components/lua/CLuaComponent.h"

namespace Framework
{
  //LuaComponent::LuaComponent()
  //{
  //}

  void LuaComponent::Initialize()
  {
    Lua::CallFunc(ENGINE->Lua(), "AttachComponentToObject", space->GetName().c_str(),
      owner.operator unsigned(), self.operator unsigned(), name.c_str());

    if (loadCommand.length() > 0)
    {
      Lua::CallFunc(ENGINE->Lua(), "DeserializeComponent", space->GetName().c_str(),
        owner.operator unsigned(), self.operator unsigned(), loadCommand.c_str());
    }
  }

  void LuaComponent::QueryLoadCommand()
  {
    Lua::CallFunc(ENGINE->Lua(), "SerializeComponent", space->GetName().c_str(),
      owner.operator unsigned(), self.operator unsigned(), this);
  }

  void LuaComponent::ReceiveLoadCommend(const char* command)
  {
    loadCommand = command;
  }

  void LuaComponent::Remove()
  {
    // The object will do this for us so yeah who cares
    if (space != nullptr)
      Lua::CallFunc(ENGINE->Lua(), "RemoveComponentFromGameObject",
      space->GetName().c_str(), owner.operator size_t(), self.operator size_t());

    name.~basic_string();
    loadCommand.~basic_string();
  }

  LuaComponent& LuaComponent::operator=(const LuaComponent& rhs)
  {
    name = rhs.name;
    loadCommand = rhs.loadCommand;

    return *this;
  }

}