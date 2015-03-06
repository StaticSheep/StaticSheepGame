/*****************************************************************
Filename: blocklights.h
Project:  Gam200
Author(s): Zachary Nawar (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "components/base/Component.h"

namespace Framework
{
  class BlockLights : public GameComponent
  {
  public:
    BlockLights();

    enum Effect
    {
      NONE,
      FLICKER,
      PULSE
    };

    struct TriggerData
    {
      int duration = 1;
      Effect fx = NONE;
      bool overrideColor = false;
      Vec4 color;
    };

    void Initialize();
    void Remove();

    void Update(float dt);
    void TriggerLight(int group, TriggerData* data);

    void Activate(TriggerData* data);
    void Deactivate();

    // This is a bit-masked integer
    int m_group = 0; 

    // Time left
    int m_timeLeft = 0;

    // Current effect
    Effect m_effect;

    

    bool m_activated = false;


  private:
    Vec4 m_prevColor;
    bool m_prevStatus;
    bool m_overrideColor;
  };
}