/*****************************************************************
Filename: GameLogic.cpp
Project: 
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"

#include <iostream>

#include "GameLogic.h"

#include "components/lua/CLuaComponent.h"
#include "components/gamepad/CGamePad.h"
#include "components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/CBullet_default.h"
#include "components/gameplay_scripts/CElevatorPlat.h"
#include "components/gameplay_scripts/CLevel1_Logic.h"
#include "components/gameplay_scripts/CGiantKillBox.h"
#include "components/gameplay_scripts/CGrinder.h"
#include "components/gameplay_scripts/CExplosion.h"
#include "components/sound/CSoundEmitter.h"
#include "components/gameplay_scripts/CWeaponPickup.h"
#include "components/gameplay_scripts/CWarningText.h"
#include "components/gameplay_scripts/CBackgroundPan.h"
#include "components/gameplay_scripts/CCheats.h"
#include "components/slotmachine/slotmachine.h"
#include "components/gameplay_scripts/CAimingArrow.h"
#include "components/gameplay_scripts/CLevel1_Lighting.h"
#include "components/gameplay_scripts/CDashEffect.h"
#include "components/gameplay_scripts/CAsteroid.h"

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
    REGISTER_COMPONENT(LuaComponent);
    REGISTER_COMPONENT(GamePad);
	  REGISTER_COMPONENT(PlayerController);
	  REGISTER_COMPONENT(Bullet_Default);
    REGISTER_COMPONENT(ElevatorPlat);
    REGISTER_COMPONENT(Level1_Logic);
    REGISTER_COMPONENT(Level1_Lighting);
    REGISTER_COMPONENT(SoundEmitter);
    REGISTER_COMPONENT(GiantKillBox);
    REGISTER_COMPONENT(Grinder);
    REGISTER_COMPONENT(Explosion);
    REGISTER_COMPONENT(WeaponPickup);
    REGISTER_COMPONENT(WarningText);
    REGISTER_COMPONENT(BackgroundPan);
    REGISTER_COMPONENT(Cheats);
    REGISTER_COMPONENT(SlotMachine);
    REGISTER_COMPONENT(AimingArrow);
    REGISTER_COMPONENT(DashEffect);
    REGISTER_COMPONENT(Asteroid);
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
  }

  const void* GameLogic::GetDebugData()
  {
    return &m_debugData;
  }

}