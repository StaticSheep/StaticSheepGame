/*****************************************************************
Filename: CollisionGroups.h
*****************************************************************/

#pragma once
enum CollisionGroup{
  NonCollide,
  Collide,
  Resolve,
  Player1,
  Player2,
  Player3,
  Player4,
  Player1Weapon,
  Player2Weapon,
  Player3Weapon,
  Player4Weapon,
  Item,
  PlayerHitOnly,
  Static,
  CollGroupLength
};

enum Collision
{
  NOCOLLIDE,
  COLLIDE,
  RESOLVE,
  COLLNUMBER
};
