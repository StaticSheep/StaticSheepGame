/*****************************************************************
Filename: CImpactEffect.h
Project:  Gam200
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include <string>

namespace Framework
{
  class ImpactEffect : public GameComponent
  {
  public:
    ImpactEffect();
    ~ImpactEffect();

    void Initialize();
    void OnImpact(int playerOwner);
    void LogicUpdate(float dt);
    void Remove();

    std::string m_effect;
  };
}