/*****************************************************************
Filename: MetricController.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

//#include "components/gamepad/CGamePad.h"
#include "components/gameplay_scripts/CCheats.h"
#include "systems/metrics/MetricInfo.h"

namespace Framework
{

  class MetricController : public ISystem
  {
  public:
    MetricController();
    ~MetricController();

    virtual std::string GetName() { return "SheepMetrics"; };

    void Initialize();
    void Update(float dt);
    void RecieveMetric(MetricInfo info);
    void UpdateFire(char player, char weap);
    void UpdatePickup(char player, Weapons weap);
    void UpdateLocation(char player, float time, int x, int y);
    void UpdateDeath(char player, float time, int x, int y);
    void UpdateKill(char player, float time, int x, int y);
    void SetRoundLength(float time);
    void SetRoundWinner(char player);
    void ReceiveMessage(Message& msg);
    void UpdateButtonPress(char player, Buttons button);
    void PrintMetrics();
    float GameTimer;

  private:
    //an array to record shots from the three different weapons, as well as one for how many times weapons are picked up
    unsigned int player_fire[4][3], player_pickup[4][5], player_button_press[4][Buttons::TOTALBUTTONS];
    unsigned int player_deaths[4], player_kills[4];

    //alright, i know this looks gross, but all it is is a vector of pairs
    //of which one element of that pair is another pair.
    //this is to record time, and location (x,y) to make heat maps
    std::vector <std::pair<float, std::pair<int, int>>> player_loc[4], player_death[4];
    float roundLength;
    char roundWinner; //one through four
    
  };

  extern MetricController *SHEEPMETRICS;

}