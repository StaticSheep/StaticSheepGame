#pragma once

namespace Framework
{
  struct Weapon
  {
    Weapon() {};
    virtual ~Weapon() {};
    virtual void Fire(GameObject*) = 0;

    int delay;  //delay between shots
    int damage; //damage caused by weapon
    bool semi; //is the weapon semi-auto or automatic
  };
}