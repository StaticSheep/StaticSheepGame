/*****************************************************************
Filename: LEGrinderBig.cpp
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "LEGrinderBig.h"
#include "Components/transform/CTransform.h"
#include "Components/colliders/CCircleCollider.h"
#include "Components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/Weapon_Scripts/CBullet_default.h"
#include "components/particles/CParticleCircleEmitter.h"
#include "components/particles/CParticleSystem.h"
#include "../SheepUtil/include/Matrix3D.h"
#include "components/gameplay_scripts/Level_Event_Scripts/CGiantKillBox.h"
#include "Components/sprites/CSprite.h"
#include "components/gameplay_scripts/arena/CBlockLights.h"
#include "boost/filesystem/operations.hpp"

namespace Framework
{
  LEGrinderBig::LEGrinderBig()
  {
    timer = 7.0f;
    warned = false;
  }

  LEGrinderBig::~LEGrinderBig()
  {

  }

  void LEGrinderBig::Update(float dt)
  {
    timer -= dt;

    if (timer <= 2.5f && !warned)
    {
      Warn();
    }

    if (timer <= 0)
    {
      FireEventSmall(LC);
      timer = 7.0f;
    }
  }

  void LEGrinderBig::Warn()
  {
    warned = true;

    if (GetRandom(0, 1))
      spawnLeft = true;
    else
      spawnLeft = false;

    BlockLights::EventData ed;

    ed.duration = 2.5f;
    ed.settings.color = Vec4(1.0f, 0.0f, 0.0f, 0.8f);
    ed.settings.fx = BlockLights::CUSTOM;
    ed.settings.customData.duration = 2.0f;
    ed.settings.customSequence = "zzzmmzzzmmzzzzmmzzzzmmzzzmmmzzzmmmzzzmzzzzmmm";

    
    

    if (spawnLeft)
    {
      LC->space->hooks.Call("LightingEvent", (unsigned)(0x1 | 0x2 | 0x4 | 0x8 | 0x16), &ed);
    }
    else
    {
      LC->space->hooks.Call("LightingEvent", (unsigned)(16384 + 32768 + 65536 + 131072 + 262144), &ed);
    }
  }

  void LEGrinderBig::FireEvent(GameObject *LogicController)
  {
    LC = LogicController;
    GameObject *eGiantPlat = (FACTORY->LoadObjectFromArchetype(LogicController->space, "KillBoxBig"));
    Transform *GPT = eGiantPlat->GetComponent<Transform>(eTransform);
    if (GetRandom(0, 1))
    {
      eGiantPlat->GetComponent<GiantKillBox>(eGiantKillBox)->direction = true;
      GPT->SetTranslation(Vec3(1000.0, 0.0, 1.0));
    }
    else
    {
      eGiantPlat->GetComponent<GiantKillBox>(eGiantKillBox)->direction = false;
      GPT->SetTranslation(Vec3(-1000.0, 0.0, 1.0));
    }
  }

  void LEGrinderBig::FireEventSmall(GameObject *LogicController)
  {
    int numGrinders = 3;
    LC = LogicController;

    GameObject *eGiantPlat = (FACTORY->LoadObjectFromArchetype(LogicController->space, "KillBoxBig"));
    Transform *GPT = eGiantPlat->GetComponent<Transform>(eTransform);
    BoxCollider *GPC = eGiantPlat->GetComponent<BoxCollider>(eBoxCollider);

    GPC->SetBodyCollisionGroup("NonCollide");
    GPT->SetScale(Vec3(2.0f, 0.5f, 1.0f));

    float ranY = (float)GetRandom(-250, 250);
    if (!spawnLeft)
    {
      eGiantPlat->GetComponent<GiantKillBox>(eGiantKillBox)->direction = true;
      eGiantPlat->GetComponent<GiantKillBox>(eGiantKillBox)->numOfGrinders = numGrinders;
      GPT->SetTranslation(Vec3(1000.0, ranY, 1.0));
    }
    else
    {
      eGiantPlat->GetComponent<GiantKillBox>(eGiantKillBox)->direction = false;
      eGiantPlat->GetComponent<GiantKillBox>(eGiantKillBox)->numOfGrinders = numGrinders;
      GPT->SetTranslation(Vec3(-1000.0, ranY, 1.0));
    }

    warned = false;
  }

}


