/*****************************************************************
Filename: CPulser.h
Project:  Gam200
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "types/vectors/Vec2.h"


namespace Framework
{

  class Pulser : public GameComponent
  {
  public:
    Pulser();
    ~Pulser();

    void Initialize();
    void LogicUpdate(float dt);
    void Remove();

    Vec2 m_amount;
    float m_time = 0;

  private:
    float m_cTime;
    bool m_larger = false;
    Vec3 m_tempScale;
  };
}