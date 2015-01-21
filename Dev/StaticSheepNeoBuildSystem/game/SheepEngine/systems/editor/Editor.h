/*****************************************************************
Filename: Editor.h
Project:
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "systems/System.h"

namespace Framework
{
  class Editor : public ISystem
  {

    Editor();
    ~Editor();

    // Initialization call for the system
    virtual void Initialize();

    // All system can be updated every frame through this call
    virtual void Update(float dt);

    // Systems receive and handle messages in different ways
    virtual void ReceiveMessage(Message& msg);

    // Systems can have names too!
    virtual std::string GetName() { return std::string("Editor"); };
  };
}