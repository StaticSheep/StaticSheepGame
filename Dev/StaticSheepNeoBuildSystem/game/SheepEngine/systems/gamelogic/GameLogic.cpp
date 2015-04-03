/*****************************************************************
Filename: GameLogic.cpp
Project: 
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"

#include <iostream>

#include "GameLogic.h"
#include "engine/factory/Factory.h"

#include "components/lua/CLuaComponent.h"
#include "components/gamepad/CGamePad.h"
#include "components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/Weapon_Scripts//CBullet_default.h"
#include "components/gameplay_scripts/Level_Event_Scripts/CElevatorPlat.h"
#include "components/gameplay_scripts/Level_Scripts/CLevel1_Logic.h"
#include "components/gameplay_scripts/Level_Event_Scripts/CGiantKillBox.h"
#include "components/gameplay_scripts/Level_Scripts/CGrinder.h"
#include "components/gameplay_scripts/Level_Scripts/CPinwheel.h"
#include "components/gameplay_scripts/Level_Event_Scripts/CSaucer.h"
#include "components/gameplay_scripts/Level_Event_Scripts/CSaucerMissile.h"
#include "components/gameplay_scripts/FX_Scripts/CExplosion.h"
#include "components/sound/CSoundEmitter.h"
#include "components/gameplay_scripts/Pickup_Scripts/CWeaponPickup.h"
#include "components/gameplay_scripts/Level_Event_Scripts/CWarningText.h"
#include "components/gameplay_scripts/Level_Scripts/CBackgroundPan.h"
#include "components/gameplay_scripts/CCheats.h"
#include "components/slotmachine/slotmachine.h"

#include "components/gameplay_scripts/arena/CBlockLights.h"
#include "components/tester/CTester.h"

#include "components/gameplay_scripts/Player_Scripts/CAimingArrow.h"
#include "components/gameplay_scripts/Level_Scripts/CLevel1_Lighting.h"
#include "components/gameplay_scripts/Player_Scripts/CDashEffect.h"
#include "components/gameplay_scripts/FX_Scripts/CJuggernautEffect.h"
#include "components/gameplay_scripts/Level_Event_Scripts/CAsteroid.h"
#include "components/gameplay_scripts/Pickup_Scripts/CPowerupPickup.h"
#include "components/gameplay_scripts/Slot_Machine_Scripts/CSlotController.h"
#include "components/gameplay_scripts/Weapon_Scripts/CLaser.h"
#include "components/gameplay_scripts/Slot_Machine_Scripts/CCoinBall.h"
#include "components/gameplay_scripts/FX_Scripts/CAOEDamage.h"
#include "components/gameplay_scripts/Weapon_Scripts/CImpactEffect.h"
#include "Components/controllers/chip/CChipController.h"
#include "Components/controllers/round/CRoundController.h"
#include "Components/controllers/round/CRoundText.h"
#include "Components/controllers/round/CRoundResults.h"
#include "components/gameplay_scripts/FX_Scripts/CPulser.h"
#include "components/gameplay_scripts/Slot_Machine_Scripts/CPersonalSlotController.h"
#include "Components/controllers/lobby/CLobbyController.h"
#include "Components/controllers/lobby/CJoinText.h"
#include "components/gameplay_scripts/Slot_Machine_Scripts/CPersonalSlotSpawner.h"
#include "components/controllers/light patterns/CLightPatternController.h"
#include "Components/controllers/lobby/CTutorialText.h"

namespace Framework
{
  // Global pointer
  GameLogic* GAMELOGIC = NULL;

  GameLogic::GameLogic()
  {

    GAMELOGIC = this;
  }

  GameLogic::~GameLogic()
  {
    GAMELOGIC = nullptr;
    // Free anything that was allocated
  }

  void GameLogic::RegisterComponents()
  {
    REGISTER_COMPONENT(Tester);
    REGISTER_COMPONENT(LuaComponent);
    REGISTER_COMPONENT(GamePad);
	  REGISTER_COMPONENT(PlayerController);
	  REGISTER_COMPONENT(Bullet_Default);
    REGISTER_COMPONENT(ElevatorPlat);
    REGISTER_COMPONENT(Level1_Logic);
    REGISTER_COMPONENT(Level1_Lighting);
    REGISTER_COMPONENT(SoundEmitter);
    REGISTER_COMPONENT(GiantKillBox);
    REGISTER_COMPONENT(Saucer);
    REGISTER_COMPONENT(SaucerMissile);
    REGISTER_COMPONENT(Grinder);
    REGISTER_COMPONENT(Pinwheel);
    REGISTER_COMPONENT(Explosion);
    REGISTER_COMPONENT(WeaponPickup);
    REGISTER_COMPONENT(WarningText);
    REGISTER_COMPONENT(BackgroundPan);
    REGISTER_COMPONENT(Cheats);
    REGISTER_COMPONENT(SlotMachine);
    REGISTER_COMPONENT(AimingArrow);
    REGISTER_COMPONENT(DashEffect);
    REGISTER_COMPONENT(JuggernautEffect);
    REGISTER_COMPONENT(Asteroid);
    REGISTER_COMPONENT(PowerupPickup);
    REGISTER_COMPONENT(SlotController);
    REGISTER_COMPONENT(PersonalSlotController);
    REGISTER_COMPONENT(BlockLights);
    //REGISTER_COMPONENT(Laser);
    REGISTER_COMPONENT(CoinBall);
    REGISTER_COMPONENT(ImpactEffect);
    REGISTER_COMPONENT(AOEDamage);
    REGISTER_COMPONENT(ChipController);
    REGISTER_COMPONENT(RoundController);
    REGISTER_COMPONENT(LobbyController);
    REGISTER_COMPONENT(RoundText);
    REGISTER_COMPONENT(RoundResults);
    REGISTER_COMPONENT(Pulser);
    REGISTER_COMPONENT(JoinText);
    REGISTER_COMPONENT(TutorialText);
    REGISTER_COMPONENT(SpineLight);
    REGISTER_COMPONENT(LightPatternController);
    REGISTER_COMPONENT(PersonalSlotSpawner);
  }

  void GameLogic::Initialize()
  {
  }

  void GameLogic::Update(float dt)
  {
    GameSpace* space;
    std::vector<GameSpace*> removeList;

    m_debugData = { 0 };

    for (auto it = ENGINE->m_spaces.begin(); it != ENGINE->m_spaces.end(); ++it)
    {
      space = *it;

      if (!space->m_ready)
        continue;

      if (!space->Paused())
        space->hooks.Call("LogicUpdate", dt);

      space->hooks.Call("FrameUpdate", dt);

#if SHEEP_DEBUG
      ++(m_debugData.numSpaces);
      m_debugData.objectsAllocated += space->m_objects.Size();
      for (size_t i = 0; i < ecountComponents; ++i)
      {
        m_debugData.componentsAllocated += space->m_components[i].Size();
      }
#endif
    }

    Lua::CallFunc(ENGINE->Lua(), "hook.Call", "LogicUpdate", dt);
    Lua::CallFunc(ENGINE->Lua(), "hook.Call", "FrameUpdate", dt);

    Lua::CallFunc(ENGINE->Lua(), "gui.Update");
    Lua::CallFunc(ENGINE->Lua(), "uimenu.Update");

    for (auto it = ENGINE->m_spaces.begin(); it != ENGINE->m_spaces.end(); ++it)
    {
      space = *it;

      if (!space->m_ready)
        continue;

      space->Cleanup();
    }

  }

  void GameLogic::ReceiveMessage(Message& msg)
  {
    if (msg.MessageId == Message::GUIDraw)
      Lua::CallFunc(ENGINE->Lua(), "gui.Draw");
    
    if (msg.MessageId == Message::PostGUIDraw)
      Lua::CallFunc(ENGINE->Lua(), "hook.Call", "PostDraw");
  }

  const void* GameLogic::GetDebugData()
  {
    return &m_debugData;
  }

}