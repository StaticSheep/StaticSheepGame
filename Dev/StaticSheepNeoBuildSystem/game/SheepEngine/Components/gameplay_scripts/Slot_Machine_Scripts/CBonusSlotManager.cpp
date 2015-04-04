#include "pch/precompiled.h"
#include "CBonusSlotManager.h"

namespace Framework
{

  void CBonusSlotManager::Initialize()
  {
    space->hooks.Add("RoundStart", self, BUILD_FUNCTION(CBonusSlotManager::RoundStart));
    space->hooks.Add("RoundOver", self, BUILD_FUNCTION(CBonusSlotManager::RoundOver));

    //still to be updated
   // space->hooks.Add("GivePlayerCoins", self, BUILD_FUNCTION(Level1_Logic::GivePlayerCoins));


    //player->space->hooks.Call("GivePlayerCoins", playerController->playerNum, coinAmount);

      //initialize important starting values
    m_currentLead = BONUSMAX;
    m_currentLeader = NOLEADER;    //no current leader, no value awarded

  }

  void CBonusSlotManager::RoundStart()
  {
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

  }


}