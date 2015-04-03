#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../Level_Scripts/CLevel1_Logic.h"
#include "../../slotmachine/slotmachine.h"
#include "components/controllers/chip/CChipController.h"
#include "types/space/Space.h"

namespace Framework
{

  enum BONUSTYPES
  {
    COINS,
    KILLS,
    DEATHS,
    TYPES
  };

  enum BONUSMODIFIERS
  {
    MOST,
    LEAST,
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
    void GivePlayerCoins(int player, int coins);


    private:
      struct PlayerBonusStats
      {
        int coins;
        int deaths;
        int kills;
      };

      PlayerBonusStats stats[4];

  };



}