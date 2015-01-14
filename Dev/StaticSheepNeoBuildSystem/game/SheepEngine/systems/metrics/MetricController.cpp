/*****************************************************************
Filename: MetricController.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "MetricController.h"

namespace Framework
{
  MetricController::MetricController()
  {

  }

  MetricController::~MetricController()
  {

  }

  void MetricController::Initialize()
  {

  }

  void MetricController::Update(float dt)
  {

  }
  void MetricController::RecieveMetric(MetricInfo info)
  {
    switch (info.mt)
    {
    case PLAYER_LOCATION:
      UpdateLocation(info.playerNum, info.timeOfOccurance, info.x, info.y);
      break;
    case PLAYER_DEATH:
      UpdateLocation(info.playerNum, info.timeOfOccurance, info.x, info.y);
      break;
    case PLAYER_BUTTON_PRESS:
      break;
    case AUTO_FIRE:
      UpdateFire(info.playerNum, 1);
    case PISTOL_FIRE:
      UpdateFire(info.playerNum, 0);
    case SHOT_FIRE:
      UpdateFire(info.playerNum, 2);
      break;
    case WEAP_PICKUP:
      UpdatePickup(info.playerNum, info.words[0]);
      break;
    case ROUND_WINNER:
      SetRoundWinner(info.playerNum);
      break;
    case ROUND_LENGTH:
      SetRoundLength(info.timeOfOccurance);
      break;
    }
  }
  void MetricController::UpdateFire(char player, char weap)
  {
    ++(player_fire[player][weap]);
  }
  void MetricController::UpdatePickup(char player, char weap)
  {
    ++(player_pickup[player][weap]);
  }
  void MetricController::UpdateLocation(char player, float time, int x, int y)
  {
    std::pair<float, std::pair<int, int>> metric_loc(time, std::pair<int,int>(x,y));
    player_loc[player].push_back(metric_loc);
  }
  void MetricController::UpdateDeath(char player, float time, int x, int y)
  {
    std::pair<float, std::pair<int, int>> metric_loc(time, std::pair<int, int>(x, y));
    player_death[player].push_back(metric_loc);
  }
  void MetricController::SetRoundLength(float time)
  {
    roundLength = time;
  }
  void MetricController::SetRoundWinner(char player)
  {
    roundWinner = player;
  }
  void MetricController::UpdateButtonPress(char player, Buttons button)
  {
    ++(player_button_press[player][button]);
  }
}