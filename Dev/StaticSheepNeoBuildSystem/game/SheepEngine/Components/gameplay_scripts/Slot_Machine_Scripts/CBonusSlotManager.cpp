/*****************************************************************
Filename: CBonusSlotManager.h
Project:  Gam250
Author(s): Zachary Nawar (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "CBonusSlotManager.h"

namespace Framework
{

  BonusSlotManager::BonusSlotManager()
  {

  }

  BonusSlotManager::~BonusSlotManager()
  {

  }

  void BonusSlotManager::Initialize()
  {
    space->hooks.Add("BonusResult", self, BUILD_FUNCTION(BonusSlotManager::BonusResult));
    space->hooks.Add("RoundOver", self, BUILD_FUNCTION(BonusSlotManager::RoundOver));
    space->hooks.Add("GivePlayerCoins", self, BUILD_FUNCTION(BonusSlotManager::GivePlayerCoins));
    space->hooks.Add("PlayerDied", self, BUILD_FUNCTION(BonusSlotManager::PlayerDied));
    
      //initialize important starting values
    for (int number = 0; number < 2; ++number)
    {
      m_bonusInfo[number].m_currentLead = BONUSMAX;
      m_bonusInfo[number].m_currentLeader = BONUSNOLEADER;    //no current leader, no value awarded
    }

    //iterate through all players
    for (int i = 0; i < 4; ++i)
    {
      m_stats[i].stats.coins = 0;
      m_stats[i].stats.deaths = 0;
      m_stats[i].stats.kills = 0;
    }
  }

  void BonusSlotManager::Remove()
  {
    space->hooks.Remove("BonusResult", self);
    space->hooks.Remove("RoundOver", self);
    space->hooks.Remove("GivePlayerCoins", self);
    space->hooks.Remove("PlayerDied", self);
  }

  void BonusSlotManager::BonusSlotPrize(int number)
  {
    //tie resulted - dispatch message for tie and no winner, or perhaps there should be a winner?
    if (m_bonusInfo[number].m_tie)
      return;

      //prizes temporarily disabled
    switch (m_bonusInfo[number].m_prize)
    {
      case BCOINS:
        //space->hooks.Call("GivePlayerCoins", m_bonusInfo[number].m_currentLeader, 15000);
        break;

      case BONECHIP:
        //space->hooks.Call("GivePlayerChip", m_bonusInfo[number].m_currentLeader, 1);
        break;

      case BTWOCHIP:
        //space->hooks.Call("GivePlayerChip", m_bonusInfo[number].m_currentLeader, 2);
        break;
    }

    //for now, end the active slots here
    m_bonusInfo[number].m_active = false;
  }


  void BonusSlotManager::BonusResult(BResultData result)
  {
    //determine which number of array to fill
    int number = 0;
    if (result.left)
      number = 1;

    m_bonusInfo[number].m_coinDeathKill = result.type;
    m_bonusInfo[number].m_mod = (BONUSMODIFIERS)result.mod;
    m_bonusInfo[number].m_prize = (BONUSPRIZE)result.prize;
    m_bonusInfo[number].m_active = true;

    //********************************
    //filler code for coin ball spawns
    //REMOVE when slot implemented

    int coinballs = 0;
    if (result.type == COINS)
      ++coinballs;

    if (result.type == KILLS)
      ++coinballs;

    if (result.type == DEATHS)
      coinballs += 2;

    if (result.mod == MOST)
      ++coinballs;

    if (result.prize == BCOINS)
      ++coinballs;

    if (result.prize == BONECHIP)
      ++coinballs;

    if (result.prize == BTWOCHIP)
      coinballs += 2;

    for (int i = 0; i < coinballs; ++i)
    {
      float ranX = GetRandom(-600, 600);
      float ranY = GetRandom(-300, 300);
      Vec3 pos(ranX, ranY, 0.0f);
      space->hooks.Call("SpawnItem", "CoinBall", pos);
    }
  }

  void BonusSlotManager::RoundOver()
  {
    for (int number = 0; number < 2; ++number)
    {
        //skip if not active slot
      if (!m_bonusInfo[number].m_active)
        continue;

        //determine winner
      for (int i = 0; i < 4; ++i)
      {
        if ((m_stats[i].cdk[m_bonusInfo[number].m_coinDeathKill] * m_bonusInfo[number].m_mod) < m_bonusInfo[number].m_currentLead)
        {
          m_bonusInfo[number].m_currentLead = m_stats[i].cdk[m_bonusInfo[number].m_coinDeathKill] * m_bonusInfo[number].m_mod;
          m_bonusInfo[number].m_currentLeader = i;
          m_bonusInfo[number].m_tie = false;
        }
        else if (m_stats[i].cdk[m_bonusInfo[number].m_coinDeathKill] == m_bonusInfo[number].m_currentLead)
        {
          m_bonusInfo[number].m_tie = true;
        }
      }

        //dispatch prize
      BonusSlotPrize(number);
    }
  }

  void BonusSlotManager::GivePlayerCoins(int playerNum, int coins)
  {
    m_stats[playerNum].stats.coins += coins;
  }

  void BonusSlotManager::PlayerDied(int playerNum, int killer)
  {
    ++m_stats[playerNum].stats.deaths;
    ++m_stats[killer].stats.kills;
  }
}