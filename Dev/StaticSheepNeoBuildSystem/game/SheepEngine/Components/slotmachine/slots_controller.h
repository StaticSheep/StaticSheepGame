/******************************************************************************
Filename: slots_controller.h
Project:
Author(s): Zachary Nawar (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once


#include "components\base\Component.h"

namespace Framework
{
  class Transform;

  class SlotController : public GameComponent
  {
  public:

    SlotController();
    ~SlotController();

    virtual void Initialize();
    virtual void Remove();

    void Update(float dt);
    void Stop();

    float slotOptions = 3;

    float startSpeed = 7;
    

  private:
    float m_timeLeft = 0;
    float m_curSpeed = 0;
    float m_uvSnap = 0;
    bool m_spinning = false;
  };
}