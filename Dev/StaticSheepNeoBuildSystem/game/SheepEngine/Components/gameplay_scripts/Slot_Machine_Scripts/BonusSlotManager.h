#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../Level_Scripts/CLevel1_Logic.h"
#include "../../slotmachine/slotmachine.h"
#include "components/controllers/chip/CChipController.h"

namespace Framework
{

  enum BONUSTYPES
  {
    COINS,
    KILLS,
    TYPES
  };

  enum BONUSMODIFIERS
  {
    MOST,
    LEAST,
    MODIFIERS
  };

  class BonusSlotManager
  {
    BonusSlotManager();
    ~BonusSlotManager();

    void Initialize();
    void Remove();

    void BonusSlotResults();



  };



}