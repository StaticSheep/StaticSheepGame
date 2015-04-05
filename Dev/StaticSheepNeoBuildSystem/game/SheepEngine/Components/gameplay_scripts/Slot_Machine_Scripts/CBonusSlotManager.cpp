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
    space->hooks.Add("RoundStart", self, BUILD_FUNCTION(BonusSlotManager::RoundStart));
    space->hooks.Add("RoundOver", self, BUILD_FUNCTION(BonusSlotManager::RoundOver));
    space->hooks.Add("GivePlayerCoins", self, BUILD_FUNCTION(BonusSlotManager::GivePlayerCoins));
    space->hooks.Add("PlayerDied", self, BUILD_FUNCTION(BonusSlotManager::PlayerDied));
    
      //initialize important starting values
    m_currentLead   = BONUSMAX;
    m_currentLeader = BONUSNOLEADER;    //no current leader, no value awarded

    m_coinDeathKill = COINS;
    m_mod = MOST;
    m_prize = BCHIP;
    m_prizeSize = SMALL;

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
    space->hooks.Remove("RoundStart", self);
    space->hooks.Remove("RoundOver", self);
    space->hooks.Remove("GivePlayerCoins", self);
    space->hooks.Remove("PlayerDied", self);
  }

  void BonusSlotManager::BonusSlotStart()
  {

  }

  void BonusSlotManager::BonusSlotResults()
  {

  }

  void BonusSlotManager::BonusSlotPrize()
  {
    //tie resulted - dispatch message for tie and no winner, or perhaps there should be a winner?
    if (m_tie)
      return;

    switch (m_prize)
    {
      case BCOINS:
        space->hooks.Call("GivePlayerCoins", m_currentLeader, m_prizeSize * 1000);
        break;

      case BCHIP:
        space->hooks.Call("GivePlayerChip", m_currentLeader, (int)m_prizeSize);
        break;
    }
  }

  void BonusSlotManager::RoundStart()
  {
   
  }

  void BonusSlotManager::RoundOver()
  {
      //determine winner
    for (int i = 0; i < 4; ++i)
    {
      if ((m_stats[i].cdk[m_coinDeathKill] * m_mod) < m_currentLead)
      {
        m_currentLead = m_stats[i].cdk[m_coinDeathKill] * m_mod;
        m_currentLeader = i;
        m_tie = false;
      }
      else if (m_stats[i].cdk[m_coinDeathKill] == m_currentLead)
      {
        m_tie = true;
      }
    }

      //dispatch prize
    BonusSlotPrize();

    space->GetGameObject(owner)->Destroy();
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