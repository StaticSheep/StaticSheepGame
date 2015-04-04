#include "pch/precompiled.h"
#include "CBonusSlotManager.h"

namespace Framework
{

  void CBonusSlotManager::Initialize()
  {
    space->hooks.Add("RoundStart", self, BUILD_FUNCTION(CBonusSlotManager::RoundStart));
    space->hooks.Add("RoundOver", self, BUILD_FUNCTION(CBonusSlotManager::RoundOver));
    space->hooks.Add("GivePlayerCoins", self, BUILD_FUNCTION(CBonusSlotManager::GivePlayerCoins));
    space->hooks.Add("PlayerDied", self, BUILD_FUNCTION(CBonusSlotManager::PlayerDied));//m_playerNum, m_killer);
    
      //initialize important starting values
    m_currentLead = BONUSMAX;
    m_currentLeader = NOLEADER;    //no current leader, no value awarded

  }

  void CBonusSlotManager::Remove()
  {
    space->hooks.Remove("RoundStart", self);
    space->hooks.Remove("RoundOver", self);
    space->hooks.Remove("GivePlayerCoins", self);
    space->hooks.Remove("PlayerDied", self);

  }

  void CBonusSlotManager::BonusSlotStart()
  {

  }

  void CBonusSlotManager::BonusSlotResults()
  {

  }

  void CBonusSlotManager::BonusSlotPrize()
  {
    //tie resulted - dispatch message for tie and no winner, or perhaps there should be a winner?
    if (m_tie)
      return;

    switch (m_prize)
    {
    case COINS:
      space->hooks.Call("GivePlayerCoins", m_currentLeader, m_prizeSize * 1000);
      break;

    case CHIP:
      ChipController *CC = space->GetGameObject(owner)->GetComponent<ChipController>(eChipController);
      CC->GivePlayerChip(m_currentLeader, m_prizeSize);
      break;
    }
  }

  void CBonusSlotManager::RoundStart()
  {
      //iterate through all players
    for (int i = 0; i < 4; ++i)
    {
      m_stats[i].stats.coins = 0;
      m_stats[i].stats.deaths = 0;
      m_stats[i].stats.kills = 0;
    }
  }

  void CBonusSlotManager::RoundOver()
  {
      //determine winner
    for (int i = 0; i < 4; ++i)
    {
      if (m_stats[i].cdk[m_coinDeathKill] < m_currentLead)
      {
        m_currentLead = m_stats[i].cdk[m_coinDeathKill];
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
  }

  void CBonusSlotManager::GivePlayerCoins(int playerNum, int coins)
  {
    m_stats[playerNum].stats.coins += coins;
  }

  void CBonusSlotManager::PlayerDied(int playerNum, int killer)
  {
    ++m_stats[playerNum].stats.deaths;
    ++m_stats[killer].stats.kills;
  }
}