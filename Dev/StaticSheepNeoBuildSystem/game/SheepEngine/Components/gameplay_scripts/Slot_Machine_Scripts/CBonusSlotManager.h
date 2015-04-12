/*****************************************************************
Filename: CBonusSlotManager.h
Project:  Gam250
Author(s): Jonathan Sourbeer (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../Level_Scripts/CLevel1_Logic.h"
#include "../../slotmachine/slotmachine.h"
#include "components/controllers/chip/CChipController.h"
#include "types/space/Space.h"

namespace Framework
{
  class BonusSlotManager: public GameComponent
  {
    public:
      #define BONUSMAX	2147483647  //max int size
      #define BONUSNOLEADER 4       //one past highest player number

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
        BONECHIP,
        BTWOCHIP,
        BCOINS,
        BONUSPRIZESIZE
      };

      enum RESULT_TYPE
      {
        BonusType, // Coins, Deaths, Kills
        BonusMod, // Least, Most
        BonusPrize, // OneChip, TwoChip, Coins
      };

      struct BResultData
      {
        BONUSMODIFIERS mod;
        BONUSTYPES type;
        BONUSPRIZE prize;
        bool left;
      };

      BonusSlotManager();
      ~BonusSlotManager();

      void Initialize();
      void Remove();

      void BonusSlotPrize(int number);

        //messaged functions
      void BonusResult(BResultData result);
      void RoundOver();
      void GivePlayerCoins(int player, int coins);
      void PlayerDied(int playerNum, int killer);

    private:

        //used for tracking player scores during round
      struct bonusStats
      {
        int coins;
        int deaths;
        int kills;
      };

        //ease of access and comparison
      struct PlayerBonusStats
      {
        union
        {
          int cdk[3];
          bonusStats stats;
        };
      };

        //critical values for round
      PlayerBonusStats m_stats[4];

      struct BonusInfo
      {    
        BONUSMODIFIERS  m_mod;
        BONUSPRIZE m_prize;

        //used to make determing winner simpler
        bool  m_tie;                    //ties are not allowed - this tells whether a tie is occurring
        int   m_currentLeader;          //current leader of the bonus
        int   m_currentLead;            //current lead value
        int   m_coinDeathKill;          //tracks which part of array to use - coins, deaths, or kills

        bool  m_active;
      };

      BonusInfo m_bonusInfo[2];

  };
}