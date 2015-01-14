/*****************************************************************
Filename: MetricController.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include "Components\gamepad\CGamePad.h"
#include "components\gameplay_scripts\CCheats.h"

namespace Framework
{
  //this enum keeps track of the different kinds of metrics to take
  enum MetricType
  {
    WEAP_PICKUP,
    AUTO_FIRE,
    SHOT_FIRE,
    PISTOL_FIRE,
    PLAYER_LOCATION,
    PLAYER_DEATH,
    PLAYER_BUTTON_PRESS,
    ROUND_WINNER,
    ROUND_LENGTH
  };
  //this struct will be sent with messages to the metric controller
  struct MetricInfo
  {
    int playerNum;
    float timeOfOccurance;
    MetricType mt;
    char words[2];
    int x;
    int y;
  };

  class MetricController : public ISystem
  {
  public:
    MetricController();
    ~MetricController();

    void Initialize();
    void Update(float dt);
    void RecieveMetric(MetricInfo info);
    void UpdateFire(char player, char weap);
    void UpdatePickup(char player, char weap);
    void UpdateLocation(char player, float time, int x, int y);
    void UpdateDeath(char player, float time, int x, int y);
    void SetRoundLength(float time);
    void SetRoundWinner(char player);
    void UpdateButtonPress(char player, Buttons button);

  private:
    //an array to record shots from the three different weapons, as well as one for how many times weapons are picked up
    unsigned int player_fire[4][3], player_pickup[4][3], player_button_press[4][10];

    //alright, i know this looks gross, but all it is is a vector of pairs
    //of which one element of that pair is another pair.
    //this is to record time, and location (x,y) to make heat maps
    std::vector <std::pair<float, std::pair<int, int>>> player_loc[4], player_death[4];
    float roundLength;
    char roundWinner; //one through four
  };
}