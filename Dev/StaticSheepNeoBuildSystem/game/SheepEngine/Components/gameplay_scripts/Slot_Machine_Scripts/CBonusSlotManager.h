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

  class CBonusSlotManager: public GameComponent
  {
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

    enum BONUSPRIZE
    {
      COINS,
      CHIP,
      BONUSPRIZESIZE
    };

    enum BONUSPRIZEMOD
    {
      SMALL = 1,
      MED = 2,
      LARGE = 5,
    };

    CBonusSlotManager();
    ~CBonusSlotManager();

    void Initialize();
    void Remove();

    void BonusSlotStart();
    void BonusSlotResults();
    void BonusSlotPrize();

      //messaged functions
    void RoundStart();
    void RoundOver();
    void GivePlayerCoins(int player, int coins);
    void PlayerDied(int playerNum, int killer);


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
      BONUSPRIZE m_prize;
      BONUSPRIZEMOD m_prizeSize;

      bool m_tie;                 //ties are not allowed - this tells whether a tie is occurring
      int m_currentLeader;          //current leader of the bonus
      int m_currentLead;            //current lead value

        //used to make determing winner simpler
      int m_coinDeathKill;          //tracks which part of array to use

      

  };



}