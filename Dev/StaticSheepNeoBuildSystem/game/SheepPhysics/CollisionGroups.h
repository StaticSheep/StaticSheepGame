/*****************************************************************
Filename: CollisionGroups.h
Project:
Author(s): Jon Sourbeer (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
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
  RayCast,
  CollGroupLength
};

enum Collision
{
  NOCOLLIDE,
  COLLIDE,
  RESOLVE,
  COLLNUMBER
};