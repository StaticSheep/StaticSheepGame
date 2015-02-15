/*****************************************************************
Filename: LEBase.h
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

namespace Framework
{
  struct LevelEvent
  {
    LevelEvent() {};
    virtual ~LevelEvent() {};
    virtual void Update(float dt) = 0;
    virtual void FireEvent(GameObject *LogicController) = 0;
  };
}