/*****************************************************************
Filename: WBase.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

namespace Framework
{
  struct Weapon
  {
    Weapon() {};
    virtual ~Weapon() {};
    virtual void Fire(GameObject*) = 0;
    virtual void Update(float dt) = 0;
    virtual void DelayUpdate(float dt) = 0;
    virtual void ResetDelay() = 0;

    float delay;  //delay between shots
    int damage; //damage caused by weapon
    int knockback; //how much the weapon knocks the player back in zero gravity
    bool semi; //is the weapon semi-auto or automatic
    bool explosive_;
  };
}