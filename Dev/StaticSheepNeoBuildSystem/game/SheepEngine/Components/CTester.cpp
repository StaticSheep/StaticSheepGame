/*****************************************************************
Filename: CTester.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "precompiled.h"

namespace Framework
{
  void Tester::Initialize()
  {
    //space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Tester::Update));
  }

  void Tester::Update(float dt)
  {
    testvalue1 = 10;
  }
}