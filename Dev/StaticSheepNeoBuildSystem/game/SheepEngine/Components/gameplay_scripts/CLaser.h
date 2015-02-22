/*****************************************************************
Filename: CLaser.h
Project:  Giga Gravity Games
Author(s): Jon Sourbeer (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../transform/CTransform.h"
#include "components/sprites/CSprite.h"


namespace Framework
{
  

  class Laser : public Sprite
  {
  public:
    Laser();
    ~Laser();
    void LogicUpdate(float dt);
    void Initialize();
    void Remove();
    void SimpleCaster(CircleCollider *lc);
    void ComplexCaster(CircleCollider *lc);

    void(Laser::*Caster)(CircleCollider *lc);

    //member variables
    Handle lTransfrom;
    Handle lCollider;
    Handle lEmitter;
    Handle lBeam;

    unsigned int type;

    float startDelay;   //the amount of time to wait before firing
    float duration;

    float damage;
    int width;  //the width of the total laser, will use an algorithm to determine # of rays to cast

    float arcRotation;  //the arc to trace out over the duration
    float arcDelay;     //the delay on the arc before firing
    float arcPerSec;

    std::vector<Vec3D> positionOffsets; //used for multiple raycasts
    int numberOfRays;

  };
}