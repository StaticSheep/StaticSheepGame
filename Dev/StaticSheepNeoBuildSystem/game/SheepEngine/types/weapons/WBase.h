#pragma once

namespace Framework
{
  struct Weapon
  {
    Weapon() {};
    virtual ~Weapon() {};
    virtual void Fire(GameObject*) = 0;

    float delay;  //delay between shots
    int damage; //damage caused by weapon
    int knockback; //how much the weapon knocks the player back in zero gravity
    bool semi; //is the weapon semi-auto or automatic
  };
}