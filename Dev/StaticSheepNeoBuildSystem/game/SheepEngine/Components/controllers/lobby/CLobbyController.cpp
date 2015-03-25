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
  }

  void LobbyController::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }

  void LobbyController::LogicUpdate(float dt)
  {
    GoToState(dt);
    /*if (!playing)
    {
      SoundPlayer *sp = space->GetGameObject(owner)->GetComponent<SoundPlayer>(eSoundPlayer);
      SoundInstance instance;
      instance.volume = 0.95f;
      instance.mode = PLAY_LOOP;

      sp->Play("TripGBass", &instance);
      playing = true;
    }*/
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
      timer_ = 0.5f;
      space->hooks.Call("GameStart");
    }
  }

  void LobbyController::GameStart(float dt)
  {
    //roll up lobby text
    Transform *rt = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);
    rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, 80.0f, 0.0f));

    timer_ -= dt;
    if (timer_ <= 0)
    {
      /*SoundPlayer *sp = space->GetGameObject(owner)->GetComponent<SoundPlayer>(eSoundPlayer);
      sp->Stop("TripGBass", FADE_FAST);*/
      ENGINE->ChangeLevel("Asteroid");
      space->GetGameObject(owner)->Destroy();
    }

  }

  void LobbyController::PlayerDied(int player, int whoKilledThem)
  {
    Players[player] = Handle::null;
  }
}