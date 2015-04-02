/*****************************************************************
Filename: CStarController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CLobbyController.h"
#include "types/space/Space.h"
#include "CJoinText.h"
#include "../../sprites/CSprite.h"
#include "../../gameplay_scripts/Level_Scripts/CLevel1_Logic.h"
#include "../../sound/CSoundPlayer.h"
#include "../../gameplay_scripts/arena/CBlockLights.h"

static const char *playerNames[] = { "Player1", "Player2", "Player3", "Player4" };

namespace Framework
{
  bool playerJoined[4] = { false, false, false, false };

  LobbyController::LobbyController()
  {
    numOfPlayers = 0;
    for (int i = 0; i < 4; ++i)
    {
      Framework::playerJoined[i] = false;
      Players[i] = Handle::null;
    }
    state_ = SPAWN;
    spawnTimer = 1.0f;
    spawned = false;

    spawnPos[0] = Vec3(-600.0f, -300.0f, 0.0f);
    spawnPos[1] = Vec3(600.0f, -300.0f, 0.0f);
    spawnPos[2] = Vec3(600.0f, 300.0f, 0.0f);
    spawnPos[3] = Vec3(-600.0f, 300.0f, 0.0f);
    blockLights = false;
  }

  LobbyController::~LobbyController()
  {

  }

  void LobbyController::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(LobbyController::LogicUpdate));
    space->hooks.Add("PlayerDied", self, BUILD_FUNCTION(LobbyController::PlayerDied));
    //space->hooks.Add("Draw", self, BUILD_FUNCTION(LobbyController::Draw));
    bounceDownTimer = .5f;
    playing = false;
    levelSound = space->GetGameObject(owner)->GetComponentHandle(eSoundPlayer);
    powerDownSound = false;
  }

  void LobbyController::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }

  void LobbyController::LogicUpdate(float dt)
  {
    GoToState(dt);
    if (!playing)
    {
      SoundPlayer *sp = space->GetHandles().GetAs<SoundPlayer>(levelSound);
      SoundInstance instance;
      instance.volume = 0.95f;
      instance.mode = PLAY_LOOP;

      sp->Play("TripGBass", &instance);
      playing = true;
    }
  }

  void LobbyController::GoToState(float dt)
  {
    switch (state_)
    {
    case SPAWN:
      Spawn(dt);
      break;
    case JOIN:
      Join(dt);
      break;
    case GAME_START:
      GameStart(dt);
      break;
    }
  }

  void LobbyController::Spawn(float dt)
  {
    if (!spawned)
    {
      //spawn "press A to join"
      for (int i = 0; i < 4; ++i)
      {
        //create join text
        GameObject *jText = (FACTORY->LoadObjectFromArchetype(space, "JoinText"));
        JoinText *joinText = jText->GetComponent<JoinText>(eJoinText);
        //set its player
        joinText->playerNum = i;
        //set handle back to this
        joinText->lobbyController_ = owner;
        //move join text to correct corner
        jText->GetComponent<Transform>(eTransform)->SetTranslation(spawnPos[i]);
      }
      spawned = true;
    }
    if (!bounceDownDone)
      BounceDown(dt);

    spawnTimer -= dt;
    if (spawnTimer <= 0)
    {
      state_ = JOIN;
    }
  }
  static bool soundFlag_ = false;
  void LobbyController::BounceDown(float dt)
  {
    Transform *rt = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);
    SoundEmitter *se = space->GetGameObject(owner)->GetComponent<SoundEmitter>(eSoundEmitter);

    if (bounceDownTimer >= 0.3f)
    {
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, -80.0f, 0.0f));
    }
    else if (bounceDownTimer >= 0.2)
    {
      if (!soundFlag_)
      {
        se->Play("impact1", &SoundInstance(1.0f));
        soundFlag_ = true;
      }
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, 40.0f, 0.0f));
    }
    else
    {
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, -20.0f, 0.0f));
    }
    bounceDownTimer -= dt;
    if (bounceDownTimer <= 0)
    {
      //rt->SetRotation(0.0f);
      bounceDownDone = true;
    }
  }

  void LobbyController::Join(float dt)
  {
    //for loop that spawns the players and keeps them spawned while they are joining
    for (int i = 0; i < 4; ++i)
    {
      if (Framework::playerJoined[i] && Players[i] == Handle::null)
      {
        Players[i] = (FACTORY->LoadObjectFromArchetype(space, playerNames[i]))->self;
        GameObject *temp = space->GetGameObject(Players[i]);
        Transform *playTrans = space->GetGameObject(Players[i])->GetComponent<Transform>(eTransform);
        playTrans->SetTranslation(spawnPos[i]);
      }
    }

    if (startPressed)
    {
      state_ = GAME_START;
      timer_ = 3.5f;
      space->hooks.Call("GameStart");
      SoundPlayer *sp = space->GetHandles().GetAs<SoundPlayer>(levelSound);
      sp->Stop("TripGBass", INSTANT);
      for (int i = 0; i < 4; ++i)
      {
        if (Players[i] != Handle::null)
        {
          space->GetGameObject(Players[i])->Destroy();
        }
      }
      BlockLights::EventData ed;

      ed.duration = 2.0f;
      ed.settings.color = Vec4(0.3f, 0.3f, 0.3f, 0.3f);
      ed.settings.fx = BlockLights::FLICKER;
      ed.settings.customData.duration = 2.0f;

      space->hooks.Call("LightingEvent", (unsigned)0xFFFFFFFF, &ed);
    }
  }

  void LobbyController::GameStart(float dt)
  {
    //roll up lobby text
    Transform *rt = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);
    rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, 80.0f, 0.0f));
    //power-down sounds
    if (!powerDownSound)
      PlayPowerDown();

    timer_ -= dt;
    if (timer_ <= 0)
    {
      
      ENGINE->ChangeLevel("Asteroid");
      space->GetGameObject(owner)->Destroy();
    }
    else if (timer_ <= 1.75f && !blockLights)
    {
      BlockLights::EventData ed;

      ed.duration = 2.75f;
      ed.settings.color = Vec4(0.1f, 0.1f, 0.1f, 0.1f);
      ed.settings.fx = BlockLights::NONE;
      ed.settings.customData.duration = 2.75f;

      space->hooks.Call("LightingEvent", (unsigned)0xFFFFFFFF, &ed);
      blockLights = true;
    }
    
  }

  void LobbyController::PlayerDied(int player, int whoKilledThem)
  {
    Players[player] = Handle::null;
  }

  void LobbyController::PlayPowerDown()
  {
    SoundEmitter *se = space->GetGameObject(owner)->GetComponent<SoundEmitter>(eSoundEmitter);
    se->Play("switch_off", &SoundInstance(1.0f));
    se->Play("power_shutdown", &SoundInstance(1.0f));
    powerDownSound = true;
  }
}