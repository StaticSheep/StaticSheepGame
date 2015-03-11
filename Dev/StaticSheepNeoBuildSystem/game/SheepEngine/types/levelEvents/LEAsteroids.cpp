/*****************************************************************
Filename: LEAsteroids.cpp
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "LEAsteroids.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/Weapon_Scripts/CBullet_default.h"
#include "components/particles/CParticleCircleEmitter.h"
#include "components/particles/CParticleSystem.h"
#include "../SheepUtil/include/Matrix3D.h"

namespace Framework
{
  LEAsteroids::LEAsteroids()
  {
    timer = 0.0f;
    numOfSpawns = 3;
  }

  LEAsteroids::~LEAsteroids()
  {

  }

  void LEAsteroids::Update(float dt)
  {
    timer -= dt;
    if (timer <= 0.0f && numOfSpawns > 0)
    {
      FireEvent(LC);
      numOfSpawns -= 1;
      timer = 1.0f;
    }
    else if (timer <= 0.0f)
    {
      timer = 5.0f;
      FireEvent(LC);
    }
  }

  void LEAsteroids::FireEvent(GameObject *LogicController)
  {
    LC = LogicController;
    for (int i = 0; i < 5; ++i)
    {
      Handle asteroid = (FACTORY->LoadObjectFromArchetype(LogicController->space, "Asteroid"))->self;
      Transform *aT = (LogicController->space->GetGameObject(asteroid)->GetComponent<Transform>(eTransform));

      aT->SetTranslation(Vec3(GetRandom(-800, 800), GetRandom(-600, 600), GetRandom(175, 250)));
    }

  }

}