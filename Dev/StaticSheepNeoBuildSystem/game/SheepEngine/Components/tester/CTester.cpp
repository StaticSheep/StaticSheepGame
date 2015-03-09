/*****************************************************************
Filename: CTester.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"

#include "CTester.h"
#include "components/gameplay_scripts/arena/CBlockLights.h"

namespace Framework
{
  void Tester::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Tester::Update));
  }

  void Tester::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }

  void Tester::Update(float dt)
  {

    /*timer -= dt;

    if (timer < 0)
    {
    ++phase;
    timer = 1.0f;
    }

    if (phase == 1)
    {
    BlockLights::EventData ed;

    ed.duration = 10;
    ed.settings.color = Vec4(0.2f, 1.0f, 0.2f, 1);
    ed.settings.useColor = true;

    space->hooks.Call("LightingEvent", 0xFFFFFFFF, &ed);
    ++phase;
    }

    if (phase == 3)
    {
    BlockLights::EventData ed;

    ed.duration = 4;
    ed.settings.color = Vec4(0.9f, 0.0f, 0.2f, 0.2f);
    ed.settings.useColor = true;

    space->hooks.Call("LightingEvent", 0xFFFFFFFF, &ed);
    ++phase;
    }

    if (phase == 5)
    {
    BlockLights::EventData ed;

    ed.duration = 5;
    ed.settings.color = Vec4(0.2f, 0.2f, 1.0f, 1);
    ed.settings.useColor = true;

    space->hooks.Call("LightingEvent", 0xFFFFFFFF, &ed);
    ++phase;
    }*/

    if (timer == 0)
    {
      {
        BlockLights::EventData ed;

        ed.duration = 10;
        ed.settings.color = Vec4(0.2f, 1.0f, 0.2f, 1);
        ed.settings.useColor = true;

        space->hooks.Call("LightingEvent", 0xFFFFFFFF, &ed);
        ++phase;
      }

    {
      BlockLights::EventData ed;

      ed.duration = 5;
      ed.settings.color = Vec4(0.9f, 0.0f, 0.2f, 0.2f);
      ed.settings.useColor = true;

      space->hooks.Call("LightingEvent", 0xFFFFFFFF, &ed);
      ++phase;
    }

    {
      BlockLights::EventData ed;

      ed.duration = 3;
      ed.settings.color = Vec4(0.2f, 0.2f, 1.0f, 1);
      ed.settings.useColor = true;

      space->hooks.Call("LightingEvent", 0xFFFFFFFF, &ed);
      ++phase;
    }

    {
      BlockLights::EventData ed;

      ed.duration = 1;
      ed.settings.color = Vec4(0.2f, 0.2f, 1.0f, 1);
      ed.settings.useColor = true;

      space->hooks.Call("LightingEvent", 0xFFFFFFFF, &ed);
      ++phase;
    }

    timer = 1;
  }

    
    
  }
}