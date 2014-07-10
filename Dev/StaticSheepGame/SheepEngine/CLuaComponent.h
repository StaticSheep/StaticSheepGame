/*****************************************************************
Filename: CLuaComponent.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  class LuaComponent : public GameComponent
  {
  public:
    LuaComponent();

    virtual void Initialize();
    virtual void Remove();

    virtual void QueryLoadCommand();
    void ReceiveLoadCommend(const char* command);

    std::string name;
    std::string loadCommand;

    LuaComponent& operator=(const LuaComponent& rhs);
  };
}