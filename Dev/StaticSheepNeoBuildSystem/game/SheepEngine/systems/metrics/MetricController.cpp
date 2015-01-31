/*****************************************************************
Filename: MetricController.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include <iostream>
#include "pch/precompiled.h"
#include "MetricController.h"
#include <fstream>
#include <ctime>

namespace Framework
{
  MetricController *SHEEPMETRICS = nullptr;

  MetricController::MetricController()
  {
    for (int i = 0; i < 4; ++i)
    {
      player_loc[i].clear();
      player_death[i].clear();
    }

    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        player_fire[i][j] = 0;
        player_pickup[i][j] = 0;
      }
    }
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < Buttons::TOTALBUTTONS; ++j)
        player_button_press[i][j] = 0;
    }
    for (int i = 0; i < 4; ++i)
    {
      player_deaths[i] = 0;
      player_kills[i] = 0;
    }

    roundLength = 0.0f;
    roundWinner = 9; //9 is a sentinel value to see if a player has been chosen winner or not
    GameTimer = 0.0f;
    SHEEPMETRICS = this;
  }

  MetricController::~MetricController()
  {
    PrintMetrics();
  }

  void MetricController::Initialize()
  {
    GameTimer = 0.0f;
  }

  void MetricController::Update(float dt)
  {
    GameTimer += dt;
  }
  void MetricController::RecieveMetric(MetricInfo info)
  {
    switch (info.mt)
    {
    case PLAYER_LOCATION:
      UpdateLocation(info.playerNum, GameTimer, info.x, info.y);
      break;
    case PLAYER_DEATH:
      UpdateDeath(info.playerNum, GameTimer, info.x, info.y);
      break;
    case PLAYER_KILL:
      UpdateKill(info.playerNum, GameTimer, info.x, info.y);
      break;
    case PLAYER_BUTTON_PRESS:
      UpdateButtonPress(info.playerNum, info.button);
      break;
    case AUTO_FIRE:
      UpdateFire(info.playerNum, 1);
    case PISTOL_FIRE:
      UpdateFire(info.playerNum, 0);
    case SHOT_FIRE:
      UpdateFire(info.playerNum, 2);
      break;
    case WEAP_PICKUP:
      UpdatePickup(info.playerNum, info.weapon);
      break;
    case ROUND_WINNER:
      SetRoundWinner(info.playerNum);
      break;
    case ROUND_LENGTH:
      SetRoundLength(GameTimer);
      break;
    }
  }
  void MetricController::UpdateFire(char player, char weap)
  {
    ++(player_fire[player][weap]);
  }
  void MetricController::UpdatePickup(char player, Weapons weap)
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
    player_deaths[player]++;
  }
  void MetricController::UpdateKill(char player, float time, int x, int y)
  {
    player_kills[player]++;
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
    if (button < Buttons::TOTALBUTTONS)
      ++(player_button_press[player][button]);
  }
  void MetricController::PrintMetrics()
  {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "metrics\\%d_%m_%Y_%I_%M_%S.txt\0", timeinfo);
    //std::string str(buffer);

    std::ofstream outFile;
    outFile.open(buffer, std::ofstream::out | std::ios_base::app); //open the file and set the mode to append
    if (outFile.is_open()) //double check the file opened
    {
      outFile << "Total Round Time: " << GameTimer << std::endl;
      for (int i = 0; i < 4; ++i)
      {
        outFile << "********************************" << std::endl;
        outFile << "Player " << i + 1 << std::endl;
        outFile << "Kills: " << player_kills[i] << std::endl;
        outFile << "Deaths: " << player_deaths[i] << std::endl;
        //outFile << "Pistol fire: " << player_fire[0][0] << " shots." << std::endl;
        //outFile << "Auto-Gun fire: " << player_fire[0][1] << " shots." << std::endl;
        //outFile << "Shotgun fire: " << player_fire[0][2] << " shots." << std::endl;
        outFile << "Auto-Gun pickups: " << player_pickup[0][1] << std::endl;
        outFile << "Shotgun pickups: " << player_pickup[0][2] << std::endl;
        //outFile << "A-button presses: " << player_button_press[i][Buttons::A] << std::endl;
        //outFile << "B-button presses: " << player_button_press[i][Buttons::B] << std::endl;
        //outFile << "X-button presses: " << player_button_press[i][Buttons::X] << std::endl;
        //outFile << "Y-button presses: " << player_button_press[i][Buttons::Y] << std::endl;
        //outFile << "RT-button presses: " << player_button_press[0][Buttons::RT] << std::endl;
        //outFile << "RB-button presses: " << player_button_press[0][Buttons::RB] << std::endl;
        //outFile << "LT-button presses: " << player_button_press[0][Buttons::LT] << std::endl;
        //outFile << "LB-button presses: " << player_button_press[0][Buttons::LB] << std::endl;
      }
      outFile.close();
    }
    else
      std::cout << "Can not open or print to metrics file!!" << std::endl;
  }

  void MetricController::ReceiveMessage(Message& msg)
  {
    if (msg.MessageId == Message::MetricsData)
    {
      MetricsMessage& mm = (MetricsMessage&)msg;
      RecieveMetric(*(MetricInfo*)mm.data);
    }
  }
}