#pragma once

namespace Framework
{
  struct Weapon
  {
    Weapon() {};
    virtual ~Weapon() {};

    virtual void Fire() = 0;

    int delay;  //delay between shots
    int damage; //damage caused by weapon
  };
}