#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../Level_Scripts/CLevel1_Logic.h"
#include "../../slotmachine/slotmachine.h"
#include "components/controllers/chip/CChipController.h"
#include "types/space/Space.h"

namespace Framework
{
  #define BONUSMAX	2147483647
  #define NOLEADER 4

  enum BONUSTYPES
  {
    COINS,
    DEATHS,
    KILLS,
    TYPES
  };

    //MOST is set as negative one to make one 'if' check work for determining winner
    //MOST will result in a more negative number, which means it will work against BONUSMAX
  enum BONUSMODIFIERS
  {
    MOST = -1,
    LEAST = 1,
    MODIFIERS
  };

  class CBonusSlotManager: public GameComponent
  {
    CBonusSlotManager();
    ~CBonusSlotManager();

    void Initialize();
    void Remove();

    void BonusSlotStart();
    void BonusSlotResults();


      //messaged functions
    void RoundStart();
    void RoundOver();
    //void GivePlayerCoins(int player, int coins);


    private:

      struct bonusStats
      {
        int coins;
        int deaths;
        int kills;
      };

      struct PlayerBonusStats
      {
        union
        {
          int cdk[3];
          bonusStats stats;
        };
        
      };

      PlayerBonusStats m_stats[4];
      BONUSTYPES m_type;
      BONUSMODIFIERS  m_mod;

      bool m_tie;                 //ties are not allowed - this tells whether a tie is occurring
      int m_currentLeader;          //current leader of the bonus
      int m_currentLead;            //current lead value

        //used to make determing how to determine winner simpler
      int m_coinDeathKill;          //tracks which part of array to use

      

  };



}