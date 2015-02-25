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
#include "components/gameplay_scripts/CBullet_default.h"
#include "components/particles/CParticleCircleEmitter.h"
#include "components/particles/CParticleSystem.h"
#include "../SheepUtil/include/Matrix3D.h"
#include "components/gameplay_scripts/CGiantKillBox.h"
#include "Components/sprites/CSprite.h"

namespace Framework
{
  LEGrinderBig::LEGrinderBig()
  {
    timer = 5.0f;
  }

  LEGrinderBig::~LEGrinderBig()
  {

  }

  void LEGrinderBig::Update(float dt)
  {
    timer -= dt;
    if (timer <= 0)
    {
      FireEventSmall(LC);
      timer = 7.0f;
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

    float ranY = (float)GetRandom(-300, 300);
    if (GetRandom(0, 1))
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
  }

}


