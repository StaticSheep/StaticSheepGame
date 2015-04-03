#include "pch/precompiled.h"
#include "CBonusSlotManager.h"

namespace Framework
{

  void CBonusSlotManager::Initialize()
  {
    space->hooks.Add("RoundStart", self, BUILD_FUNCTION(CBonusSlotManager::RoundStart));
    space->hooks.Add("RoundOver", self, BUILD_FUNCTION(CBonusSlotManager::RoundOver));

    //still to be updated
    space->hooks.Add("GivePlayerCoins", self, BUILD_FUNCTION(Level1_Logic::GivePlayerCoins));


    //player->space->hooks.Call("GivePlayerCoins", playerController->playerNum, coinAmount);
  }




}