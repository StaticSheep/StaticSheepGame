/*****************************************************************
Filename: CImpactEffect.cpp
Project:  Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "CImpactEffect.h"
#include "engine/factory/Factory.h"
#include "../../transform/CTransform.h"

namespace Framework
{
  ImpactEffect::ImpactEffect()
  {

  }

  ImpactEffect::~ImpactEffect()
  {

  }

  void ImpactEffect::Initialize()
  {
    space->GetGameObject(owner)->hooks.Add("BulletImpact",
      self, BUILD_FUNCTION(ImpactEffect::OnImpact));
  }

  void ImpactEffect::OnImpact()
  {
    if (m_effect.length())
    {
      GameObject* obj = 
        FACTORY->LoadObjectFromArchetype(space, m_effect.c_str());

      Transform* sTrans = obj->GetComponent<Transform>(eTransform);
      Transform* ourTrans = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);

      sTrans->SetTranslation(ourTrans->GetTranslation());
    }
  }

  void ImpactEffect::Remove()
  {
    space->GetGameObject(owner)->hooks.Remove("BulletImpact", self);
  }


}