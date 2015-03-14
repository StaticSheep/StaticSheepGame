/*****************************************************************
Filename: CLevel1_Lighting.h
Project:  Gam250
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "components/lights/CPointLight.h"
#include "types/handle/Handle.h"

namespace Framework
{
  class Level1_Lighting : public GameComponent
  {
  public:

    Level1_Lighting();
    ~Level1_Lighting();

    void Initialize();
    void Remove();

    void TurnOn();
    void TurnOff();
    void Toggle();

    void LogicUpdate(float dt);

    void CreateSpawnLights();

    Handle m_lights[6];

    Vec3 m_spawnPos[6];

    float m_levelTime;

    bool m_isOn;

    float m_PulseT;

    bool m_PulseFlip;



  };
}