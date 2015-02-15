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

namespace Framework
{
  LEGrinderBig::LEGrinderBig()
  {

  }

  LEGrinderBig::~LEGrinderBig()
  {

  }

  void LEGrinderBig::Update(float dt)
  {

  }

  void LEGrinderBig::FireEvent(GameObject *LogicController)
  {
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

}


