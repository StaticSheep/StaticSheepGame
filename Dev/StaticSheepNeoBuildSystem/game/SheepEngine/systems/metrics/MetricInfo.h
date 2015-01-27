/*****************************************************************
Filename: MetricInfo.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

enum MetricType
{
  WEAP_PICKUP,
  AUTO_FIRE,
  SHOT_FIRE,
  PISTOL_FIRE,
  PLAYER_LOCATION,
  PLAYER_DEATH,
  PLAYER_KILL,
  PLAYER_BUTTON_PRESS,
  ROUND_WINNER,
  ROUND_LENGTH
};

enum Buttons{
  A,      //0
  B,      //1
  X,      //2
  Y,      //3
  UP,     //4
  DOWN,   //5
  LEFT,   //6
  RIGHT,  //7
  LT,  //8
  RT, //9
  LB,  //10
  RB, //11
  START,  //12
  SELECT,  //13
  TOTALBUTTONS //14
};

enum Weapons
{
  PISTOL,
  AUTO,
  SHOTGUN
};

//this struct will be sent with messages to the metric controller
struct MetricInfo
{
  MetricInfo(){};
  MetricInfo(int pn, int x_, int y_, MetricType mt, Buttons button, Weapons weap) : playerNum(pn), x(x_), y(y_), mt(mt), button(button), weapon(weap){};

  int playerNum;
  MetricType mt;
  Buttons button;
  Weapons weapon;
  int x;
  int y;
};